//
// Created by skushneryuk on 04/11/2020.
//

#include <iostream>
#include <stdio.h>
#include <deque>
#include <stack>
#include <map>

typedef uint32_t Instruction;

enum REGISTER : Instruction {
    R0 = 0,
    R1 = 1,
    R2 = 2,
    R3 = 3,
    R4 = 4,
    R5 = 5,
    R6 = 6,
    R7 = 7,
    R8 = 8,
    R9 = 9,
    R10 = 10,
    R11 = 11,
    R12 = 12,
    SP = 13,
    LR = 14,
    PC = 15
};


enum INSTRUCTION_TEMPLATE : Instruction {
    ADD_INSTR = 0xe0800000,  // add rD, rL, rR   e08LD00R | D - dest register; R - right register; L - right register
    SUB_INSTR = 0xe0400000,  // sub rD, rL, rR   e04LD00R | D - dest register; R - right register; L - right register
    MUL_INSTR = 0xe0000090,  // mul rD, rL, rR   e00D0R9L | D - dest register; R - right register; L - right register
    ORR_INSTR = 0xe3800000,  // orr rD, rN, #val e38NDSVV | N - register number;
                             // S - even_right_shift / 2; VV - value to be shifted
    POP_INSTR = 0xe49d0004,  // pop rN           e49dN004 | N - register number
    PUSH_INSTR = 0xe52d0004, // push rN          e52dN004 | N - register number
    BLX_INSTR = 0xe12fff30,  // blx rN           e12fff3N | N - register number
    MOV_INSTR = 0xe3a00000,  // mov rN, #val     e3a0NSVV | N - register number;
                             // S - even_right_shift / 2; VV - value to be shifted
    LDR_INSTR = 0xe5900000,  // ldr rD, [rN]     e59ND000 | N - register number, D - dest register
    BX_INSTR = 0xe12fff10    // bx rN            e12fff1N | N - register number
};


Instruction get_add_instruction(REGISTER dest, REGISTER left, REGISTER right){
    return ADD_INSTR | right | (dest << 12) | (left << 16);
}

Instruction get_sub_instruction(REGISTER dest, REGISTER left, REGISTER right){
    return SUB_INSTR | right | (dest << 12) | (left << 16);
}

Instruction get_mul_instruction(REGISTER dest, REGISTER left, REGISTER right){
    return MUL_INSTR | left | (right << 8) | (dest << 16);
}

Instruction get_orr_instruction(REGISTER dest, REGISTER number, uint32_t value, uint32_t shift){
    if(value != 0){
        while ((value & 3) == 0) {
            value = value >> 2;
            --shift;
        }
    } else {
        shift = 0;
    }
    return ORR_INSTR | value | (shift << 8) | (dest << 12) | (number << 16);
}

Instruction get_pop_instruction(REGISTER number){
    return POP_INSTR | (number << 12);
}

Instruction get_push_instruction(REGISTER number){
    return PUSH_INSTR | (number << 12);
}

Instruction get_blx_instruction(REGISTER number){
    return BLX_INSTR | number;
}

Instruction get_mov_instruction(REGISTER number, uint32_t value, uint32_t shift){
    return MOV_INSTR | value | (shift << 8) | (number << 12);
}

Instruction get_ldr_instruction(REGISTER dest, REGISTER number){
    return LDR_INSTR | (dest << 12) | (number << 16);
}

Instruction get_bx_instruction(REGISTER number){
    return BX_INSTR | number;
}

typedef struct {
    const char *name;
    void *pointer;
} symbol_t;


//-----------------
//      Token
//-----------------
enum TOKEN_TYPE {
    SUM, SUB, MUL, FUNC, UNARY_MINUS, OPEN_PAR, CLOSE_PAR, CONSTANT, VAR, END
};

enum PRECEDENCE : std::size_t {
    MOCK_P = 0,
    SUM_P = 1,
    SUB_P = 1,
    MUL_P = 2,
    UNARY_MINUS_P = 3,
    FUNC_P = 4
};

class JitCompiler;

