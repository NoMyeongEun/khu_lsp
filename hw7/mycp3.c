#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fdin, fdout; // File descriptors for input and output files
    char *src, *dst; // Pointers for memory mapping source and destination
    struct stat statbuf; // Structure to hold file status information

    if (argc != 3)  {
        fprintf(stderr, "Usage: %s source destination\n", argv[0]);
        exit(1);
    }

    // Open the source file in read-only mode
    if ((fdin = open(argv[1], O_RDONLY)) < 0)  {
        perror("open");
        exit(1);
    }

    // Create or truncate the destination file with read-write permissions
    if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0)  {
        perror("open");
        exit(1);
    }

    // Retrieve information about the input file
    if (fstat(fdin, &statbuf) < 0)  {
        perror("fstat");
        exit(1);
    }

    // Set the output file's size to match the input file's size
    if (lseek(fdout, statbuf.st_size-1, SEEK_SET) < 0)  {
        perror("lseek");
        exit(1);
    }
    write(fdout, "", 1); // Write an empty byte to the end of the file

    // Map the input file into memory for reading
    if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == (caddr_t) -1)  {
        perror("mmap");
        exit(1);
    }

    // Map the output file into memory for writing
    if ((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) == (caddr_t) -1)  {
        perror("mmap");
        exit(1);
    }

    // Copy the contents from the mapped input memory to the mapped output memory
    memcpy(dst, src, statbuf.st_size);

    // Close the input and output files
    close(fdin);
    close(fdout);
    return 0;
}

