//This code is my own work, it was written without consulting a tutor or code written by other students - EL YIRDAW

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdint.h>
#include <sys/time.h>
#define MAPSIZE 1024

//copied from the inodemap (So i can free it at the end)
//changed the name to avoid conflict with the system (depending on how you compile it)
char ** Mapp=NULL;
void sit_inode( ino_t i, char * f ){
    uint32_t mappos = i % MAPSIZE;
    
    if( Mapp == NULL )
        Mapp = calloc(MAPSIZE, sizeof(char*));
    
    Mapp[mappos] = f;
}
char * git_inode( ino_t i ){
    if( Mapp == NULL )
        Mapp = calloc(MAPSIZE, sizeof(char*));
    
    return Mapp[ i % MAPSIZE ];
}

//creates tar recurseively
void createtarrecur (char* dir_path, FILE* tar_file){
    DIR* dir = opendir(dir_path);
    struct dirent* entry;
    while (entry=readdir(dir)) { //continues so long as there are more entries in the dir
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || entry->d_type==DT_LNK) {
            continue;
        }
        uint64_t inode = entry->d_ino;
        fwrite(&inode,8,1,tar_file);
        char* newdir = calloc(1,strlen(entry->d_name) +2+ strlen(dir_path));
        strcat(newdir,dir_path);
        strcat(newdir,"/");
        strcat(newdir,entry->d_name);
        uint32_t namelen = strlen(newdir); 
        fwrite(&namelen,4,1,tar_file);
        fwrite(newdir,1,namelen,tar_file);
        if(entry->d_type==DT_DIR){ //if it's a dir, copy info
            struct stat thestat;
            stat(newdir,&thestat);
            uint32_t mode= thestat.st_mode;
            fwrite(&mode,4,1,tar_file);
            uint64_t modification= thestat.st_mtime;
            fwrite(&modification,8,1,tar_file);
            createtarrecur(newdir,tar_file);
            free(newdir);
        } else if(entry->d_type==DT_REG){ //if it's a file, copy content too
            if(git_inode(inode)==NULL){
                struct stat thestat;
                stat(newdir,&thestat);
                uint32_t mode= thestat.st_mode;
                fwrite(&mode,4,1,tar_file);
                uint64_t modification= thestat.st_mtime;
                fwrite(&modification,8,1,tar_file);
                uint64_t size= thestat.st_size;
                fwrite(&size,8,1,tar_file);

                char buffer[100];
                int numr = 0;
                int numw = 0;
                FILE *toread = fopen(newdir, "rb");
            
                while(feof(toread)==0){	//content copied
                    numr=fread(buffer,1,100,toread);
                    numw=fwrite(buffer,1,numr,tar_file);
                }
                fclose(toread);
            }
            sit_inode(inode,newdir);
            free(newdir);
        }
    }
    closedir(dir);
}


void createtar(char* dir_path, FILE* tar_file){ //creates the inital tar and calls the recursive method
    DIR* dir = opendir(dir_path);
    struct stat thestat;
    int temp=stat(dir_path, &thestat);
    if(dir==NULL){
        if(temp!=0){ //no such file
            fprintf(stderr, "Specified target(\"%s\") does not exist.\n",dir_path);
        }
        else if(!S_ISDIR(thestat.st_mode)){ //not a dir
            fprintf(stderr, "Specified target(\"%s\") is not a directory.\n",dir_path);
        } else{
            perror("opendir");
        }
        exit(-1);
    }
    uint32_t magic=0x7261746D;
    fwrite(&magic,4,1,tar_file);

    uint64_t inode = thestat.st_ino;
    fwrite(&inode,8,1,tar_file);

    char * prevdir = calloc (1,strlen(dir_path)+4);
    strcat(prevdir,dir_path);
    strcat(prevdir,"/..");
    DIR* pdir = opendir(prevdir);
    if(pdir==NULL){
        perror("opendir");
        exit(-1);
    }
    struct dirent* entry;
    char* dname = NULL;
    while(entry=readdir(pdir)){
        if(entry->d_ino==inode){
            dname = entry->d_name;
        }
    }
    uint32_t namelen = strlen(dname);
    fwrite(&namelen,4,1,tar_file);
    fwrite(dname,1,namelen,tar_file); 
    uint32_t mode= thestat.st_mode;
    fwrite(&mode,4,1,tar_file);
    uint64_t modification= thestat.st_mtime;
    fwrite(&modification,8,1,tar_file);
    closedir(pdir);
    free(prevdir);
    closedir(dir);  
    createtarrecur(dir_path,tar_file);
}

