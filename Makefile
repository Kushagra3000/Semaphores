blocking: 
	gcc file1_2019056.c -o a1.out -lpthread && ./a1.out

nonblocking:
	gcc file2_2019056.c -o a2.out -lpthread && ./a2.out
