#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<dirent.h>


void show_file_info(struct stat* statp, const char* filename){
    printf("mode: %o\t",statp->st_mode);
    printf("link: %lu\t",statp->st_nlink);
    printf("uid: %d\t",statp->st_uid);
    printf("gid: %d\t",statp->st_gid);
    printf("size: %ld\t",statp->st_size);
    printf("mtime: %ld\t",statp->st_mtime);
    printf("name: %s\n",filename);

}

int do_ls(const char* pathname){
    struct stat path_stat;

    if(stat(pathname, &path_stat)){
        perror("stat(pathname) failed\n");
    }

    if(S_ISREG(path_stat.st_mode)){
    //is regular file
        show_file_info(&path_stat, pathname);        

    }else if (S_ISDIR(path_stat.st_mode)){
    //is directory
        DIR *dirp;
        if(  (dirp = opendir(pathname)) == NULL)
            perror("opendir(pathname) failed\n");
        struct dirent *dire;
        while( (dire = readdir(dirp)) != NULL){
            show_file_info(&path_stat, dire->d_name);        
        }
    }

    return 0;
}


int main(int argc, char *argvs[]){
    if(argc == 1){
        const char *dirname = ".";
        do_ls(dirname);
    }else{
        while( --argc ){
            do_ls(*(++argvs));
        }
    }
    return 0;
}
