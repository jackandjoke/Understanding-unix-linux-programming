#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>


char* mode_to_letters(mode_t mode){
    static char letters[10];
    for(int i = 0; i < 10; i ++)
        letters[i] = '-';

    if(S_ISDIR(mode))
        letters[0] = 'd';

    if(S_IRUSR &mode)
        letters[1] = 'r';
    if(S_IWUSR &mode)
        letters[2] = 'w';
    if(S_IXUSR &mode)
        letters[3] = 'x';

    if(S_IRGRP &mode)
        letters[4] = 'r';
    if(S_IWGRP &mode)
        letters[5] = 'w';
    if(S_IXGRP &mode)
        letters[6] = 'x';

    if(S_IROTH &mode)
        letters[7] = 'r';
    if(S_IWOTH &mode)
        letters[8] = 'w';
    if(S_IXOTH &mode)
        letters[9] = 'x';

    return letters;

}

char* uid_to_name(uid_t uid){
    static char uid_str[255];
    struct passwd* pwd;
    if( (pwd = getpwuid(uid)) == NULL){
        // uid name not found
        sprintf(uid_str,"%d",uid);
        return uid_str;
    }else{
        return pwd->pw_name;
    }
}

char* gid_to_name(gid_t gid){
    static char gid_str[255];
    struct group* grp;
    if( (grp = getgrgid(gid)) == NULL){
        sprintf(gid_str, "%d",gid);
        return gid_str;
    }else{
        return grp->gr_name;
    }

}



void show_file_info(struct stat* statp, const char* filename){
    printf("%s\t", mode_to_letters(statp->st_mode));
    printf("%lu\t",statp->st_nlink);
    printf("%s\t",uid_to_name(statp->st_uid));
    printf("%s\t",gid_to_name(statp->st_gid));
    printf("%ld\t",statp->st_size);
    printf("%.12s\t",4 + ctime(&statp->st_mtime) );
    printf("%s\n",filename);

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
        struct stat file_stat;
        while( (dire = readdir(dirp)) != NULL){
            if(stat(dire->d_name, &file_stat)){
                perror("stat(dire->d_name) failed\n");
            }
            show_file_info(&file_stat, dire->d_name);        
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
