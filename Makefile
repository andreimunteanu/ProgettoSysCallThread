# Sources:
SRCS:=main.c aux.c mem_init.c thread_init.c slave.c
OBJS:=$(SRCS:.c=.o)

# Config:
CC:=gcc
CFLAGS:= -c
LD:=gcc
LDFLAGS:=-lpthread

all: p_calc.x

clean:
  @echo Cleaning.
	@rm -f *.o
	@rm -f p_calc.x

p_calc.x: $(OBJS)
	@echo $@
	@$(LD) -o $@ $^ $(LDFLAGS)

aux.o: aux.c header.h
	@echo $@
	@$(CC) $(CFLAGS) -o $@ $<

mem_init.o: mem_init.c header.h
	@echo $@
	@$(CC) $(CFLAGS) -o $@ $<

thread_init.o: thread_init.c header.h
	@echo $@
	@$(CC) $(CFLAGS) -o $@ $<

slave.o: slave.c header.h
	@echo $@
	@$(CC) $(CFLAGS) -o $@ $<

main.o: main.c  header.h
	@echo $@
	@$(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean
