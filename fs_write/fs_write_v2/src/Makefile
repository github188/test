vpath %.h ../headers
OBJECTS = main.o moniter.o parse_args.o list.o threads.o md5.o
LIBES = -lpthread
INCLUDE = -I ../headers
CC = gcc 
DBG = 
CCFLAGS = -Wall -g

fs_write : $(OBJECTS)
	$(CC) $(DBG) $(CCFLAGS) $(OBJECTS) $(LIBES) $(INCLUDE) -o fs_write

list.o: list.c ../headers/list.h ../headers/comm.h
	$(CC) $(DBG) $(CCFLAGS) -c $< $(INCLUDE) -o $@
main.o: main.c ../headers/comm.h ../headers/parse_args.h \
		 ../headers/comm.h ../headers/moniter.h ../headers/threads.h \
		  ../headers/main.h ../headers/list.h
	$(CC) $(DBG) $(CCFLAGS) -c $< $(INCLUDE) -o $@
md5.o: md5.c ../headers/byteorder.h ../headers/mdigest.h
	$(CC) $(DBG) $(CCFLAGS) -c $< $(INCLUDE) -o $@
moniter.o: moniter.c ../headers/parse_args.h ../headers/comm.h \
	 ../headers/comm.h ../headers/list.h ../headers/moniter.h \
	  ../headers/threads.h
	$(CC) $(DBG) $(CCFLAGS) -c $< $(INCLUDE) -o $@
parse_args.o: parse_args.c ../headers/comm.h ../headers/list.h \
		 ../headers/comm.h ../headers/parse_args.h
	$(CC) $(DBG) $(CCFLAGS) -c $< $(INCLUDE) -o $@
threads.o: threads.c ../headers/comm.h ../headers/mdigest.h \
		 ../headers/list.h ../headers/comm.h ../headers/moniter.h \
		  ../headers/parse_args.h ../headers/threads.h
	$(CC) $(DBG) $(CCFLAGS) -c $< $(INCLUDE) $(LIBES) -o $@

.PHONY : clean
clean :
	@echo "remove..."
	@rm  $(OBJECTS)

