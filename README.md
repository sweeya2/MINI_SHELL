# MINI_SHELL
### PROGRAMMING LANGUAGE USED : C++
### OPERATING SYSTEM : LINUX

### Initial two arguments to execute the shell:
```
g++ mini_shell.cpp
./a.out
```

### KEY FEATURES INCLUDED:
1. Environment variables
2. Path functions
3. Parsing
4. Piping
5. Input and Output redirection:
Read mode
Write mode
Append mode
### VARIOUS COMMANDS : APPLICATIONS
1. USER, PATH, HOME, SHELL, TERM : environment variables
2. cd : change directory to /home/user
3. cd (some directory) : changes directory to the given directory
4. history : to get all the input you've given before exit
5. history exit : gets all the input you've given before exit and then exits from the shell
6. exit (or) quit (or) x : exits from the shell
7. (path functions) : e.g. ls
8. (command1)|(command2) : piping and executing both commands
9. (command)>(output.txt) : executes the command and outputs it to output.txt (write mode)
10. (command)>>(output.txt) : executes the command and adds the output to output.txt (append mode)
11. (command)<(input.txt) : executes the command using the input form input.txt (read mode)
12. setenv (environment variable) = (new value) : sets new value for the given environment variable
13. help : opens all the command and their syntax  
(you don't actually have to put the paranthesis () as per the above explanation)
### NOTE:
1. Piping : type both the command with no space before and after the pipe symbol (|)
2. Redirection : type both the command with no space before and after the redirection symbol (> , <, >>)
3. At every stage you get an error message saying "wrong input" when you type some wrong input
4. Do not include single or double quotes in any of the commands you type
5. setenv : type command as setenv (environment variable) = (new value)
6. only single piping and redirection
