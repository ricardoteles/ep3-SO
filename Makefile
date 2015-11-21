cc = gcc
cflags = -Wall
dflags = -g

headers = fs.h dados.h binario.h path.h
objs = ep3.o fs.o dados.o binario.o path.o
exec = ep3 converte

all: ep3 converte
	-rm -f *.h.gch $(objs)
	@echo "\nCompilou!"

ep3: $(objs) -lreadline
	$(cc) -o $@ $^

converte: converte.o
	$(cc) -o $@ $^

converte.o: converte.c
	$(cc) -c $^

%.o: %.c $(headers)
	$(cc) -c $(dflags) $(cflags) $^ 

clean:
	-rm -f $(exec) $(objs) *.h.gch *~core* /tmp/uni* converte.o