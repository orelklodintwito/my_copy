/*
 * File: copy_my.c
 * Description: Copies a source file to a destination file using Linux system calls only.
 * Author: Orel Twito
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    char answer = 0;     /* A char used to store the user’s response, for handling Section 4. */
    char buffer[4096];  
    ssize_t bytes_read; /* Holds the number of bytes read from the source file.*/ 

    if (argc != 3) /*Checks that exactly two arguments were passed– for handling Section 3.*/  
    {
        write(2, "Invalid number of arguments: expected 2.\n", sizeof("Invalid number of arguments: expected 2.\n") - 1);
        exit(1);
    }

    if (access(argv[1], F_OK) == -1)   /*Checks if the source file does not exist – for handling Section 3.*/ 
    {
        perror("source file access");
        exit(1);
    }

    if (access(argv[1], R_OK) == -1) /*Checks read permissions for the source file – for handling Section 3.*/ 
    {
        perror("access");
        exit(1);
    }

    if (access(argv[2], F_OK) == 0) /*Checks whether the destination file exists – for handling Section 4.*/ 
    {

        write(1, "The destination file already exists. Copying will delete the software. Do you want to continue? (n/y)\n", sizeof("The destination file already exists. Copying will delete the software. Do you want to continue? (n/y)\n") - 1);

        if (read(0, &answer, 1) == -1) /* Checks if the read operation was successful – for handling Section 5 */
        {
            perror("read");
            exit(1);
        }

        while (answer != 'n' && answer != 'N' &&   answer != 'y' && answer != 'Y') /* Validates user input; prompts again if the response is not 'y' or 'n' – for handling Section 4 */
        {
            write(1,"The destination file already exists. Copying will delete the software. Do you want to continue? (n/y)\n", sizeof("The destination file already exists. Copying will delete the software. Do you want to continue? (n/y)\n") - 1);
            if (read(0, &answer, 1) == -1) /* Checks if the read operation was successful – for handling Section 5 */
            {
                perror("read");
                exit(1);
            }
        }

        if (answer == 'n' || answer == 'N') /*Checks whether the user wants to cancel the copy operation – for handling Section 4.*/
        {
            write(1, "The copy was canceled at the user's request.\n", sizeof("The copy was canceled at the user's request.\n") - 1);
            exit(0);
        }
    }

    int src_fd = open(argv[1], O_RDONLY); /* src_fd represents the file descriptor of the opened source file – for handling Section 4. */
    if (src_fd == -1) /*Checks if opening the source file failed – for handling Section 3 */
    {
        perror("open source");
        exit(1);
    }

    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644); /* Opens or creates the destination file for writing – for handling Section 4 */
    if (dest_fd == -1) /* Checks if opening/creating the destination file failed – for handling Section 4 */
    {
        perror("open destination");
        close(src_fd); 
        exit(1);
    }

    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) /* Read loop */
    {
        if (write(dest_fd, buffer, bytes_read) == -1) /* Checks if writing to the destination file failed – for handling Section 5 */
        {
            perror("write");
            close(src_fd);
            close(dest_fd);
            exit(1);
        }
    }

    if (bytes_read == -1) /* Checks if a read error occurred – for handling Section 5 */
    {
        perror("read");
        close(src_fd);
        close(dest_fd);
        exit(1);
    }

    close(src_fd);
    close(dest_fd);

    return 0;
}

