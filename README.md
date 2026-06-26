# Minishell - As Beautiful as a Shell Can Get

*This project has been created as part of the 42 curriculum by omiskiny and recan.*

# Description

The **Minishell** project is a comprehensive introduction to the inner workings of a Unix operating system. The goal is to create a fully functional, miniature command-line interpreter from scratch that closely mimics the core functionalities of Bash. By handling process creation, environment manipulation, data redirection, and synchronous pipeline execution, this project demonstrates how modern operating systems manage user commands.

This project significantly strengthens hands-on experience in:
- **Process Management**: Utilizing `fork()`, `waitpid()`, and executing binaries via `execve()`.
- **Inter-Process Communication (IPC)**: Constructing robust Unix data pipelines (`pipe()`).
- **File Descriptor Manipulation**: Redirecting input/output streams dynamically using `dup2()`.
- **Lexical Analysis & Parsing**: Tokenizing raw user strings into structured, executable nodes.
- **State Machine Syntax Control**: Implementing advanced character tracking for quotes.
- **Memory & Resource Management**: Preventing memory leaks across persistent runtime environments.

The program creates an interactive command loop, prompting the user for input and executing system binaries or built-in commands natively.

---

# 📐 Instructions & Implementation Overview

### 1. Lexer & Parser (The Syntax Engine)
- The raw command string entered by the user is systematically tokenized into logical blocks (Words, Pipes, Redirections).
- The parser organizes these tokens into a sequential linked list of commands (`t_cmd`), forming a clean execution tree.

### 2. The Expander Module (`expander.c`)
- Before a command can be executed, the Expander processes the arguments using a dedicated **State Machine** tracker.
- **Single Quotes (`'...'`)**: Suppresses all expansions; characters inside are treated as literal text.
- **Double Quotes (`"..."`)**: Permits environmental variable (`$`) expansion while keeping the grouped arguments as a single unit.
- **Quote Removal**: Once environmental values are resolved, the outer quote wrappers are stripped entirely from the argument arrays before passing them to the operating system.

### 3. The Executor Engine
- **Built-in Scoping**: Built-ins that alter the state of the shell itself (like `cd`, `export`, `unset`, `exit`) are executed directly within the parent process.
- **System Binaries**: Non-built-in commands search through the `PATH` environment variable, spawn isolated child processes via `fork()`, and replace the child's image using `execve()`.
- **Pipelines**: Seamlessly forwards data down the chain by binding the `stdout` of a preceding process to the `stdin` of the following command using `pipe()`.

### 4. Redirections & Heredoc (`<<`)
- **Stream Interception**: Handles `<` (input redirection), `>` (output truncating), and `>>` (output appending) by cloning open file descriptors using `dup2()`.
- **Heredoc Management**: Before execution, the shell processes `<<` blocks by capturing real-time input into a temporary file until a delimiter keyword is reached, safely serving it as input later.

---

## 🧮 Theoretical & Logic Foundations

The core stability of this interpreter stems from the application of deterministic parsing and state transitions:

### 4.1 Token State Machine
The shell evaluates strings character by character. To safely handle complex strings, double quote (`dq`) and single quote (`sq`) boolean flags alternate states natively:
c
if (word[i] == '\'' && !dq)
    sq = !sq;
else if (word[i] == '"' && !sq)
    dq = !dq;

This state architecture guarantees that a character like $ is strictly expanded only when !sq evaluates to true (outside of literal single quotes).

### 4.2 Pipeline Synchronization

When multiple commands are linked by pipelines, the shell forks multiple concurrent child processes. To avoid zombie processes and accurately catch exit codes, the parent process synchronously evaluates them:
Exit Code=WEXITSTATUS(status)

This exit code is immediately injected into the global reference structure (g_data.exit_code) so the expander can fetch it instantly whenever $? is typed.

## Mandatory Configuration & Rules

Implemented Built-ins

- echo (with support for the -n flag, handling repeating variations like -n -n -nnnn)

- cd (handles relative/absolute paths, tracks and alters system PWD and OLDPWD)

- pwd (prints the absolute path of the current directory using getcwd)

- export (adds/updates environment variables, prints variables safely with declare -x)

- unset (safely unlinks and frees environmental key-value configurations)

- env (prints active environment pairs that contain defined data values)

- exit (cleans up system resources and shuts down the shell with exact status codes)

## Error Handling

The program strictly validates operations to avoid system crashes:

- Non-numeric or excess arguments given to exit are intercepted gracefully.
- Invalid variable identifiers passed to export or unset (e.g. starting with numbers or symbols) are flagged.
- Commands that do not exist or cannot be found in the path yield a standard 127 exit code.

## Memory Management

- Every command array, pipeline tracker, and environmental node is freed at the end of each command loop.

- No memory leaks occur during active execution, failed command forks, or termination sequences.

## Compilation & Usage
### Compilation

To compile the shell, ensure readline is installed on your machine and run:
Bash

make

This generates the binary executable named minishell.
### Usage

Launch the shell natively by running:
Bash

./minishell

Example Commands:
Bash

minishell> echo "Hello from $USER"
minishell> cd .. && pwd
minishell> cat << EOF > output.txt

# Resources & Acknowledgements

This project was developed strictly for the mandatory requirements using the following official documentation, school materials, and development tools:

### 42 School Official Documents
* **Minishell Subject File:** The official guidelines and constraints provided by 42 School for the mandatory part. *(Note: This implementation does not include the bonus section).*
* **Bash Reference Manual:** Used to study and precisely match standard Bash behaviors for environment variables, redirections, and heredocs.

### Search & Research
* **Google Search:** Extensively used throughout development to research:
  * Official UNIX `man` pages for system calls like `fork`, `waitpid`, `pipe`, and `execve`.
  * Technical explanations for standard C library functions and pointer arithmetic.
  * Debugging strategies for standard compiler errors, file descriptors, and memory allocation issues.