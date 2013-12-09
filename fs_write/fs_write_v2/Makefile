OBJECTS =  main.o moniter.o parse_args.o list.o  threads.o md5.o
LIBES = -lpthread
CC = gcc
ccflags = -Wall
fs_write : $(OBJECTS) comm.h
	$(CC) $(ccflags) $(OBJECTS) $(LIBES) -o fs_write

$(OBJECTS) : comm.h
md5.o : byteorder.h mdigest.h

.PHONY : clean
clean :
	@echo "remove..."
	@rm  $(OBJECTS) fs_write
