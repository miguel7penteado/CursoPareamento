NAME = cpareia
CC = gcc
#CFLAGS = -O0 -g -Wall -Wextra -pedantic `pkg-config --cflags libxml-2.0`
CFLAGS = -O3 -march=native -Wall -Wextra -pedantic `pkg-config --cflags libxml-2.0`
LDFLAGS = `pkg-config --libs libxml-2.0` -lm -pthread -lz
SRC = .
DEPS = $(wildcard $(SRC)/*.h)
CODE = $(wildcard $(SRC)/*.c)
OBJ = $(patsubst %.c,%.o,$(CODE))
PROJ = input/project.xml

.PHONY: install clean dist-gzip dist-bzip2 dist-xz dist
.SILENT: install clean dist-gzip dist-bzip2 dist-xz dist

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(NAME)
	./$(NAME) -p $(PROJ)

val: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) -p $(PROJ)

clean:
	rm -f $(SRC)/*.o *~ $(SRC)/*~ $(NAME)
