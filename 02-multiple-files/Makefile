CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: frumos clean all

all: main.c math_ops.c frumos
	$(CC) $(CFLAGS) main.c math_ops.c -o app

frumos:
	echo "sunt frumos"

clean:
	rm -f app