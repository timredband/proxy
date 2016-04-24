.c.o:
	gcc -g -c $? -lpthread 

all: http_proxy

http_proxy: utils.o http_proxy.o
	gcc -g -o http_proxy utils.o http_proxy.o -lpthread

clean:
	rm -f *.o http_proxy
