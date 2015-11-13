cc = gcc
cflags = -Wall
dflags = -g

objs = ep3.o fs.o
exec = ep3

all: ep3

ep3: $(objs) -lreadline
	$(cc) -o $@ $^
	@echo "\nCompilou!"

%.o: %.c $(headers)
	$(cc) -c $(dflags) $(cflags) $^

clean:
	-rm -f $(exec) $(objs) $(exec) *~core*