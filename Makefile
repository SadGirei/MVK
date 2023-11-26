all: nine1
one:
	mpicc first.c -o first
	mpirun first
two:
	mpicc second.c -o second
	mpirun -n 2 second 
three1:
	mpicc three.c -o three
	mpirun three
four1:
	mpicc four.c -o four -DSUM
	mpirun four
test:
	mpicc test.c -o test1
	mpirun -n 7 test1
five1:
	mpicc five.c -o five
	mpirun -n 3 five
six1:
	mpicc six.c -o  six -DPINGPONG
	mpirun -n 2  six 
seven1:
	mpicc seven.c -o seven
	mpirun  seven
eight1:
	mpicc eight.c -o eight
	mpirun eight
nine1:
	mpicc nine.c -o nine
	mpirun nine
ten1:
	mpicc ten.c -o ten
	mpirun ten
