CFLAGS = -Wall -Werror

build: tema1.c tren.h
	gcc $(CFLAGS) -o tema1 tema1.c

clean:
	rm -f tema1