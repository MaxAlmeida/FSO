FLAGS=

all:
	gcc -o progA main_a.c -w
	gcc -o progB main_b.c -w

compile:
	gcc -o progA main_a.c -w
	gcc -o progB main_b.c -w

exec: compile
	@echo $(FLAGS)
	$(shell tmux new-session -d)
	$(shell tmux split-window -d -t 0 -h)
	$(shell tmux send-keys -t 0 'clear' enter)
	$(shell tmux send-keys -t 0 './progA $(FLAGS)' enter)

	$(shell tmux send-keys -t 1 'sleep 1' enter)
	$(shell tmux send-keys -t 1 'clear' enter)
	$(shell tmux send-keys -t 1 './progB $(FLAGS)' enter)
	$(shell tmux attach)

clean:
	rm prog*
	ipcrm --all=msg
	ipcrm --shmem-key=5678
