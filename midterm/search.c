#include <stdio.h>

main(int argc, char *argv[])
{
	FILE	*fp;
	char	arr[11];
	int	i = 0;
	char	ch;

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s filename number\n",argv[0]);
		exit(1);
	}
	
	if ((fp = fopen(argv[1], "rt")) == NULL)
	{
		perror("fopen");
		exit(1);
	}
	
	while ((ch = fgetc(fp)) != EOF)
	{
		if (ch != ' ')
		{
			arr[i] = ch;
			i++;
		}
	}

	int num = atoi(argv[2]);
	int start = 0;
	int end = 9;
	int mid;
	while (1)
	{
		mid = (start + end) / 2;
		if (start > end) break;
		if (num == mid)
		{
			printf("%d\n", num);
			fclose(fp);
			return(0);
		}
		else if (num > mid)
			start = mid + 1;
		else end = mid - 1;
	}
	fclose(fp);
	printf("None\n");
}
