all:
	clang `pkg-config --cflags --libs ncurses` main.c
	mv a.out typecurses

clean:
	rm typecurses

