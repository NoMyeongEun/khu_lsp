#include <stdio.h>

int
strlen_p(char *str)
{
	int	len = 0;

	while (*str++)  {
		len++;
	}
	return len;
}

int
strlen_a(char str[])
{
	int i;

	for (i = 0 ; str[i] != '\0' ; i++)
		;
	return i;
}

void
strcpy_p(char *dst, char *src)
{
	while (*src)  {
		*dst++ = *src++;
	}
	*dst = *src;
}

void
strcpy_a(char dst[], char src[])
{
	int i;

	for (i = 0 ; src[i] != '\0' ; i++)
		dst[i] = src[i];
	dst[i] = src[i];
}

void
strcat_p(char *dst, char *src)
{
	while (*dst++)
		;
	dst--;
	while (*src)  {
		*dst++ = *src++;
	}
	*dst = *src;
}

void
strcat_a(char dst[], char src[])
{
	int i, j;

	for (i = 0 ; dst[i] != '\0' ; i++)
		;
	for (j = 0 ; src[j] != '\0' ; j++)
		dst[i+j] = src[j];
	dst[i+j] = src[j];
}

int strcmp_p(char *dst, char *src) 
{
	while (*dst || *src) { // until neither points to null ( = neither of the string ends)
		if (*dst != *src) // if the values are different even in the same location
			return (*dst - *src); // return the difference of the ASCII value
		dst++; // move pointer to point the next character of string
		src++;
	}
	return (0); // if while loop ends, that means all the values are the same
}

int strcmp_a(char dst[], char src[])
{
	int i;

	i = 0;
	while (dst[i] != '\0' || src[i] != '\0') { // until neither of array ends
		if (dst[i] != src[i]) // if the elements of the array are different even in the same index
			return (dst[i] - src[i]); // return the difference of the ASCII value
		i++; // move to the next element of the array
	}
	return (0); // if whiel loop ends, that means all the values are the same, then return 0
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

	char *str3 = "Hello";
	char *str4 = "Hello";
	printf("strcmp_a: dst=%s, src=%s, result=%d\n",str3,str4,strcmp_a(str3, str4));
	printf("strcmp_p: dst=%s, src=%s, result=%d\n",str3,str4,strcmp_p(str3, str4));

        char *str5 = "Hello";
        char *str6 = "World";
        printf("strcmp_a: dst=%s, src=%s, result=%d\n",str5,str6,strcmp_a(str5, str6));
        printf("strcmp_p: dst=%s, src=%s, result=%d\n",str5,str6,strcmp_p(str5, str6));

        char *str7 = "Hello";
        char *str8 = "Hell";
        printf("strcmp_a: dst=%s, src=%s, result=%d\n",str7,str8,strcmp_a(str7, str8));
        printf("strcmp_p: dst=%s, src=%s, result=%d\n",str7,str8,strcmp_p(str7, str8));
}
