all : serv cli
serv : serv.c unp.h
	gcc serv.c -lpthread -o serv

cli : cli.c unp.h
	gcc cli.c -lpthread -o cli
clean:
	rm -f serv cli 
