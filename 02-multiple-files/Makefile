CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: frumos clean all

all: app

app: main.c math_ops.c
	$(CC) $(CFLAGS) main.c math_ops.c -o app

frumos:
	echo "sunt frumos"

clean:
	rm -f app