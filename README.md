# Welcome to My Printf
***


## Task
The challenge was to recreate a version of the printf function.


## Description
I solved the problem by:
- Getting the arguments (fixed and variable length arguments)
- Going through all the characters and printing them (except for the %, where I retrieve the next argument)
- Returning the number of characters printed


## Installation
To install the project, you can run:
```bash
gcc -o my_printf my_printf.c
```
or another gcc. It can also work with other compilers and these flags: -Wall -Wextra -Werror -g3 -fsanitize=address

You can also use the Makefile with the make command:
```bash
make && make clean
```
Now you can simply run with ./my_printf

## Usage
First, I initialize: `va_list args; va_start(args, format);` to manage variable length arguments.

Next, I declare an array of type `t_write`, a structure made up of a character and a pointer to a function.

I declare all the formatted characters with their associated function. For example, 'd' represents a digit and is associated with the `write_d` function, which I use to write a number to standard output.

Then, I use a while loop on the formatted string passed as an argument to go through each character.

If the character is not a '%', I simply print it with the write function and increment the count variable (initialized at the beginning).

If it is a '%', I use a while loop to go through all the elements of the array initialized previously. I check if the character in the array matches the character after the %, and if there is a match, I use the associated function.


### The Core Team


<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px' /></span>
