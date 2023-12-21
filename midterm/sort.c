#include <stdio.h>

main(int argc, char *argv[])
{
	FILE	*fp1, *fp2;
	char	ch;
	char	arr[11];
	int i = 0;

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s source destination\n",argv[0]);
		exit(1);
	}

	if ((fp1 = fopen(argv[1], "rt")) == NULL)
	{
		perror("fopen");
		exit(1);
	}
	
	if ((fp2 = fopen(argv[2], "wt")) == NULL)
	{
		perror("fopen");
		exit(1);
	}

	while ((ch = fgetc(fp1)) != EOF)
	{
		if (ch != ' ')
		{	
			arr[i] = ch;
			i++;
		}
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = i; j < 10; j++)
		{
			if (arr[i] < arr[j])
			{
				char tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}	
	}

	for (int i = 0; i < 10; i++)
	{
		fputc(arr[i], fp2);
		if (i != 9) fputc(' ', fp2);
	}
	fputc('\n',fp2);

	fclose(fp1);
	fclose(fp2);	
}
