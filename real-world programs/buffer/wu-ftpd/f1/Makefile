
all:	mp-bad mp-ok create

clean:	
	rm -f mp-bad mp-ok create mp-bad.out mp-ok.out

mp-bad: mapped-path-bad.c
	$(CC) -o mp-bad mapped-path-bad.c

mp-ok: mapped-path-ok.c
	$(CC) -o mp-ok mapped-path-ok.c

create: make-long-path.c
	$(CC) -o create make-long-path.c ; \
	touch pathfile ; \
	./create
