CC=gcc
CFLAGS= -lm -I.
DEPS = cfd_hs2017.h
OBJ = main.o get_parameters.o solver.o mkdir_p.o linear_solver.o write_data.o ovs.o  

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

run_sol: $(OBJ)
	gcc -o $@ $ $(CFLAGS)
