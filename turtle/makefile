BINS=turtle.a turtle.so 
OBJS=turtle.o
INCS=turtle.h

all: $(BINS)
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

turtle.a: $(OBJS)
	ar rs $@ $^

turtle.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(OBJS) $(BINS)
	+$(MAKE) -C tests/ clean

