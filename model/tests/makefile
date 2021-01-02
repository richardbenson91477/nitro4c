BINS=model_1
LDLIBS=../model.a ../../view/view.a ../../win/win.a ../../dlog/dlog.a ../../img/img.a ../../file/file.a ../../arc/arc.a ../../blaze/blaze.a ../../draw/draw.a -lSDL2 -lGL -lm
MODELS=test1.model
MODELAS=test1.modela
TEXS=default.rwh

.SUFFIXES: .ply .moda .model .png .rwh
.ply.moda:
	ply_to_moda $^ $@
.moda.model:
	moda_to_model $^ $@
.png.rwh:
	png_to_rwh $^ $@

all: $(BINS) $(MODELS) $(TEXS)

test: all
	./model_1

clean:
	rm -f $(BINS) $(MODELS) $(MODELAS) $(TEXS)

