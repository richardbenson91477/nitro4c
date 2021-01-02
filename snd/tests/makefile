BINS=snd_1
LDLIBS=../snd.a ../../arc/arc.a ../../blaze/blaze.a ../../file/file.a ../../dlog/dlog.a ../../timer/timer.a -lSDL2 -lSDL2_mixer -lm

all: $(BINS)

test: all
	./snd_1 data/pluck.snd

clean:
	rm -f $(BINS)

