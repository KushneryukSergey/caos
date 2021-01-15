## How to execute fork-bomb

Use next commands:
sudo useradd tmp_user # create new user
sudo passwd tmp_user  # set password
su tmp_user           # log in as new user in current terminal window
ulimit -u 100         # set limit for amount of process for this user
./main.exe            # execute *DaNgErOuS* programm
userdel tmp_user      # delete user

or write smth like this:
```
gcc inf09_0.c -o inf09_0.exe
sudo useradd tmp_user
sudo -u tmp_user bash -c 'ulimit -u 5 ; ./main.exe' 
sudo userdel tmp_user
```
