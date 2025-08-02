NAME=		ottertype
SRC=		src/$(NAME).c

CC=			cc

build:
	$(CC) $(SRC) -o $(NAME) -Os

clean:
	rm -f $(NAME)

run:
	$(CC) $(SRC) -o $(NAME) -Os
	./$(NAME) 10
	rm -f $(NAME)
