all: thread_manager simple_incr_shared simple_thread curtain_philosopher k_rooms_multithread

thread_manager : thread_manager.c
	gcc -o thread_manager thread_manager.c

simple_incr_shared : simple_incr_shared.c
	gcc -o simple_incr_shared simple_incr_shared.c

simple_thread : simple_thread.c
	gcc -o simple_thread simple_thread.c

curtain_philosopher : curtain_philosopher.c
	gcc -o curtain_philosopher curtain_philosopher.c

k_rooms_multithread : k_rooms_multithread.c
	gcc -o k_rooms_multithread k_rooms_multithread.c
clean:
	rm -f thread_manager
	rm -f simple_incr_shared
	rm -f simple_thread
	rm -f k_rooms_multithread
	rm -f curtain_philosopher