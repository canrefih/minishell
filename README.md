# Minishell

A small Unix shell implemented in C as part of the 42 curriculum.

> Built by `omiskiny` and `recan`.

## Overview

Minishell recreates the core execution model of a Unix shell: command parsing, environment expansion, process creation, pipelines, redirections, heredocs, built-ins, and exit-status handling.

The project focuses on low-level systems programming and resource management rather than relying on an existing shell implementation.

## Engineering Focus

- **Lexing & parsing** — converts raw command input into structured tokens and command nodes.
- **Expansion** — handles environment variables, `$?`, quote-aware expansion, and quote removal.
- **Process management** — uses `fork()`, `execve()`, and `waitpid()` to execute external commands.
- **Pipelines** — connects processes with Unix pipes and manages their file descriptors.
- **Redirections** — supports `<`, `>`, and `>>` through descriptor duplication with `dup2()`.
- **Heredocs** — collects input until a delimiter and provides it to the command as standard input.
- **Built-ins** — implements `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.
- **Signals** — handles interactive shell signals without corrupting the command loop.
- **Resource management** — explicitly frees command structures, environment data, and execution resources.

## Architecture

The implementation is split into focused modules:

```text
input
  │
  ▼
lexer ──► parser ──► expander ──► executor
                                      │
                         ┌────────────┼────────────┐
                         ▼            ▼            ▼
                      builtin      pipeline    redirection
                                      │
                                      ▼
                                  child process
                                      │
                                   execve()
```

The shell keeps parsing, expansion, and execution separate so that each stage can transform the command representation before the next stage consumes it.

## Supported Features

### Commands

- External executables resolved through `PATH`
- Command pipelines (`cmd1 | cmd2 | cmd3`)
- Input/output redirection (`<`, `>`, `>>`)
- Heredocs (`<<`)
- Environment-variable expansion
- Exit-status expansion through `$?`
- Single and double quote handling

### Built-ins

| Built-in | Purpose |
| --- | --- |
| `echo` | Prints arguments, including repeated `-n` flags |
| `cd` | Changes the working directory and updates `PWD` / `OLDPWD` |
| `pwd` | Prints the current working directory |
| `export` | Creates or updates environment variables |
| `unset` | Removes environment variables |
| `env` | Prints exported environment entries with values |
| `exit` | Terminates the shell with the appropriate status |

## Error Handling

The shell validates common failure cases, including invalid identifiers, invalid `exit` arguments, missing commands, and execution failures. External commands that cannot be resolved through `PATH` return the conventional `127` status.

## Build

### Requirements

- Unix-like operating system
- C compiler
- GNU Readline development headers/library
- `make`

On Debian/Ubuntu:

```bash
sudo apt-get install libreadline-dev
```

### Compile

```bash
make
```

### Run

```bash
./minishell
```

### Clean

```bash
make clean
make fclean
```

### Sanitizers / Valgrind

The Makefile also provides development targets:

```bash
make sanitize
make val
```

## Example

```text
$ ./minishell
minishell> echo "Hello from $USER"
minishell> printf 'hello\nworld\n' | grep world
minishell> cat < input.txt > output.txt
minishell> cat << EOF
> hello from heredoc
> EOF
```

## Project Scope

This repository targets the **mandatory Minishell requirements**. The bonus section of the 42 subject is not included.

## Development

The repository uses a lightweight feature-branch workflow:

```text
main
  ▲
  │ PR + CI
  │
feature/* / fix/* / refactor/* / chore/*
```

Changes should be isolated in a focused branch and merged through a pull request after the build checks pass.

## References

- 42 Minishell subject and project documentation
- Bash Reference Manual
- Unix/Linux manual pages for `fork`, `execve`, `waitpid`, `pipe`, `dup2`, signals, and related system calls

## License

This project was created for educational purposes as part of the 42 curriculum. No separate open-source license is currently declared.
