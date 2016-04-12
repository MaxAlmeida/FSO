all:
	@echo 'Usage: make <command>'
	@echo
	@echo 'Commands:'
	@echo ' compile	Generates the executables'
	@echo ' exec		Run both programs in two panes (tmux)'
	@echo ' clean		Clean the executables and msg/shmem'
	@echo
	@echo 'Flags:'
	@echo ' --debug	Prints all debug messages'
	@echo ' --tcp		Switch from SHM to TCP comunication'
	@echo

compile:
	gcc -o progA main_a.c -w -pthread
	gcc -o progB main_b.c -w -pthread

exec: compile
	$(shell tmux new-session -d)
	$(shell tmux split-window -d -t 0 -h)
	$(shell tmux send-keys -t 0 'clear' enter)
	$(shell tmux send-keys -t 0 './progA $(FLAG)' enter)

	$(shell tmux send-keys -t 1 'sleep 1' enter)
	$(shell tmux send-keys -t 1 'clear' enter)
	$(shell tmux send-keys -t 1 './progB $(FLAG)' enter)
	$(shell tmux attach)

clean:
	rm prog*
	ipcrm --all=msg
	ipcrm --shmem-key=5678
