OBJECTS =  main.o do_test.o parse_args.o list.o md5.o
LIBES = -lpthread
CC = gcc
fs_write : $(OBJECTS) comm.h
	$(CC) $(OBJECTS) $(LIBES) -o fs_write

$(OBJECTS) : comm.h
md5.o : byteorder.h mdigest.h

.PHONY : clean
clean :
	@echo "remove..."
	@rm  $(OBJECTS) fs_write
