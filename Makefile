all:six1
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
	mpirun -n 2 five
six1:
	mpicc six.c -o  six -DPINGPING
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
eleven1:
	mpicc eleven.c -o eleven
	mpirun -n 2 eleven
twelve1:
	mpicc twelve.c -o twelve
	mpirun -n 4 twelve
thirteen1:
	mpicc thirteen.c -o thirteen
	mpirun thirteen
fourteen1:
	mpicc fourteen.c -o fourteen
	mpirun fourteen