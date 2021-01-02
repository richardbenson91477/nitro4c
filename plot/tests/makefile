BINS=gen_circles gen_sines
LDLIBS=-lm
DATS=circle.dat circle.txt sin.dat sin.txt

all: $(BINS)

test: all
	./gen_circles
	../plot circle.dat 64 2 1
	./gen_sines
	../plot sin.txt 32 1 0

clean:
	rm -f $(BINS) $(DATS)

