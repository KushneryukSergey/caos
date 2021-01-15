//
// Created by skushneryuk on 16/11/2020.
//

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <immintrin.h>

typedef struct {
    // File Header
    uint16_t signature;     // 00
    uint32_t file_size;     // 02
    uint32_t reserved;      // 06
    uint32_t pixel_offset;  // 0A

    // Header Info
    uint32_t header_size;   // 0E
    int32_t width;         // 12
    int32_t height;        // 16
    uint16_t colour_planes; // 1A
    uint16_t bits_per_pixel;// 1C ---- 32 bits defined by the task

    // useless
    uint8_t unused[40];     // 1E

    // pixels info
    uint32_t *pixels;       // Data
} BMP;


void read_image(const char *file_name, BMP *bmp_file) {
    FILE *image_file = fopen(file_name, "rb");

    fread(&bmp_file->signature, sizeof(uint16_t), 1, image_file);
    fread(&bmp_file->file_size, sizeof(uint32_t), 1, image_file);
    fread(&bmp_file->reserved, sizeof(uint32_t), 1, image_file);
    fread(&bmp_file->pixel_offset, sizeof(uint32_t), 1, image_file);
    fread(&bmp_file->header_size, sizeof(uint32_t), 1, image_file);
    fread(&bmp_file->width, sizeof(int32_t), 1, image_file);
    fread(&bmp_file->height, sizeof(int32_t), 1, image_file);
    fread(&bmp_file->colour_planes, sizeof(uint16_t), 1, image_file);
    fread(&bmp_file->bits_per_pixel, sizeof(uint16_t), 1, image_file);
    fread(&bmp_file->unused, sizeof(bmp_file->unused), 1, image_file);

    int32_t updater = bmp_file->width;
    int32_t abs_height = abs(bmp_file->height);

    bmp_file->pixels = (uint32_t *) malloc(updater * abs_height * sizeof(uint32_t));

    uint32_t *pointer = bmp_file->pixels;

    if (abs_height > 0) {
        pointer += ((abs_height - 1) * updater);
        updater = -updater;
        printf("Hey\n");
    }

    for (int i = 0; i < abs_height; i++) {
        fread(pointer, sizeof(uint32_t), bmp_file->width, image_file);
        pointer += updater;
    }

    fclose(image_file);
}

void write_image(const char *file_name, BMP *bmp_file) {
    FILE *image_file = fopen(file_name, "wb");

    fwrite(&bmp_file->signature, sizeof(uint16_t), 1, image_file);
    fwrite(&bmp_file->file_size, sizeof(uint32_t), 1, image_file);
    fwrite(&bmp_file->reserved, sizeof(uint32_t), 1, image_file);
    fwrite(&bmp_file->pixel_offset, sizeof(uint32_t), 1, image_file);
    fwrite(&bmp_file->header_size, sizeof(uint32_t), 1, image_file);
    fwrite(&bmp_file->width, sizeof(int32_t), 1, image_file);
    fwrite(&bmp_file->height, sizeof(int32_t), 1, image_file);
    fwrite(&bmp_file->colour_planes, sizeof(uint16_t), 1, image_file);
    fwrite(&bmp_file->bits_per_pixel, sizeof(uint16_t), 1, image_file);
    fwrite(&bmp_file->unused, sizeof(bmp_file->unused), 1, image_file);


    int32_t updater = bmp_file->width;
    int32_t abs_height = abs(bmp_file->height);

    uint32_t *pointer = bmp_file->pixels;

    if (abs_height > 0) {
        pointer += ((abs_height - 1) * updater);
        updater = -updater;
        printf("Hey\n");
    }

    for (int i = 0; i < abs_height; i++) {
        fwrite(pointer, sizeof(uint32_t), bmp_file->width, image_file);
        pointer += updater;
    }

    fclose(image_file);
}

void compose_pixel(uint32_t* destination, uint32_t* source){
    uint32_t mid = *destination;
    uint32_t AD = (mid % 256); mid >>= 8;
    uint32_t RD = (mid % 256); mid >>= 8;
    uint32_t GD = (mid % 256); mid >>= 8;
    uint32_t BD = (mid % 256); mid >>= 8;

    mid = *source;
    uint32_t AS = (mid % 256); mid >>= 8;
    uint32_t RS = (mid % 256); mid >>= 8;
    uint32_t GS = (mid % 256); mid >>= 8;
    uint32_t BS = (mid % 256); mid >>= 8;

    __m128i result;
    __m128 sour, dest, dest_alpha, sour_alpha, res_alpha, not_s_alpha, mask255;

    mask255 = _mm_cvtepi32_ps(_mm_set_epi32(255, 255, 255, 255));

    dest = _mm_cvtepi32_ps(_mm_set_epi32(AD, RD, GD, BD));
    sour = _mm_cvtepi32_ps(_mm_set_epi32(AS, RS, GS, BS));

    dest_alpha = _mm_div_ps(_mm_permute_ps(dest, 0xff), mask255); // vector with destination alpha
    sour_alpha = _mm_div_ps(_mm_permute_ps(sour, 0xff), mask255); // vector with source alpha

    not_s_alpha = _mm_cvtepi32_ps(_mm_set_epi32(1, 1, 1, 1));
    not_s_alpha = _mm_sub_ps(not_s_alpha, sour_alpha); // vector with 1 - source alpha
    res_alpha = _mm_add_ps(sour_alpha, _mm_mul_ps(not_s_alpha, dest_alpha)); // vector of result alpha

    dest = _mm_mul_ps(dest, dest_alpha);
    dest = _mm_mul_ps(dest, not_s_alpha);
    sour = _mm_mul_ps(sour, sour_alpha);
    dest = _mm_add_ps(dest, sour);
    dest = _mm_div_ps(dest, res_alpha); // counting colors

    dest[3] = _mm_mul_ps(res_alpha, mask255)[0];
    result = _mm_cvtps_epi32(dest);

    AD = result[1] >> 32;
    RD = result[1];
    GD = result[0] >> 32;
    BD = result[0];

    *destination = (BD << 24) | (GD << 16) | (RD << 8) | AD;
}


int main(int argc, char *argv[]) {
    char *destination = argv[1];
    char *source = argv[2];
    char *result = argv[3];
    BMP bmp_destination;
    BMP bmp_source;

    read_image(destination, &bmp_destination);
    read_image(source, &bmp_source);

    int height = abs(bmp_source.height);
    int width = bmp_source.width;

    uint32_t* destination_pointer = bmp_destination.pixels;
    uint32_t* source_pointer = bmp_source.pixels;

    for(int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j) {
            compose_pixel(destination_pointer + i * bmp_destination.width + j,
                          source_pointer + i * width + j);
        }
    }

    write_image(result, &bmp_destination);
    return 0;
}