class Token {
public:
    TOKEN_TYPE type; // token type
    PRECEDENCE precedence; // for functions and operators
//private:
    uint32_t value; // constant values
    std::size_t arity; // for functions and operators
    int *addr; // address for vars and functions
public:
    Token() {}
    Token(TOKEN_TYPE type, uint32_t value, std::size_t arity, PRECEDENCE precedence, int *addr);


    void put_instructions(JitCompiler &);

    void put_sum_instructions(JitCompiler&);
    void put_sub_instructions(JitCompiler&);
    void put_mul_instructions(JitCompiler&);
    void put_func_instructions(JitCompiler&);
    void put_unary_minus_instructions(JitCompiler&);
    void put_const_instructions(JitCompiler&);
    void put_var_instructions(JitCompiler&);
    void load_value_to_register(JitCompiler&, uint32_t, REGISTER);
    bool is_value();
    bool is_operation();
    void update_arity(std::size_t);
};


//-----------------
//    JitParser
//-----------------
class JitParser {
    std::map<std::string, int *> address;
    std::deque <Token> parsed;
    std::stack <Token> operations;
    std::string name;
    bool only_digits;
    bool minus_may_be_unary;

    void preparation();

    void check_name();
    void check_opar(); // open parenthesis
    void check_cpar(); // close parenthesis
    void check_minus();
    void check_plus();
    void check_mult();
    void check_comma();
    void check_non_op_symbols(char symbol);

public:
    JitParser() = delete;
    JitParser(const symbol_t *externs);
    void parse_expression(const char *);
    std::deque <Token>& get_parsed();
};


//-----------------
//   JitCompiler
//-----------------
class JitCompiler{
    JitCompiler() = delete;
    uint32_t* out_buffer;
    JitParser parser;
    std::size_t index;
public:
    JitCompiler(const symbol_t *externs);

    void put_instruction(Instruction instruction);
    void compile(const char*, uint32_t*);
};



//---------------------------
//      Token Functions
//---------------------------

Token::Token(TOKEN_TYPE type, uint32_t value, std::size_t arity, PRECEDENCE precedence, int* addr):
        type(type),
        precedence(precedence),
        value(value),
        arity(arity),
        addr(addr){
}

bool Token::is_value(){
    return type == CONSTANT || type == VAR;
}

bool Token::is_operation() {
    return type == SUM || type == SUB || type == MUL || type == UNARY_MINUS || type == FUNC;
}

void Token::update_arity(std::size_t value) {
    arity += value;
}

void Token::load_value_to_register(JitCompiler &jit_compiler, uint32_t value, REGISTER reg) {
    //std::cout << std::hex << value << std::endl;
    jit_compiler.put_instruction(get_mov_instruction(reg, value & ((1u << 8) - 1u), 0));
    value = value >> 8;
    if (value & ((1u << 8) - 1u))
        jit_compiler.put_instruction(get_orr_instruction(reg, reg, value & ((1u << 8) - 1u), 12));
    value = value >> 8;
    if (value & ((1u << 8) - 1u))
        jit_compiler.put_instruction(get_orr_instruction(reg, reg, value & ((1u << 8) - 1u), 8));
    value = value >> 8;
    if(value & ((1u << 8) - 1u))
        jit_compiler.put_instruction(get_orr_instruction(reg, reg, value & ((1u << 8) - 1u), 4));
}

void Token::put_instructions(JitCompiler &jit_compiler) {
    if(type == SUM) put_sum_instructions(jit_compiler);
    if(type == SUB) put_sub_instructions(jit_compiler);
    if(type == MUL) put_mul_instructions(jit_compiler);
    if(type == FUNC) put_func_instructions(jit_compiler);
    if(type == UNARY_MINUS) put_unary_minus_instructions(jit_compiler);
    if(type == CONSTANT) put_const_instructions(jit_compiler);
    if(type == VAR) put_var_instructions(jit_compiler);
}

void Token::put_sum_instructions(JitCompiler& jit_compiler) {
    jit_compiler.put_instruction(get_pop_instruction(R1));
    jit_compiler.put_instruction(get_pop_instruction(R0));
    jit_compiler.put_instruction(get_add_instruction(R0, R0, R1));
    jit_compiler.put_instruction(get_push_instruction(R0));
}

