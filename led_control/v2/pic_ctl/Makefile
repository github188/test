CROSS_COMPILE =

lib-objs := pic_ctl.o

-include config.mk

all: libpic.a 


libpic.a: $(call to-progs,$(lib-objs))
	$(QUIET_AR)


$(OBJS) : %.o : %.c
	$(QUIET_CC)

.PHONY: clean test utils

clean::
	@echo "  CLEAN   test"
	$(call QUIET_RM,*.o *.a *.so)
