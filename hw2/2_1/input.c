#include <stdio.h>

main()
{
	char c, s[80];
	int i; long l;
	float f; double d;

	scanf("%c %s %d %ld %f %lf", &c, s, &i, &l, &f, &d); // standard input : scanf -> use system call 'read', no reference type in C -> use pointer value 
	printf("Output: %c %s %d %ld %.4f %.2lf\n", c, s, i, l, f, d);  // standard output : printf -> use system call 'write'

	c = getchar(); // one character input -> use system call 'read'
	putchar(c); // one character output -> use system call 'write'

	gets(s); // string input -> use system call 'read'
	puts(s); // string output -> use system call 'write'
}
