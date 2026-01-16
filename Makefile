libscflags=`pkg-config --libs --cflags cairo gmp xcb-image`

compile:
	@gcc -g -Wall -O2 $(libscflags) -lm source.c -o platform
	@sudo chown root ./platform
	@sudo chmod +s ./platform
	@echo "ok"
