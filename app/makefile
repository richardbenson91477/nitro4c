BINS=app.a app.so
OBJS=app.o
INCS=app.h

all: $(BINS)
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

app.a: $(OBJS)
	ar rs $@ $^

app.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS) $(OBJS)
	+$(MAKE) -C tests/ clean

