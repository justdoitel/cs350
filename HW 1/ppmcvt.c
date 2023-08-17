#include "pbm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int intCeil(int a, int b) {
      if (a % b != 0) {
        return (a / b) + 1;
      } else {
        return (a / b);
      }
}

//type is first letter, extra is if there is an extra argument, input and output
void transformit(int type, char* extra, char* input, char* output){
    PPMImage* theimage = read_ppmfile(input);
    if(type=='r'){ //remove a certain color
        int k; //used to hold which color is asked to get removed
        if(strcmp("red",extra)==0) k=0;
        else if(strcmp("green",extra)==0) k=1;
        else k=2;
        for(int i=0;i<theimage->height;i++){ //loops through clearing the color by setting it to 0.
            for(int j=0;j<theimage->width;j++){
                theimage->pixmap[k][i][j]=0;
            }
        }
        //write the file then delete the image
        write_ppmfile(theimage,output);
        del_ppmimage(theimage);
    }
    if(type=='i'){ //isolate a certain color
        int k; //used to hold which color is asked to get removed
        if(strcmp("red",extra)==0) k=0;
        else if(strcmp("green",extra)==0) k=1;
        else k=2;
        for(int i=0;i<theimage->height;i++){ //loops through clearing other colors by setting them to 0.
            for(int j=0;j<theimage->width;j++){
                for(int a=0;a<3;a++){
                if(k!=a){
                theimage->pixmap[a][i][j]=0;
                }
                }
            }
        }
        //write the file then delete the image
        write_ppmfile(theimage,output);
        del_ppmimage(theimage);
    }
    if(type=='s'){
        //loops through the 2d array, changing the values according to the calculations
        for(int i=0;i<theimage->height;i++){
            for(int j=0;j<theimage->width;j++){  
                int oldR = theimage->pixmap[0][i][j];
                int oldG = theimage->pixmap[1][i][j];
                int oldB = theimage->pixmap[2][i][j];
                int a=0.393*oldR + 0.769*oldG+0.189*oldB;
                int b=0.349*oldR + 0.686*oldG+0.168*oldB;
                int c= 0.272*oldR + 0.534*oldG+0.131*oldB;
                if(a>theimage->max) a=theimage->max;
                if(b>theimage->max) b=theimage->max;
                if(c>theimage->max) c=theimage->max;
                theimage->pixmap[0][i][j] = a;
                theimage->pixmap[1][i][j] = b;
                theimage->pixmap[2][i][j] = c;
            }
        }
        //write the file then delete the image
        write_ppmfile(theimage,output);
        del_ppmimage(theimage);
    }
    if(type=='m'){
        for(int i=0;i<theimage->height;i++){ //loops through the 2d array, mirroring the numbers
            for(int j=0;j<theimage->width;j++){ 
                theimage->pixmap[0][i][theimage->width-j-1] = theimage->pixmap[0][i][j];
                theimage->pixmap[1][i][theimage->width-j-1] = theimage->pixmap[1][i][j];
                theimage->pixmap[2][i][theimage->width-j-1] = theimage->pixmap[2][i][j];
            }
        }
        //write the file then delete the image
        write_ppmfile(theimage,output); 
        del_ppmimage(theimage);
    }
    if(type=='t'){ //reduce it to the thumbnail size based on the scale given and save it in a smaller file
        int scale=atoi(extra);
        PPMImage * ppm = new_ppmimage(intCeil(theimage->width,scale), intCeil(theimage->height,scale), theimage->max); //using intceil to make the result ppm enough
        for(int i=0;i<ppm->height;i++){
            for(int j=0;j<ppm->width;j++){ 
                ppm->pixmap[0][i][j] = theimage->pixmap[0][i*scale][j*scale];
                ppm->pixmap[1][i][j] = theimage->pixmap[1][i*scale][j*scale];
                ppm->pixmap[2][i][j] = theimage->pixmap[2][i*scale][j*scale];
            }
        }
        //write the file then delete the images
        write_ppmfile(ppm,output); 
        del_ppmimage(ppm);
        del_ppmimage(theimage);
    }
     if(type=='n'){ //reduces to thumbnail (as above) first then overwrite the original file repeatedly with it to create tile effect
        //the thumbnail method copied from above
        int scale=atoi(extra);
        PPMImage * ppm = new_ppmimage(intCeil(theimage->width,scale), intCeil(theimage->height,scale), theimage->max);
        for(int i=0;i<ppm->height;i++){
            for(int j=0;j<ppm->width;j++){ 
                ppm->pixmap[0][i][j] = theimage->pixmap[0][i*scale][j*scale];
                ppm->pixmap[1][i][j] = theimage->pixmap[1][i*scale][j*scale];
                ppm->pixmap[2][i][j] = theimage->pixmap[2][i*scale][j*scale];
            }
        }

        int colscale=0; //we substract this from the scale when dividing because, after every loop, we deal with smaller problems 
        int coldone=0; //the rows we've already copied to. we subtract thus from height when dividing for the same reason as above
        while(1){ //this works on every column
            if(scale==colscale) break; //every column is done
            int y=intCeil(theimage->height-coldone,scale-colscale); //calculating how many rows we're copying to this round
            if(y==0) break; //if we don't need to copy, we're done
            for (int j=0;j<y;j++){ //loop through all the columns and copying the content
                int rowscale=0; //like colscale, this goes down as we progress to indicate we're dealing with smaller row
                int rowdone=0; //holds the row we've already copied to
                while(1){ //works like the outer loop
                    if(scale==rowscale) break;
                    int x=intCeil(theimage->width-rowdone,scale-rowscale);
                    if(x==0) break;
                    for(int i=0;i<x;i++){ //loops through the image and assign it the thumbnail accordingly
                        theimage->pixmap[0][j+coldone][i+rowdone] = ppm->pixmap[0][j][i];
                        theimage->pixmap[1][j+coldone][i+rowdone] = ppm->pixmap[1][j][i];
                        theimage->pixmap[2][j+coldone][i+rowdone] = ppm->pixmap[2][j][i];
                    }
                    rowdone=x+rowdone;
                    rowscale++;
                }
            }
            coldone=y+coldone;
            colscale++;
        }
        //write the file then delete the images
        write_ppmfile(theimage,output); 
        del_ppmimage(theimage);
        del_ppmimage(ppm);

    }
    if(type=='g'){ //convert into pgm using the max greyscale given
        int pgmmax=atoi(extra);
        PGMImage * pgm = new_pgmimage(theimage->width,theimage->height,pgmmax);
        for(int i=0;i<theimage->height;i++){ //goes through the 2d averaging the colors and scaling them using the pgmmax provided
            for(int j=0;j<theimage->width;j++){ 
            float avg = (theimage->pixmap[0][i][j]+theimage->pixmap[1][i][j]+theimage->pixmap[2][i][j])/3.0;
            pgm->pixmap[i][j]=((avg/theimage->max)*pgmmax);
            }
        }
        //write the file then delete the images
        write_pgmfile(pgm,output);
        del_ppmimage(theimage);
        del_pgmimage(pgm);
    }
    if(type=='b'){ //converts into pbm using the instructions
        PBMImage *toreturn = new_pbmimage(theimage->width,theimage->height);
        for(int i=0;i<theimage->height;i++){ //loops through the 2d array averaging the colors and converting them into bits based on the instruction.
            for(int j=0;j<theimage->width;j++){ 
            double avg = (theimage->pixmap[0][i][j]+theimage->pixmap[1][i][j]+theimage->pixmap[2][i][j])/3.0;
            toreturn->pixmap[i][j]=avg<theimage->max/2.0;
               
            }
        }
        //write the file then delete the images
        write_pbmfile(toreturn,output);
        del_ppmimage(theimage);
        del_pbmimage(toreturn);
    }
} 

