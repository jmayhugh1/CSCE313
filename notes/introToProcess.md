# Introduction to Processes

## What is a process?

A process is an instance of a program running on a computer. Everytime a user runs a program, the shell creates a new process and then runs the executable file in that process. A process is a container for a set of resources used when executing a program. These resources include:
- Process ID
- Address Space (code, data, heap, stack)
- Processor state (registers)
- etc.

```
$ ps 
    PID TTY           TIME CMD
    501 ttys000    0:00.04 -bash
    502 ttys000    0:00.00 ps
    ```

```
PID returns the unique process ID
TTY returns the terminal type
TIME returns the amount of CPU time used
CMD returns the command that started the process

```
ps aux

```
ps: status process command
a: displays information about all users
u: displays the process's belonging to specified usernames
x: displays the process's not attached to a terminal

A process provides each program with two key abstractions:
- Logical Control Flow
- Private Memory Space
how are these illusions maintained?

- Process executions interleaved (multitasking)
- main memory addressn spaces managed by OS (virtual memory)

a context sw
## Multiprocessing

## Context Switching

## Process Creation
