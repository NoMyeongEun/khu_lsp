#include <stdio.h>

int
strlen_p(char *str)
{
	int	len = 0; // initialize length to 0

	while (*str++)  { // until the character value equal to null character(= means the end of string)
		len++; // add 1 to the length if the current pointer value is not null character
	}
	return len;
}

int
strlen_a(char str[])
{
	int i;

	for (i = 0 ; str[i] != '\0' ; i++) // until the element of array eqauls to null chracter(=means the end of array), add 1 to i
		;
	return i;
}

void
strcpy_p(char *dst, char *src)
{
	while (*src)  { // until the src string ends
		*dst++ = *src++; // copy the value that the src pointer points to the value that the dst pointer points 
	}
	*dst = *src; // add null character at the end of dst string
}

void
strcpy_a(char dst[], char src[])
{
	int i;

	for (i = 0 ; src[i] != '\0' ; i++) // until the array src ends
		dst[i] = src[i]; // copy the i-th value of the src array to the i-th value of the dst array
	dst[i] = src[i]; // add null character at the dst array to represent the end of array
}

void
strcat_p(char *dst, char *src)
{
	while (*dst++) // until the dst string ends
		;
	dst--; // to concatenate from the null character
	while (*src)  { // until the src string ends
		*dst++ = *src++; // concentenate value by value (= the pointer points)
	}
	*dst = *src; // add null character at the end of dst string
}

void
strcat_a(char dst[], char src[])
{
	int i, j;

	for (i = 0 ; dst[i] != '\0' ; i++) // calculate the length of dst array
		;
	for (j = 0 ; src[j] != '\0' ; j++) // until the src array ends
		dst[i+j] = src[j]; // the index value = add j to the length of dst array(= i)
	dst[i + j] = src[j]; // add null character at the dst array to represent the end of array
}

main()
{
	int		len1, len2;
	char	str1[20], str2[20];

	len1 = strlen_p("Hello");
	len2 = strlen_a("Hello");
	printf("strlen: p=%d, a=%d\n", len1, len2);

	strcpy_p(str1, "Hello");
	strcpy_a(str2, "Hello");
	printf("strcpy: p=%s, a=%s\n", str1, str2);

	strcat_p(str1, ", World!");
	strcat_a(str2, ", World!");
	printf("strcat: p=%s, a=%s\n", str1, str2);
}
