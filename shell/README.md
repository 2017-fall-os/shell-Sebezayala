## Sebastian Ayala Urtaza
## Simple Unix Shell

Shell version 2.1

This shell supports the use of :
- Simple commands (e.g. $/bin/ls or $ ls)
- Simple pipes (e.g. $ /bin/ls | /bin/sort -r)
- Background tasks(e.g. $ find /etc -print & )
- cd command to change directories (e.g. cd ..)
- Redirection of input and output (e.g. $ ls > /tmp/files.txt), however it does not support multiple redirection (e.g.$ ls > /tmp/files.txt > /tmp/files2.txt)
- Use of the PS1 environmental variable if exported beforehand
- setting enviroment variables: "var=Value"

Passes all the tests of testShell.sh

## To Compile
Simply run make from the shell folder by typing make in the terminal.
You can delete the files created by make, by typing make clean in the termianl.

## To Use 
After compiling run myShell from the shell folder by typing ./myShell in the terminal.
You will be prompted by a the prompt string 1 if exported or a default $ sign here you can start typing commands.
You can exit the shell with the word "exit"
