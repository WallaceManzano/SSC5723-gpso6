flags=-Wall --ansi -D_POSIX_C_SOURCE=200809L


compile:
	gcc $(flags) -c pipeline.c

test: 
	gcc $(flags) pipeline_test.c pipeline.c -o pipeline_test
	./pipeline_test

install:
	@mkdir -p $(PREFIX)/usr
	@mkdir -p $(PREFIX)/usr/lib
	@mkdir -p $(PREFIX)/usr/include
	gcc $(flags) -fpic -c pipeline.c
	ar rcs $(PREFIX)/usr/lib/libmylib.a pipeline.o
	gcc -fPIC -shared -o $(PREFIX)/usr/lib/libmylib.so pipeline.o 
	cp mylib.h $(PREFIX)/usr/include/mylib.h


clean:
	rm -f *.o
	rm -f $(PREFIX)/usr/lib/libmylib.a
	rm -f $(PREFIX)/usr/include/mylib.h
	rm -f $(PREFIX)/usr/lib/libmylib.so
	rm -f pipeline_test