int main (int argc, char *argv[]) {
    int opt;
    opterr=0; //so that error isn't printed but we can use opt&optarg ourselves 
    int transform = -1; //type of transformation is held here. if not -1, it's already been set
    char* output=""; //the output
    char* extra=""; //the extra (if necessary)
    while ((opt = getopt(argc, argv, "bg:i:r:smt:n:o:" ))!= -1){ //loops through grabbing the opts
        switch(opt){ //goes to the case inputted and acts accordingly. Error is checked case by case (no comment since it's straight forward)
            case 'b':
                if(transform!=-1){
                    printf("Error: multiple transformations specified\n");
                    return 1;
                }
                transform='b';
                break;
            case 'g':
                if(transform!=-1){
                    printf("Error: multiple transformations specified\n");
                    return 1;
                }
                transform='g';
                if(atoi(optarg)<1||atoi(optarg)>65535){
                    printf("Error: Invalid max grayscale pixel value: %s; must be less than 65,536\n",optarg);
                    return 1;
                }
                extra=optarg;
                break;
            case 'i':
                if(transform!=-1){
                    printf("Error: multiple transformations specified\n");
                    return 1;
                }
                transform='i';
                if( strcmp("red",optarg)!=0 & strcmp("green",optarg)!=0 & strcmp("blue",optarg)!=0 ){
                    printf("Error: Invalid channel specification: (%s); should be ‘red’, ‘green’, or ‘blue’\n", optarg);
                    return 1;
                }
                extra=optarg;
                break;
            case 'r':
                if(transform!=-1){
                    printf("Error: multiple transformations specified\n");
                    return 1;
                }
                transform='r';
                if( strcmp("red",optarg)!=0 & strcmp("green",optarg)!=0 & strcmp("blue",optarg)!=0 ){
                    printf("Error: Invalid channel specification: (%s); should be ‘red’, ‘green’, or ‘blue’\n", optarg);
                    return 1;
                }
                extra=optarg;
                break;
            case 's':
                if(transform!=-1){
                    printf("Error: multiple transformations specified\n");
                    return 1;
                }
                transform='s';
                break;
            case 'm':
                if(transform!=-1){
                    printf("Error: multiple transformations specified\n");
                    return 1;
                }
                transform='m';
                break;
            case 't':
                if(transform!=-1){
                    printf("Error: multiple transformations specified\n");
                    return 1;
                }
                transform='t';
                if(atoi(optarg)<1||atoi(optarg)>8){
                    printf("Error: Invalid scale factor: %s; must be 1-8\n",optarg);
                    return 1;
                }
                extra=optarg;
                break;
            case 'n':
                if(transform!=-1){
                    printf("Error: multiple transformations specified\n");
                    return 1;
                }
                transform='n';
                if(atoi(optarg)<1||atoi(optarg)>8){
                    printf("Error: Invalid scale factor: %s; must be 1-8\n",optarg);
                    return 1;
                }
                extra=optarg;
                break;
            case 'o':
                output=optarg;
                break;
            case '?':
                if(optopt=='g'|optopt=='i'|optopt=='r'|optopt=='t'|optopt=='n'|optopt=='o')
                    printf("Missing argument for option -%c\n",optopt);  
                else
                    printf("Usage: ppmcvt [-bgirsmtno] [FILE]\n");
                return 1;
        }
    }
    
    if(strcmp(output,"")==0){
        printf("Error: No output file specified\n");
        return 1;
    }

    if(optind>argc-1){
    printf("Error: No input file specified\n");
    return 1;
    }
    char* input = argv[optind];
    
    if(transform==-1){
        transform='b';
    }

    transformit(transform, extra, input, output); //once we've collected the data, we send it to a helper method
    return 0;
 
}
