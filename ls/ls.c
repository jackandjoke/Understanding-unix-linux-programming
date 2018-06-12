#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>

#define MAX_FILE_NUM 256

char* mode_to_letters(mode_t mode){
    static char letters[10];
    //deal with rwx
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
    printf("%s", mode_to_letters(statp->st_mode));
    printf("%4lu ",statp->st_nlink);
    printf("%-8s ",uid_to_name(statp->st_uid));
    printf("%-8s ",gid_to_name(statp->st_gid));
    printf("%8ld ",statp->st_size);
    printf("%.12s ",4 + ctime(&statp->st_mtime) );
    printf("%s\n",filename);

}

void cat_full_filename(const char *pathname, const char *name, char fullname[]){
    int psize = sizeof(pathname) / sizeof(char);
    strcat(fullname, pathname);
    char slash[1] = "/";
    if(pathname[psize-1]!='/')
        strcat(fullname,slash);
    strcat(fullname, name);
}


int do_ls(const char* pathname){
    struct stat path_stat;

    if(stat(pathname, &path_stat)){
        fprintf(stderr,"ls: Cannot access '%s': ",pathname); 
        perror("");
        exit(1);
    }

    if(S_ISREG(path_stat.st_mode)){
    //is regular file
        show_file_info(&path_stat, pathname);        

    }else if (S_ISDIR(path_stat.st_mode)){
    //is directory
        DIR *dirp;
        if(  (dirp = opendir(pathname)) == NULL){
            fprintf(stderr, "ls: Cannot open directory '%s': ",pathname);
            perror("");
            exit(1);
        }
        struct dirent *dire;
        const char* filenames[MAX_FILE_NUM];
        int idx = 0;
        while( (dire = readdir(dirp)) != NULL){

            if(idx == MAX_FILE_NUM - 1){
                fprintf(stderr, "two many files in '%s'",pathname);
                perror("");
                exit(1);
            }
            filenames[idx++] = dire->d_name;
        }
        int mycmp(const void *, const void*);
        qsort(filenames,idx,sizeof(const char*),mycmp);
        int i;
        struct stat file_stat;
        for(i = 0; i < idx; i ++){
            char full_filename[255] = "";
            cat_full_filename(pathname,filenames[i],full_filename);
            if( stat(full_filename, &file_stat)==-1 ){
                perror("stat(dire->d_name) failed\n");
                printf("\t%s\n",full_filename);
            }else{
                show_file_info(&file_stat, filenames[i]);
            }
        }
        closedir(dirp);
    }

    return 0;
}
int mycmp(const void *c1, const void *c2){
    return  strcmp( *(const char**) c1, *(const char**) c2 );
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
