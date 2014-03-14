CROSS_COMPILE =

lib-objs := pic_ctl.o libled.o

-include config.mk

all: libpic.a

libpic.a: $(call to-progs,$(lib-objs)) libled.a
	$(QUIET_AR)

libled.a:
	gcc -c libled.c
	ar -rcs libled.a

$(OBJS) : %.o : %.c
	$(QUIET_CC)

.PHONY: clean test utils

clean::
	@echo "  CLEAN   test"
	$(call QUIET_RM,*.o *.a *.so)
