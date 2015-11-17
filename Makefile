cc = gcc
cflags = -Wall
dflags = -g

headers = fs.h superbloco.h bitmap.h fat.h dados.h
objs = ep3.o fs.o superbloco.o bitmap.o fat.o dados.o
exec = ep3 converte

all: ep3 converte
	-rm -f *.h.gch

ep3: $(objs) -lreadline
	$(cc) -o $@ $^
	@echo "\nCompilou!"

converte: converte.o 

%.o: %.c $(headers)
	$(cc) -c $(dflags) $(cflags) $^

clean:
	-rm -f $(exec) $(objs) $(exec) *~core* /tmp/unidade*