void Token::put_sub_instructions(JitCompiler& jit_compiler) {
    jit_compiler.put_instruction(get_pop_instruction(R1));
    jit_compiler.put_instruction(get_pop_instruction(R0));
    jit_compiler.put_instruction(get_sub_instruction(R0, R0, R1));
    jit_compiler.put_instruction(get_push_instruction(R0));
}

void Token::put_mul_instructions(JitCompiler& jit_compiler) {
    jit_compiler.put_instruction(get_pop_instruction(R1));
    jit_compiler.put_instruction(get_pop_instruction(R0));
    jit_compiler.put_instruction(get_mul_instruction(R0, R0, R1));
    jit_compiler.put_instruction(get_push_instruction(R0));
}

void Token::put_func_instructions(JitCompiler& jit_compiler) {
    if(arity > 3) jit_compiler.put_instruction(get_pop_instruction(R3));
    if(arity > 2) jit_compiler.put_instruction(get_pop_instruction(R2));
    if(arity > 1) jit_compiler.put_instruction(get_pop_instruction(R1));
    if(arity > 0) jit_compiler.put_instruction(get_pop_instruction(R0));

    uint32_t value = reinterpret_cast<uintptr_t>(addr);
    load_value_to_register(jit_compiler, value, LR);
    jit_compiler.put_instruction(get_blx_instruction(LR));
    jit_compiler.put_instruction(get_push_instruction(R0));
}

void Token::put_unary_minus_instructions(JitCompiler& jit_compiler) {
    jit_compiler.put_instruction(get_mov_instruction(R0, 0, 0));
    jit_compiler.put_instruction(get_pop_instruction(R1));
    jit_compiler.put_instruction(get_sub_instruction(R0, R0, R1));
    jit_compiler.put_instruction(get_push_instruction(R0));
}

void Token::put_const_instructions(JitCompiler& jit_compiler) {
    load_value_to_register(jit_compiler, value, R0);
    jit_compiler.put_instruction(get_push_instruction(R0));
}

void Token::put_var_instructions(JitCompiler& jit_compiler) {
    load_value_to_register(jit_compiler, reinterpret_cast<uintptr_t>(addr), R0);
    jit_compiler.put_instruction(get_ldr_instruction(R0, R0));
    jit_compiler.put_instruction(get_push_instruction(R0));
}



//---------------------------
//    JitParser Functions
//---------------------------

JitParser::JitParser(const symbol_t *externs) {
    while(externs->name != 0 || externs->pointer != 0){
        address[std::string(externs->name)] = static_cast<int*>(externs->pointer);
        ++externs;
    }
}

void JitParser::check_name() {
    if (!name.empty()){
        if(only_digits){
            parsed.push_back(Token(CONSTANT, std::stoi(name), 0, MOCK_P, nullptr));
        } else {
            parsed.push_back(Token(VAR, 0, 0, MOCK_P, address[name]));
        }
        name = "";
        only_digits = true;
    }
}

void JitParser::check_opar() {
    minus_may_be_unary = true;
    if (!name.empty()){
        operations.push(Token(FUNC, 0, 1, FUNC_P, address[name]));
        operations.top().update_arity(1);
        name = "";
        only_digits = true;
    }
    operations.push(Token(OPEN_PAR, 0, 0, MOCK_P, nullptr));
}

void JitParser::check_cpar() {
    check_name();

    minus_may_be_unary = false;
    while (operations.top().type != OPEN_PAR) {
        parsed.push_back(operations.top());
        operations.pop();
    }
    operations.pop();
    if (!operations.empty() && operations.top().type == FUNC) {
        operations.top().update_arity(-1);
    }
}

void JitParser::check_minus() {
    check_name();

    if (minus_may_be_unary) {
        operations.push(Token(UNARY_MINUS, 0, 2, UNARY_MINUS_P, nullptr));
    } else {
        minus_may_be_unary = true;
        while (!operations.empty() && operations.top().is_operation() && operations.top().precedence >= SUB_P) {
            parsed.push_back(operations.top());
            operations.pop();
        }
        operations.push(Token(SUB, 0, 2, SUB_P, nullptr));
    }
}

