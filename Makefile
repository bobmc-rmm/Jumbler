## Makefile

PGM	= jumble

All: $(PGM).c
	   gcc -Wall  -o $(PGM) $(PGM).c -I.

# copy all the words except 's or possessives
# grep -Fiv \' data2.txt > data3.txt