all:
	ipcrm --shmem-key=5678
	gcc main.c -w -pthread
	./a.out > out.txt
