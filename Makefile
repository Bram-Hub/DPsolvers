all: verbose silent

silent:
	gcc -o dpsilent gdpmain.c

verbose:
	gcc -o dpverbose gdpmain.c -DSADVAK_VERBOSE

clean:
	rm -f *~ dpsilent dpverbose