void print(char* thetar){ //prints the tar as formatted in the pdf
    FILE *tar = fopen(thetar,"rb");
    if(tar==NULL){
       fprintf(stderr, "Specified target(\"%s\") does not exist.\n",thetar);
       exit(-1);
    }
    uint32_t themagic = 0;
    fread(&themagic,4,1,tar);
    
    if (themagic!=0x7261746D){
        fprintf(stderr, "Error: Bad magic number (%d), should be %d.\n",themagic,0x7261746D);
        exit(-1);
    }
    int nread;
   while(feof(tar)==0){ //loops the file until done
        uint64_t inode;
        nread=fread(&inode,8,1,tar);
        uint32_t namelen;
        nread+=fread(&namelen,4,1,tar);
        char *fname = calloc(1,namelen+1);
        nread+=fread(fname,namelen,1,tar);
        
        if(nread!=3){ 
            free(fname);
            if(feof(tar)){
                fclose(tar);
                return;
            }
            perror("fread");
            exit(-1);
        }

        if(git_inode(inode)!=NULL){
            printf("%s/ -- inode: %llu\n",fname,inode);
            free(fname);
            continue;
        } else{
            sit_inode(inode,fname);
        }
        uint32_t mode;
        uint64_t modification;
        nread=fread(&mode,4,1,tar);
        nread+=fread(&modification,8,1,tar);

        if(nread!=2){
            free(fname);
            perror("fread");
            fclose(tar);
            exit(-1);
        }

        if(S_ISDIR(mode)){
            printf("%s/ -- inode: %llu, mode %03o, mtime: %llu\n",fname,inode,mode%512,modification);
            free(fname);
            continue;
        }
        uint64_t size;
        
        if(fread(&size,8,1,tar)==0){
            free(fname);
            perror("fread");
            fclose(tar);
            exit(-1);
        }
        if ((mode&S_IXUSR)||(mode&S_IXGRP)||(mode&S_IXOTH)){
            printf("%s* -- inode: %llu, mode: %03o, mtime: %llu, size: %llu\n",fname,inode,mode%512,modification,size);
        } else {
            printf("%s -- inode: %llu, mode: %03o, mtime: %llu, size: %llu\n",fname,inode,mode%512,modification,size);
        }
        
        fseek(tar,size,SEEK_CUR); //skip over the content of a file since we won't need it
        free(fname);

    }
    fclose(tar);
}

int myrmdir (char* path){ //removes directory recursively
    DIR *dir = opendir(path);
    struct dirent *entry;
    if (!dir){
        return -1;
    }
    while (entry=readdir(dir)){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || entry->d_type==DT_LNK) {
            continue;
        }
        char* newdir = calloc(1,strlen(path)+2+strlen(entry->d_name));
        strcat(newdir,path);
        strcat(newdir,"/");
        strcat(newdir,entry->d_name);
        if(entry->d_type==DT_DIR){
            myrmdir(newdir);
        } else {
            if (remove(newdir)!=0){ //had to google this one
                return -1;
            }
        }
        free(newdir);
    }
    closedir(dir);
    if(rmdir(path)!=0) return -1;
}

