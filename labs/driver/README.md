## Requirements

* [tmux](https://tmux.github.io/): lets you switch between several
programs in one terminal.

## Program

```
Usage: make <command> FLAG="<flag>"

Commands:

 compile	    Generates the executables
 exec		   Run both programs in two panes (tmux)
 clean		  Clean the executables and msg/shmem

Flags:
 --debug		Prints all debug messages
 --tcp		  Switch from SHM to TCP comunication

```
## Examples

Execute the program with shared memory:
```
$ make exec
```

Execute the program with TCP:
```
$ make exec FLAG="=--tcp"
```

Execute the program with TCP and debug messages:
```
$ make exec FLAG="--tcp --debug"
```
