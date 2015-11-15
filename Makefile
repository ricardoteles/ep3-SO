cc = gcc
cflags = -Wall
dflags = -g

objs = ep3.o fs.o
exec = ep3 converte

all: ep3 converte

ep3: $(objs) -lreadline
	$(cc) -o $@ $^
	@echo "\nCompilou!"

converte: converte.o 

%.o: %.c $(headers)
	$(cc) -c $(dflags) $(cflags) $^

clean:
	-rm -f $(exec) $(objs) $(exec) *~core*