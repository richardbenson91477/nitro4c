BINS=file.a file.so
OBJS=file.o
INCS=file.h

all: $(BINS)
	+$(MAKE) -C tools/
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

file.a: $(OBJS)
	ar rs $@ $^

file.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS) $(OBJS)
	+$(MAKE) -C tools/ clean
	+$(MAKE) -C tests/ clean

