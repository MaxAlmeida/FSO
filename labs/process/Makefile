all:
	gcc -o progA main_a.c -w
	gcc -o progB main_b.c -w

compile:
	gcc -o progA main_a.c -w
	gcc -o progB main_b.c -w

exec:
	$(shell tmux new-session -d)
	$(shell tmux split-window -d -t 0 -h)
	$(shell tmux send-keys -t 0 'clear' enter)
	$(shell tmux send-keys -t 0 './progA' enter)
	$(shell tmux send-keys -t 0 'exit' enter)

	$(shell tmux send-keys -t 1 'clear' enter)
	$(shell tmux send-keys -t 1 './progB' enter)
	$(shell tmux send-keys -t 1 'exit' enter)
	$(shell tmux attach)

clean:
	rm prog*
