BINS=marlo
OBJS=marlo_conf.o brick.o marlo.o player.o
INCS=marlo_conf.h brick.h player.h ../or.h
LDLIBS=../or.a ../../win/win.a ../../dlog/dlog.a ../../img/img.a ../../file/file.a ../../arc/arc.a ../../blaze/blaze.a ../../draw/draw.a ../../snd/snd.a -lSDL2 -lSDL2_mixer -lGL -lm 

all: $(BINS)

$(BINS): $(OBJS)
$(OBJS): $(INCS)

test: all
	./marlo

clean:
	rm -f $(BINS) $(OBJS)

