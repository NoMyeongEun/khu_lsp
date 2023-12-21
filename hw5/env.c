#include <stdio.h>

main(int argc, char *argv[], char *envp[]) /* the current environment variables are stored in the third paramter of main function */
{
	int			i;
	char		**p;
	extern char	**environ; /* environ is defined as a global varaible in the Glibc source file posix/environ.c. you can use the global varaible of the other file, by using keyword extern. */

	printf("List command-line arguments\n");
	for (i = 0 ; i < argc ; i++)  { /* there are (argc - 1) command-line arguments */
		printf("%s\n", argv[i]);
	}

	printf("\n");
	printf("List environment variables from environ variable\n");
#if l
	for (i = 0 ; environ[i] != NULL ; i++) { /* list version. */
		printf("%s\n", environ[i]);
	}
#else
	for (p = environ ; *p != NULL ; p++) { /* pointer version. each environment variable stored as string */
		printf("%s\n", *p);
	}
#endif
	
	printf("\n");
	printf("List environment variables from envp variable\n");
#if l
        for (i = 0 ; envp[i] != NULL ; i++) { /* list version */
                printf("%s\n", envp[i]);
        }
#else
        for (p = envp ; *p != NULL ; p++) { /* pointer version.*/
                printf("%s\n", *p);
        }
#endif
}
