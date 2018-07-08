/* simple mkdir command, with -p option
 * using mkdir(), stat(),chdir(),
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>


#define PATH_LEN 128

static int opt_p;                           //option -p 

int dir_exists(char *path){
    struct stat info;
    if(stat(path,&info) == 0){
        if(S_ISDIR(info.st_mode)) return 1; // directory exists
        else return 2;                      // path is not a directory
    }
    else return 0;                          // path does not exist
}


void make_dir(char *path){
    char path2[PATH_LEN];
    strncpy(path2,path,PATH_LEN);
    path2[PATH_LEN-1]='\0';
    char* cur_dir; 
    cur_dir = strtok(path2,"/");
    int dir_stat; 

    if(opt_p){
        while(cur_dir){
            dir_stat = dir_exists(cur_dir);
            if(dir_stat == 2){
                fprintf(stderr, "mkdir: cannot create directory '%s': Not a directory\n",path);
                exit(1);
            }else if (dir_stat == 0){
                mkdir(cur_dir,0755);
            }
            chdir(cur_dir);
            cur_dir = strtok(NULL,"/");
        }
    }else{
        char *next_dir = strtok(NULL,"/");             
        dir_stat = dir_exists(cur_dir);
        while(cur_dir){
            if( next_dir && dir_stat!=1){
                if(dir_stat == 2){
                    fprintf(stderr, "mkdir: cannot create directory '%s': Not a directory\n",path);
                }else if(dir_stat == 0){
                    fprintf(stderr, "mkdir: cannot create directory '%s': No such directory\n",path);
                }
                exit(1);
            }
            if(!next_dir){
                if(dir_stat  >= 1){
                    fprintf(stderr, "mkdir: cannot create directory '%s': File exists\n",path);
                    exit(1);
                }
                mkdir(cur_dir,0755);
            }
            chdir(cur_dir);
            cur_dir = next_dir;
            next_dir = strtok(NULL,"/");
            dir_stat = dir_exists(cur_dir);
        }
    }
}

int main(int argc, char *argv[]){


    if(argc < 2){
        printf("usage: mkdir [-p] <path>...\n");
        exit(0);
    }
    opt_p = 0;

    char dirs[argc][PATH_LEN];
    int dirnum = 0;

    memset(dirs,0,sizeof(dirs));
    for(int i = 1; i < argc ; i++){
        if(argv[i][0]!='-'){
            strncpy(dirs[dirnum],argv[i],PATH_LEN);
            dirs[dirnum][PATH_LEN-1] = '\0';
            dirnum++;
        }else{
            if(argv[i][1] == 'p')
                opt_p = 1;
            else{
                fprintf(stderr,"invalid options %s\n",argv[i]);
                printf("usage: mkdir [-p] <path>...\n");
                exit(1);
            }
        }
    }
    
    char origin_dir[PATH_LEN];
    getcwd(origin_dir,PATH_LEN);
    for(int j = 0; j < dirnum; j++){
        chdir(origin_dir);
        make_dir(dirs[j]);
    }
    return 0;
}
