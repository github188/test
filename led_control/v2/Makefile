CC = gcc
MAKE = make
CFLAGS = -Wall -g
LDFLAGS = -lev
OBJECTS = led_shm.o led_worker.o  main.o

all: led-ctl-daemon libpic.a client

led-ctl-daemon:$(OBJECTS) libpic.a 
	$(CC) -o led-ctl-daemon $(OBJECTS) ./pic_ctl/libpic.a $(CFLAGS) $(LDFLAGS)
libpic.a:
	$(MAKE) -C pic_ctl
client:libpic.a
	$(MAKE) -C client

clean:
	rm -rf *.o led-ctl-daemon
	$(MAKE) -C pic_ctl clean
	$(MAKE) -C client clean
