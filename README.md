# POSIX Shell Implementation

This is a basic implementation of a shell in C++. It provides a command-line interface with several built-in commands and the ability to execute external programs.

## Overview

The shell implements a Read-Eval-Print Loop (REPL) that:
1. Displays a prompt
2. Reads user input
3. Parses the input into a command and arguments
4. Executes the command
5. Repeats

Built-in commands are handled directly by the shell, while external commands are executed by searching the system PATH.

## Build Instructions

To build the shell, you'll need:
- A C++ compiler supporting C++17 (e.g., GCC 7+ or Clang 5+)
- The standard C++ library

Follow these steps to build the shell:

```
g++ -std=c++17 main.cpp -o simple_shell
```

This will create an executable named `main` in the current directory.

## Execution Instructions

To run the shell:

1. Open a terminal
2. Navigate to the directory containing the `simple_shell` executable
3. Run the following command:

```
./main
```

You should now see the shell prompt `$`, indicating that the shell is ready to accept commands.

**ALTERNATIVELY**,

You can build and run the shell by executing,

```
./your_shell.sh
```

This will build and run the executable and you will see the shell prompt `$`, indicating that the shell is ready to accept commands.

To run this step you should have `vcpkg` ececutable installed, and you should clone the [vcpkg](https://github.com/microsoft/vcpkg) repository and save its path as the environment variable `VCPKG_ROOT`.

## Available Commands

The shell supports the following built-in commands:

1. `okay`: Prints "All okay!"
2. `exit [status]`: Exits the shell with the provided status code
3. `echo [args...]`: Prints the provided arguments
4. `type [command]`: Identifies whether a command is a shell builtin or an external program
5. `pwd`: Prints the current working directory
6. `cd [directory]`: Changes the current working directory

In addition to these built-in commands, the shell can execute any external programs available in the system PATH.

## Usage Examples

```
$ okay
All okay!

$ echo Hello, World!
Hello, World!

$ pwd
/home/user/current/directory

$ cd ..
$ pwd
/home/user/current

$ type echo
echo is a shell builtin

$ type ls
ls is /bin/ls

$ ls
file1.txt file2.txt directory1

$ exit 0
```

**Note: The actual output may vary depending on your system configuration and current directory contents.**

Caution: Some parts of this readme were genrated by an LLM, others were written by a human.