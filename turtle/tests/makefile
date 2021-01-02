BINS=tsquig snowflake
LDLIBS=../turtle.a ../../win/win.a ../../img/img.a ../../arc/arc.a ../../file/file.a ../../dlog/dlog.a ../../blaze/blaze.a ../../draw/draw.a -lSDL2 -lGL -lm

all: $(BINS)

test: all
	./tsquig
	./snowflake

clean:
	rm -f $(BINS)

