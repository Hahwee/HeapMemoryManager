exe: lmm.o testapp.o
	gcc -g lmm.o testapp.o -o exe

lmm.o: lmm.c lmm.h
	gcc -g -c lmm.c -o lmm.o

testapp.o: testapp.c api_lmm.h
	gcc -g -c testapp.c -o testapp.o

clean:
	rm ./exe *.o
