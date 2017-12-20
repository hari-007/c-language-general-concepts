# C Language code for Operating System related concepts

## Getting Started

    Here we got few examples like Round Robin & FCFS implementation, Duplicate Shell, Splitting input file to subfiles . These are the concepts which are used to explain in Operating System course. Most of the Code is targeted for Unix environments [ because of posix API calls ]

### Compilation in GCC supported unix environment

```c
    gcc -Wall split.c -o split
        or
    gcc -Wall schedule.c -o schedule
        or
    gcc -Wall shell.c -o shell
```

### Steps to Run above code for Splitting input file to Sub Files functionality

```c
    ./split inputFileName.txt
```

For **Error Handling / Execution** in Split functionality code

```txt
hari@hari-007:~$ ./split
Too few arguments !!
Usage : split inputfile
hari@hari-007:~$ ./split foo bar
Too many arguments !!
Usage : split inputfile
hari@hari-007:~$ ./split nofoo
split: Couldn't open file nofoo: No such file or directory.
hari@hari-007:~$ ./split filewithnopermission
split: Couldn't open file filewithnopermission: Permission Denied.
hari@hari-007:~$ ./split foo
foo.0
foo.1
foo.2
foo.3
foo.4
hari@hari-007:~$ ls -l
total 9
-rw------- 1 hari-007 faculty   25252 Dec 10 10:53 foo
-rw------- 1 hari-007 faculty   10000 Dec 10 10:54 foo.0
-rw------- 1 hari-007 faculty   10000 Dec 10 10:54 foo.1
-rw------- 1 hari-007 faculty   10000 Dec 10 10:54 foo.2
-rw------- 1 hari-007 faculty   10000 Dec 10 10:54 foo.3
-rw------- 1 hari-007 faculty   10000 Dec 10 10:54 foo.4
-rwx--x--x 1 hari-007 faculty    5241 Dec 10 10:54 split
-rw---x--x 1 hari-007 faculty     480 Dec 10 10:54 split.c
-rw---x--x 1 hari-007 faculty    7680 Dec 10 10:54 split.c
```

### Steps to Run above code for Scheduling functionality from input burst time

For **Error Handling / Execution** in Scheduling functionality code

```txt
hari@hari-007:~$ gcc -Wal -o scheduler scheduler.c
hari@hari-007:~$ ./scheduler 0 data.txt
P0 P1 P2 P3
Average wait time: 135.500000
Average turnaround time: 198.00000
hari@hari-007:~$ ./scheduler 1 data.txt
P0 P1 P2 P3 P0 P1 P2 P3 P0 P1 P2 P0 P1 P2 P0 P1 P2 P0 P2 P0 P0 P0 P0 P0 P0 P0 P0
Average wait time: 101.750000
Average turnaround time: 164.250000
```

### Steps to Run above code for Duplicating Shell functionality

For **Error Handling / Execution** in Duplicate functionality code

* The run command will execute other programs specified by the users. For example:

>run ls will execute ls,  
>run ls -l will execute ls -l,  
>run date will execute date

* If user provides an unknown program, the shell should print out an error message.

```txt
myshell> run azzxcd
myshell: started child pid 4324
myshell: No such file or directory azzxcd
```

* The exit command will just exit  myshell.

* If user enters an empty command, do nothing and return to the prompt.

```txt
myshell>
myshell>
```

* If user enters commands other than run or exit, the shell should print out an error message:

```txt
myshell> start ls
myshell: start is not a valid command
```