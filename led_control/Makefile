CC = gcc
MAKE = make
CFLAGS = -Wall -g
LDFLAGS = -lev
OBJECTS = led_ev.o led_work.o led_worker.o list.o main.o

all: server libpic.a client
	$(MAKE) -C client
server:$(OBJECTS) libpic.a client
	$(CC) -o server $(OBJECTS) ./pic_ctl/libpic.a $(CFLAGS) $(LDFLAGS)
libpic.a:
	$(MAKE) -C pic_ctl


clean:
	rm -rf *.o server
	$(MAKE) -C pic_ctl clean
	$(MAKE) -C client clean
