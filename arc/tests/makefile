all:
test: all clean
	rm -f test1.arc
	../tools/arc_import test1.arc dir1
	../tools/arc_report test1.arc
	rm -rf out/*
	../tools/arc_export test1.arc out
	diff -r out/dir1 dir1
	./out/dir1/subdir1/file3

clean:
	rm -rf test1.arc out/*
