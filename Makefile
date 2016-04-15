CONFIG   =  Config.sh
PYTHON   =  /usr/bin/python
CFLAGS   =
CC       = mpicc
INCL	 = include/allvars.h include/proto.h include/transfer_options.h
SRC 	 = src/main.c src/allvars.c src/init.c src/proc.c src/linalg.c src/thermodynamics.c \
	src/stats.c src/finalize.c

make:
	python make_macros.py
	$(CC) $(CFLAGS) $(SRC) -Wall -O3 -lm -fopenmp -o bin/transfer

clean:
	rm *~ src/*.o src/*~ include/*.o include/*~ bin/* \#* src/\#* include/\#* bin/\#* \
	python/*~ python/\#* OUTPUTS/*~ OUTPUTS/\#* OUTPUTS/*
