ifndef ARCH
	ARCH=x86_64
endif

ifdef DEBUG
    CFLAGS=-Og -g -gdwarf-4 -fkeep-inline-functions -fPIC
else
    CFLAGS=-O3 -march=$(IDM_CARCH) -fomit-frame-pointer -funroll-loops -Wall -Wno-stringop-truncation -fPIC
endif

SUBDIRS=blaze dlog timer colors enums_map file arc img draw fb vgrab win snd view model plot spline comp turtle net or name_map sys map app n2hoops

all: $(SUBDIRS)
	for _SUBDIR in $(SUBDIRS); do cd $$_SUBDIR; $(MAKE); cd ..; done
	echo made all

test: $(SUBDIRS)
	for _SUBDIR in $(SUBDIRS); do cd $$_SUBDIR; $(MAKE) test; cd ..; done
	echo made test

clean:
	for _SUBDIR in $(SUBDIRS); do cd $$_SUBDIR; $(MAKE) clean; cd ..; done
	echo made clean

