OBJECTS = main.o moniter.o parse_args.o fs_list.o threads.o md5.o
LIBES = -lpthread
CC = gcc 
DBG = 
CCFLAGS = -Wall -g

fs_write : $(OBJECTS)
	$(CC) $(DBG) $(CCFLAGS) $(OBJECTS) $(LIBES) -o fs_write

list.o: fs_list.c fs_list.h comm.h
	$(CC) $(DBG) $(CCFLAGS) -c  $< $(LIBES) -o $@
main.o: main.c comm.h parse_args.h moniter.h threads.h main.h fs_list.h
	$(CC) $(DBG) $(CCFLAGS) -c  $< $(LIBES) -o $@
md5.o: md5.c byteorder.h mdigest.h
	$(CC) $(DBG) $(CCFLAGS) -c  $< $(LIBES) -o $@
moniter.o: moniter.c parse_args.h comm.h fs_list.h moniter.h threads.h
	$(CC) $(DBG) $(CCFLAGS) -c  $< $(LIBES) -o $@
parse_args.o: parse_args.c comm.h fs_list.h
	$(CC) $(DBG) $(CCFLAGS) -c  $< $(LIBES) -o $@
threads.o: threads.c comm.h mdigest.h fs_list.h moniter.h parse_args.h \
	threads.h
	$(CC) $(DBG) $(CCFLAGS) -c  $< $(LIBES) -o $@

.PHONY : clean
clean :
	@echo "remove..."
	@rm  $(OBJECTS) fs_write
