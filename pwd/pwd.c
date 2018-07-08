#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>

#define MAX_SIZE 256
#define print_error(s,i) {perror(s);exit(i);}


ino_t get_inode(const char *path){
    struct stat buf;
    if( stat(path,&buf) != 0 ){
        print_error("stat failed",1);
    }

    return buf.st_ino;
}

void solve(ino_t cur_ino){
    ino_t p_ino = get_inode("..");
    if(p_ino == cur_ino) {return;}

    DIR* dir;
    if( (dir = opendir("..")) == NULL){
        print_error("readdir failed",1);
    }

    struct dirent* cur_ent;
    int found = 0;
    while( (cur_ent = readdir(dir)) != NULL){
        if(cur_ent -> d_ino == cur_ino){
            found = 1;
            break;
        }
    }
    if(!found){
        print_error("current inode not found in parent directory",1);
    }
    chdir("..");
    solve(p_ino);
    printf("/%s",cur_ent->d_name);


}


int main(){
    ino_t cur_ino = get_inode(".");
    if(cur_ino == get_inode("/")){
        printf("/");
    }else{
        solve(cur_ino); 
    }
    printf("\n");
    return 0;
}
