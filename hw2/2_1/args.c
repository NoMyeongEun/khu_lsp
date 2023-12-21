#include <stdio.h>

main(int argc, char *argv[]) // when you want to use the values entered at the start of the program -> use argc and argv[]
{
	int		i;

	for (i = 0 ; i < argc ; i++) // argc = arguments count
		printf("argv[%d]=%s\n", i, argv[i]); // argv = argument varaible : each argument is stored in the array argv[] as type of string 
}