void JitParser::check_plus() {
    check_name();

    minus_may_be_unary = true;
    while (!operations.empty() && operations.top().is_operation() && operations.top().precedence >= SUM_P) {
        parsed.push_back(operations.top());
        operations.pop();
    }
    operations.push(Token(SUM, 0, 2, SUM_P, nullptr));
}

void JitParser::check_mult() {
    check_name();

    minus_may_be_unary = true;
    while (!operations.empty() && operations.top().is_operation() && operations.top().precedence >= MUL_P) {
        parsed.push_back(operations.top());
        operations.pop();
    }
    operations.push(Token(MUL, 0, 2, MUL_P, nullptr));
}

void JitParser::check_comma() {
    minus_may_be_unary = true;
    check_name();
    while (operations.top().type != OPEN_PAR) {
        parsed.push_back(operations.top());
        operations.pop();
    }
    operations.pop();
    if (!operations.empty() && operations.top().type == FUNC) {
        operations.top().update_arity(1);
    }
    operations.push(Token(OPEN_PAR, 0, 0, MOCK_P, 0));
}


void JitParser::check_non_op_symbols(char symbol) {
    minus_may_be_unary = false;
    if (only_digits && !isdigit(symbol)) {
        only_digits = false;
    }
    name.push_back(symbol);
}

void JitParser::preparation(){
    name = "";
    while (!operations.empty()) operations.pop();
    parsed.clear();
    minus_may_be_unary = true;
    only_digits = true;
}

void JitParser::parse_expression(const char *expression) {
    preparation();
    std::size_t index = 0;
    while (*(expression + index) != '\0'){
        //std::cout << *(expression + index) << std::endl;
        if(isspace(*(expression + index))){
            ++index;
            continue;
        } else if(*(expression + index) == '('){
            check_opar();
        } else if(*(expression + index) == ','){
            check_comma();
        } else if(*(expression + index) == ')'){
            check_cpar();
            if (!operations.empty() && operations.top().type == FUNC && *(expression + index - 1) == '(') {
                operations.top().update_arity(-1);
            }
        } else if(*(expression + index) == '+'){
            check_plus();
        } else if(*(expression + index) == '-'){
            check_minus();
        } else if(*(expression + index) == '*'){
            check_mult();
        } else {
            check_non_op_symbols(*(expression + index));
        }
        ++index;
    }
    check_name();
    while(!operations.empty()){
        parsed.push_back(operations.top());
        operations.pop();
    }
    parsed.push_back(Token(END, 0, 0, MOCK_P, 0));
}

std::deque<Token>& JitParser::get_parsed() {
    return parsed;
}


//---------------------------
//   JitCompiler Functions
//---------------------------

void JitCompiler::put_instruction(Instruction instruction) {
    //std::cout << std::hex << instruction << std::endl;
    *(out_buffer + index++) = instruction;
}

void JitCompiler::compile(const char *expression, uint32_t* buffer) {
    parser.parse_expression(expression);
    std::deque<Token>& parsed = parser.get_parsed();

    out_buffer = buffer;
    index = 0;

    put_instruction(get_push_instruction(LR));
    while(parsed.front().type != END) {
        //std::cout << parsed.front().type << ' ' << parsed.front().arity << ' ' << parsed.front().addr << std::endl;
        //std::cout << parsed.front().type << " ";
        parsed.front().put_instructions(*this);
        parsed.pop_front();
    }
    put_instruction(get_pop_instruction(R0));
    put_instruction(get_pop_instruction(LR));
    put_instruction(get_bx_instruction(LR));
}

JitCompiler::JitCompiler(const symbol_t* externs): parser(externs) {}


extern "C" void jit_compile_expression_to_arm(const char * expression,
                              const symbol_t * externs,
                              void* out_buffer){
    //printf("%s\n", expression);
    JitCompiler compiler(externs);
    uint32_t* uint_buffer = static_cast<uint32_t*>(out_buffer);
    compiler.compile(expression, uint_buffer);
    //std::cout << std::hex << get_orr_instruction(R0, R0, 0, 0) << std::endl;
    //std::cout << std::hex << get_orr_instruction(R0, R0, 32, 12) << std::endl;
}