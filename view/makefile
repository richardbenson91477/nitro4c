BINS=view.a view.so 
OBJS=view.o
INCS=view.h

all: $(BINS)
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

view.a: $(OBJS)
	ar rs $@ $^

view.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(OBJS) $(BINS)
	+$(MAKE) -C tests/ clean

