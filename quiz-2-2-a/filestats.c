#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <grp.h>

int main(int argc, char *argv[])
{
    // todo
    //create a directory structure
    DIR *dp;
    struct dirent *dirp; // pointer to directory entry
    // check if directtory name is passed and then attempts to open it
    if (argc != 2)
    {
        printf("usage: filestats directory_name\n");
        return 1;
    }
    if ((dp = opendir(argv[1])) == NULL)
    {
        printf("can't open %s\n", argv[1]);
        return 1;
    }
    chdir(argv[1]); //changes the current working directory to the one passed as argument

    while ((dirp = readdir(dp)) != NULL){
        struct stat fileStat;
        if (stat(dirp->d_name, &fileStat) < 0)
        {
            printf("ERROR\n");
            continue;
        }
        else
        {
            // doing all the easu stuff

            printf("File name: %s\n", dirp->d_name);
             
             printf("Inode number: %ld\n", fileStat.st_ino);
            printf("Number of links: %ld\n", fileStat.st_nlink);
            printf("User ID of owner: %d\n", fileStat.st_uid);

            printf("Group ID of owner: %d\n", fileStat.st_gid);
            printf("Size in bytes: %ld\n", fileStat.st_size);

            printf("Last access: %s", ctime(&fileStat.st_atime));
               printf("Last modification: %s", ctime(&fileStat.st_mtime));
            printf("Last status change: %s", ctime(&fileStat.st_ctime));
            
            printf("Number of disk blocks allocated: %ld\n", fileStat.st_blocks);
            printf("Access mode in octal: %o\n", fileStat.st_mode);
            // getting access flags
      

            printf("Access mode flags: ");
                      (S_IRUSR & fileStat.st_mode) ? printf("r") : printf("-");
            (S_IWUSR & fileStat.st_mode) ? printf("w") : printf("-");
            (S_IXUSR & fileStat.st_mode) ? printf("x") : printf("-");
            (S_IRGRP & fileStat.st_mode) ? printf("r") : printf("-");
            (S_IWGRP & fileStat.st_mode) ? printf("w") : printf("-");
            (S_IXGRP & fileStat.st_mode) ? printf("x") : printf("-");
            (S_IROTH & fileStat.st_mode) ? printf("r") : printf("-");
            (S_IWOTH & fileStat.st_mode) ? printf("w") : printf("-");
            (S_IXOTH & fileStat.st_mode) ? printf("x") : printf("-");
        }
        printf("\n\n");

    }
    closedir(dp);
    exit(0);


//     File name
// inode number
// number of links
// User ID of owner
// Group ID of owner
// Size in bytes
// Last access
// Last modification
// Last status change
// Number of disk blocks allocated
// Access mode in octal
// Access mode flags

}