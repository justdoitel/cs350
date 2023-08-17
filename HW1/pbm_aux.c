#include "pbm.h"
#include <stdlib.h>
#include<stdio.h>

//returns a pointer to a new ppmimage
PPMImage * new_ppmimage( unsigned int w, unsigned int h, unsigned int m )
{
    //storage for the struct itself and set its parameters
    PPMImage *toreturn = malloc(sizeof(*toreturn)); 
    toreturn->width=w;
    toreturn->height=h;
    toreturn->max=m;
    //allocate memory for each color
    toreturn->pixmap[0]=malloc(h*sizeof(int*));
    toreturn->pixmap[1]=malloc(h*sizeof(int*));
    toreturn->pixmap[2]=malloc(h*sizeof(int*));
    //allocate memory per row
    for (int i=0;i<h;i++){
    toreturn->pixmap[0][i] = malloc(w*sizeof(int));
    toreturn->pixmap[1][i] = malloc(w*sizeof(int));
    toreturn->pixmap[2][i] = malloc(w*sizeof(int));
    }
    return toreturn;
}

//returns a pointer to a new pbmimage
PBMImage * new_pbmimage( unsigned int w, unsigned int h )
{
    //storage for the struct itself and set its parameters
    PBMImage *toreturn = malloc(sizeof(*toreturn));
    toreturn->width=w;
    toreturn->height=h;
    //allocate memory for the 2d array
    toreturn->pixmap=malloc(h*sizeof(int*));
    for (int i=0;i<h;i++){
    toreturn->pixmap[i] = malloc(w*sizeof(int));
    }
    return toreturn;
}

//returns a pointer to a new pgmimage
PGMImage * new_pgmimage( unsigned int w, unsigned int h, unsigned int m )
{
    //storage for the struct itself and set its parameters
    PGMImage *toreturn = malloc(sizeof(*toreturn));
    toreturn->width=w;
    toreturn->height=h;
    toreturn->max=m;
    //allocate memory for the 2d array
    toreturn->pixmap=malloc(h*sizeof(int*));
    for (int i=0;i<h;i++){
    toreturn->pixmap[i] = malloc(w*sizeof(int));
    }
    return toreturn;
}

//deletes everything allocated for a ppmimage 
void del_ppmimage( PPMImage * p )
{
    //free the content of each row 
    for (int i=0;i<p->height;i++){
    free (p->pixmap[0][i]);
    free (p->pixmap[1][i]);
    free (p->pixmap[2][i]);
    }
    //free the columns
    free(p->pixmap[0]);
    free(p->pixmap[1]);
    free(p->pixmap[2]);
    free(p);
}

//deletes everything allocated for a pgmimage 
void del_pgmimage( PGMImage * p )
{
    //free the content of each row
    for (int i=0;i<p->height;i++){
    free(p->pixmap[i]);
    }
    //free the columns
    free(p->pixmap);
    free(p);
}

//deletes everything allocated for a pbmimage 
void del_pbmimage( PBMImage * p )
{
    //free the content of each row
    for (int i=0;i<p->height;i++){
    free(p->pixmap[i]);
    }
    //free the 2d array itself
    free(p->pixmap);
    free(p);
}

