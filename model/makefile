BINS=model.a model.so
OBJS=model.o
INCS=model.c

all: $(BINS)
	+$(MAKE) -C tools/
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

model.a: $(OBJS)
	ar rs $@ $^

model.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS) $(OBJS)
	+$(MAKE) -C tools/ clean
	+$(MAKE) -C tests/ clean

