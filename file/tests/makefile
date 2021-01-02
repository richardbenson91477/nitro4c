BINS=data_1
LDLIBS=../file.so ../../dlog/dlog.so

.SUFFIXES: .dat
.dat.h:
	../tools/data_to_h $^ $(basename $@ .dat) > $@

all: $(BINS)

data_1: data_1.h

test: all
	./data_1

clean:
	rm -f $(BINS) data_1.h

