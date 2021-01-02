BINS=or.a or.so
OBJS=or_app.o or_bg.o or_ent_type.o or_ent.o or_map.o or_menu.o or_tiles.o
INCS=or.h or_app.h or_bg.h or_ent_type.h or_ent.h or_map.h or_menu.h or_tiles.h

all: $(BINS)
	+$(MAKE) -C marlo/

$(OBJS): $(INCS)

or.a: $(OBJS)
	ar rs $@ $^

or.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C marlo/ test

clean:	
	rm -f $(OBJS) $(BINS)
	+$(MAKE) -C marlo/ clean

