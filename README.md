# Basic Unix Shell

A very limited shell that supports:
- Variable assignment for any variable (`$HOME=/Users/...`)
- Program execution (`ls`, `cd`, ...)
- Printing variables (`printenv`)
- Spacing and escape characters (`cd "my directory"` or `cd my\ directory`)
- Quit the shell (`exit`)

To run the shell, first change the `profile` file such that it contains the appropriate `HOME` and `PATH` variables.
Then run `make` to compile and finally to actually run `./main` to run the shell.

## Project Structure
```
.
├── environment.c    # Parses the profile file and sets up the virtual environment
├── environment.h    # Stores the environment in global variables and defines the records struct
├── execute.c        # Responsible for executing a given program
├── execute.h
├── main.c           # 'Glues' other files together with while loop simulates the shell
├── MakeFile         # Allows user to compile file with `make`
├── path.c           # Parses the PATH variable and looks for program strings in PATH
├── path.h           # Defines linked list for PATH
├── profile
├── README.md
├── strings.c        # Parses strings according to a delimiter. Also supports dynamic string sizes and escape characters
└── strings.h        # Defines standard string length
```
