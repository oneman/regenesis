libscflags=`pkg-config --libs --cflags cairo-xcb cairo gmp libdrm`

compile:
	@gcc -g -Wall -O2 $(libscflags) -lm source.c -o pass
	@echo "ok"
