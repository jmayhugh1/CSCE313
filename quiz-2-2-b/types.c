#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    //Write a C program that prints the type of file for each path provided as a command-line argument.
    if (argc <= 1)
    {
        printf("usage: path1 [path2 ...]\n");
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            printf("%s: ", argv[i]);
            struct stat buf;
            if (lstat(argv[i], &buf) < 0)
            {
                printf("path error\n");
                continue;
            }
            else
            {
                if(S_ISDIR(buf.st_mode)){
                    printf("directory\n");
                }
                else if(S_ISREG(buf.st_mode)){
                    printf("regular file\n");
                }
                else if(S_ISLNK(buf.st_mode)){
                    printf("symbolic link\n");
                }
                else if(S_ISCHR(buf.st_mode)){
                    printf("character special file\n");
                }
                else if(S_ISBLK(buf.st_mode)){
                    printf("block device\n");
                }
                else if(S_ISFIFO(buf.st_mode)){
                    printf("FIFO\n");
                }
                else if(S_ISSOCK(buf.st_mode)){
                    printf("socket\n");
                }
                else{
                    printf("other\n");
                }
            }

        }
    }
    printf("\n");


}