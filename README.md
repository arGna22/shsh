# shsh

## What is this?
shsh is a basic shell implemented in the C programming language. 

## Functionality
* Shell builtins
* User commands
* I/O redirection: 
	* `>` to redirect `stdout` (overwrites)
	* `>>` to redirect `stdout` (appends)
	* `>>>` to redirect `stderr` (overwrites)
	* `<` to redirect `stdin`

## Installation
1. Clone this repository: `git clone https://github.com/arGna22/shsh.git && cd shsh`
2. Build the shell using the Makefile: `make main`
3. Execute the shell `./main`
