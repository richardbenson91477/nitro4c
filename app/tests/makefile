BINS=app_1
LDLIBS=../app.a ../../map/map.a ../../sys/sys.a ../../name_map/name_map.a ../../comp/comp.a ../../spline/spline.a ../../model/model.a ../../view/view.a ../../win/win.a ../../snd/snd.a ../../img/img.a ../../draw/draw.a ../../arc/arc.a ../../file/file.a ../../timer/timer.a ../../dlog/dlog.a ../../blaze/blaze.a -lSDL2 -lSDL2_mixer -lGL -lm 

all: $(BINS)
 
test: all
	./app_1

clean:
	rm -f $(BINS)

