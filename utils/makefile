NAME = buscabr
CC = g++
#CFLAGS = -O0 -g -Wall -Wextra -pedantic `pkg-config --cflags libxml-2.0`
CXXFLAGS = -O3 -march=native -flto -Wall -Wextra -Weffc++ -std=c++11 -pedantic -Wno-unused-parameter -Wno-ignored-qualifiers
ARG = michel almada de castro boaventura

.SILENT: run clean

$(NAME): $(NAME).cc $(NAME).h
	$(CC) $(CXXFLAGS) -o $(NAME) $(NAME).cc

run: $(NAME)
	./$(NAME) $(ARG)

val: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) $(ARG)

clean:
	rm -f *.o *~ $(NAME)