void extract(char* thetar){ //extracts tar from a path
    FILE *tar = fopen(thetar,"rb");
    if(tar==NULL){
       fprintf(stderr, "Specified target(\"%s\") does not exist.\n",thetar);
       exit(-1);
    }
    
    uint32_t themagic = 0;
    fread(&themagic,4,1,tar);
    
    if (themagic!=0x7261746D){
        fprintf(stderr, "Error: Bad magic number (%d), should be %d.\n",themagic,0x7261746D);
        fclose(tar);
        exit(-1);
    }
    int nread=0;

    while(feof(tar)==0){ //goes through the file
        
        uint64_t inode;
        nread=fread(&inode,8,1,tar); 
        uint32_t namelen = 0;
        nread+=fread(&namelen,4,1,tar);
        char *fname = calloc(1,namelen+1);
        nread+=fread(fname,namelen,1,tar);
        if(nread!=3){
            free(fname);
            if(feof(tar)){ 
                fclose(tar);
                return;
            }
            fclose(tar);
            perror("fread");
            exit(-1);
        }
        
        if (git_inode(inode)!=NULL){
            if(link(git_inode(inode),fname)!=0){
                perror("link");
                free (fname);
                fclose(tar);
                exit(-1);
            }
            free (fname);
            continue;
        } else {
            sit_inode(inode,fname);
        }
        
        uint32_t mode = 0;
        uint64_t modification = 0;
        nread=fread(&mode,4,1,tar);
        nread+=fread(&modification,8,1,tar);
        
        if(nread!=2){
            free(fname);
            perror("fread");
            fclose(tar);
            exit(-1);
        }

        if (S_ISDIR(mode)){

            struct stat verytemp;
            if(stat(fname,&verytemp)==0){
                if(myrmdir(fname)!=0){ //if deleting the existing directory isn't possible
                    fprintf(stderr, "Error: there is another file with the same name (%s) as the extract.\n",fname);
                    fclose(tar);
                    exit (-1);
                }
            }
            errno = 0;

            if(mkdir(fname,mode%512)!=0){
                free(fname);
                perror("mkdir");
                fclose(tar);
                exit (-1);
            }
            if(chmod(fname,mode%512)!=0){
                free(fname);
                perror("chmod");
                fclose(tar);
                exit(-1);
            }
            
            struct timeval tv[2];
            gettimeofday(&tv[0], NULL);
            tv[1].tv_sec=modification;
            tv[1].tv_usec=0;
            utimes(fname,tv);
            
            continue;
        }            
            uint64_t size;
            if(fread(&size,8,1,tar)==0){
                free(fname);
                perror("fread");
                fclose(tar);
                exit(-1);
            }
            
            FILE *newfile = fopen(fname,"wb");
            if(newfile==NULL){
                perror("fopen");
                exit(-1);
            }

            char buffer [10];
            uint64_t thesize = size;
            
            
            while(thesize>0){
                int numbread = fread(buffer,1,1,tar);
                if(numbread!=1){
                    perror("fread");
                    fclose(tar);
                    fclose(newfile);
                    exit(-1);
                }
                fwrite(buffer,1,numbread,newfile);
                thesize--;
            }
            
            fclose(newfile);
            
            struct timeval times[2];
            gettimeofday(&times[0], NULL);
            times[1].tv_sec=modification;
            times[1].tv_usec=0;

            if(utimes(fname,times)!=0){
                perror("utimes");
                exit(-1);
            }
            
    }
    fclose(tar);
    
}

int main(int argc, char const *argv[])
{
    int opt;
    opterr=0; //so that error isn't printed but we can use opt&optarg ourselves 
    int option = -1; //type of option. if not -1, it's already been set
    char* thetar=""; //holds the name of the archive file
    while ((opt = getopt(argc, argv, "cxtf:" ))!= -1){ //get through our option
        switch(opt){ //we check the case and set the option appropriately, quitting if multiple modes are given
            case 'c':
                if(option!=-1){
                    fprintf(stderr, "Error: multiple modes specified.\n");
                    return -1;
                }
                option = 'c';
                break;
            case 'x':
                if(option!=-1){
                    fprintf(stderr, "Error: multiple modes specified.\n");
                    return -1;
                }
                option = 'x';
                break;
            case 't':
                if(option!=-1){
                    fprintf(stderr, "Error: multiple modes specified.\n");
                    return -1;
                }
                option = 't';
                break;
            case 'f': //this gets the argument
                thetar=optarg;
                break;
            case '?': //if there's an error, it checks what the error is and responds accordingly
                if(optopt=='f')
                    fprintf(stderr, "Error: No tarfile specified.\n");
                else
                    fprintf(stderr, "Error: Unrecognized option -%c.\n",optopt);
                    return -1;    
        }
    }

    if(strcmp(thetar,"")==0){ //tar not being set
        fprintf(stderr, "Error: No tarfile specified.\n");
        return -1;
    }

    if(option == -1){ //option not being set
        fprintf(stderr, "Error: No mode specified.\n");
        return -1;
    }

    if(option!='c') { //if the option is not c, we're done
        if(option=='x'){
            extract(thetar);
            for (int i=0; i<MAPSIZE;i++)
            free(git_inode(i));
            free(Mapp);
        }
        else if (option == 't'){
            print(thetar);
            free(Mapp);
        }
        
        return 0;
    }

    if(optind>argc-1){ //since we know it's c, we check for a target and reject if not
    fprintf(stderr, "Error: No directory target specified.\n");
    return -1;
    }

    char* dir = argv[optind]; //we get the directory
    FILE* tar_file = fopen(thetar,"wb");
    if(tar_file==NULL){
        perror("fopen");
        return -1;
    }

    createtar(dir,tar_file);
    fclose(tar_file);


    free(Mapp);
    
    return 0;
}
