# 🐚 Minishell

### This project has been created by Tetiana Koval (tkoval) and Yevheniia Minashkina (yminashk)

Minishell is a simplified Unix shell written in C.
The goal of this project is to reproduce the behavior of a real shell such as Bash by implementing command parsing, environment variable expansion, pipes, redirections, built-in commands and process management.

This project provided hands-on experience with:

- Process creation using fork()
- Program execution using execve()
- Inter-process communication with pipes
- File descriptor manipulation with dup2()
- Signal handling
- Environment variable management
- Parsing and lexical analysis

The implementation aims to mimic Bash behavior as closely as possible for the features required by the subject.


## Instructions
### Compile
Compile the project:  
make  

Recompile the project from scratch:  
make re  

Remove object files:  
make clean  

Remove object files and the executable:  
make fclean

### RUN
./minishell

### Example:

minishell$ echo Hello World  
Hello World

minishell$ export USER=marvin  
minishell$ echo $USER  
marvin

minishell$ ls -l | grep minishell
...

### Valgrind
Run Valgrind with a readline suppression file:  
make valgrind

Runs Valgrind with leak checking and suppresion:    
valgrind --leak-check=full --track-origins=yes --suppresions=readline.supp ./minishell

## Features
### Builtins
- echo
- cd
- pwd
- export
- unset
- env
- exit

### Command execution
- Execution of commands using PATH
- Relative and absolute paths
- Exit status handling
- Error management similar to Bash

### Pipes
echo hello | cat  
seq 100 | grep 42  
yes | head -n 1

### Redirections
cat < infile  
echo hello > outfile  
echo hello >> outfile  
cat << EOF

### Environment variables
echo $HOME  
echo $PATH  
export USER=tkoval  

### Quotes
- Single quotes '
- Double quotes "
- Variable expansion inside double quotes
- Protection of literals inside single quotes

## Resources
Official documentation:
- GNU Bash Manual
- GNU Readline Manual
- POSIX Shell Command Language
- Linux man pages (fork, execve, pipe, dup2, waitpid, signal)

Useful references:
- https://m4nnb3ll.medium.com/minishell-building-a-mini-bash-a-42-project-b55a10598218

AI Usage

Artificial Intelligence tools (ChatGPT) were used as an auxiliary resource for:

- Understanding Bash behavior and edge cases;
- Debugging memory leaks and file descriptor issues;
- Discussing architecture choices;

All final design decisions, implementation and testing were carried out by the authors of the project.