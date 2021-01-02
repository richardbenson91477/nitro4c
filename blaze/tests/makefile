BINS=blaze_1
LDLIBS=../blaze.a

ifndef CFLAGS
	CFLAGS=-O3 -march=native -fPIC -fomit-frame-pointer -funroll-loops
endif

all: $(BINS)

test: $(BINS)
	./blaze_1

clean:
	rm -f $(BINS)

