BINS=n2hoops
LDLIBS=../app/app.a ../map/map.a ../sys/sys.a ../name_map/name_map.a ../comp/comp.a ../spline/spline.a ../model/model.a ../view/view.a ../win/win.a ../snd/snd.a ../img/img.a ../draw/draw.a ../arc/arc.a ../file/file.a ../timer/timer.a ../dlog/dlog.a ../blaze/blaze.a -lSDL2 -lSDL2_mixer -lGL -lm 

all: n2hoops_arc.h $(BINS)
 
n2hoops_arc.h: n2hoops.arc
	data_to_h $^ n2hoops_arc > $@

n2hoops.arc: data/
	arc_import $@ data

test: all
	./n2hoops

clean:
	rm -f $(BINS) n2hoops.arc n2hoops_arc.h

