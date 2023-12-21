#include <stdio.h>

main()
{
	int		a = 0x12345678; /* integet - 4 byte */
	char	*p = (char *)&a; /* character pointer - 1 byte*/

/* Integer Byte Ordering - way to save integer  */
#if 1
	printf("Address %p: %#x\n", p, *p); p++; /* print the address as hexadecimal representation and increase the pointer value */
	printf("Address %p: %#x\n", p, *p); p++; /* then the pointer value points the next address -> the largest value at the smallest address -> little endian*/
	printf("Address %p: %#x\n", p, *p); p++;
	printf("Address %p: %#x\n", p, *p); p++;
#else
	printf("Address %p: %#x\n", p, *p++);
	printf("Address %p: %#x\n", p, *p++);
	printf("Address %p: %#x\n", p, *p++);
	printf("Address %p: %#x\n", p, *p++);
#endif
}