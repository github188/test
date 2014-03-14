OBJECTS := tools-test-led.o
LIBS := -L../pic_ctl -lpic
all: tools-test-led

tools-test-led:$(OBJECTS)
	gcc -o tools-test-led $(OBJECTS) $(LIBS)
clean:
	rm -rf *.o tools-test-led


