BINS=win.a win.so 
OBJS=win.o
INCS=win.h

all: $(BINS)
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

win.a: $(OBJS)
	ar rs $@ $^

win.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(OBJS) $(BINS)
	+$(MAKE) -C tests/ clean

