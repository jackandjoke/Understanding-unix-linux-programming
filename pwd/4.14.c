#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>


void create_dir(const char* path,mode_t mode){
    if( mkdir(path,mode) !=0){
        fprintf(stderr,"cannot create dir: %s\n",path);
        perror("");
        exit(1);
    }
}


int main(){
    mode_t m = 0775;
    create_dir("demodir",m);
    chdir("demodir");
    create_dir("a",m);
    create_dir("c",m);
    chdir("c");
    create_dir("d1",m);
    create_dir("d2",m);

    return 0;
}
