#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE 256
#define print_error(s,i) {perror(s);exit(i);}


ino_t get_inode(const char *path){
    struct stat buf;
    if( stat(path,&buf) != 0 ){
        print_error("stat failed",1);
    }

    return buf.st_ino;
}

void inum_to_name(ino_t ino, char its_name[], DIR *dir){

    struct dirent* direntp;

    int found = 0;
    while( (direntp = readdir(dir)) != NULL){
        if(direntp -> d_ino == ino){
            found = 1;
            break;
        }
    }
    if(!found){
        print_error("current inode not found in parent directory",1);
    }
    strncpy(its_name,direntp->d_name,MAX_SIZE);
    its_name[MAX_SIZE-1] = '\0';
    closedir(dir);
}

void printpathto(ino_t cur_ino){
    ino_t p_ino = get_inode("..");
    if(p_ino == cur_ino) {return;}

    DIR* dir;
    if( (dir = opendir("..")) == NULL){
        print_error("readdir failed",1);
    }

    char its_name[MAX_SIZE];
    inum_to_name(cur_ino,its_name,dir);
    chdir("..");
    printpathto(p_ino);
    printf("/%s",its_name);


}


int main(){
    ino_t cur_ino = get_inode(".");
    if(cur_ino == get_inode("/")){
        printf("/");
    }else{
        printpathto(cur_ino); 
    }
    printf("\n");
    return 0;
}
