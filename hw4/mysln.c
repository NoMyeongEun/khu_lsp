#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{       
        if (argc != 3)  {
                fprintf(stderr, "Usage: %s source destination\n", argv[0]);
                exit(1);
        }       
        
        if (symlink(argv[1], argv[2]) < 0)  { /* make a symbolic link for argv[1], whose filee name is argv[2] */
                perror("link");
                exit(1);
        }       
}    
