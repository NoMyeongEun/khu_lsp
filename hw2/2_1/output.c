#include <stdio.h>

main()
{
	char c = 'a', s[] = "hello";
	int i = 100; long l = 99999;
	float f = 3.14; double d = 99.999;
	int *p = &i;

	printf("Output: %c %s %d %#X %ld %.4f %.2lf %p\n", c, s, i, i, l, f, d, p);
	/*
	 * c : character , s : string, d :decimal  integer, X : hexadecimal as uppercase, # : display '0' before hexadecimal
	 * f : float, .4 : up to four decimal places, lf : double, p : pointer
	 */
	putchar(c);
	puts(s);
}
