#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <iostream>
using namespace std;

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGB;

typedef struct
{
    int nRows;
    int nCols;
    RGB *pixel;
} RGBpixmap;

unsigned short getShort(FILE *fp);
unsigned long getLong(FILE *fp);
void readBMPFile(RGBpixmap *pm, char *file)
{
    FILE *fp;
    int row,col,numPadBytes, nBytesInRow;
    unsigned long numCols; // number of columns in image
    unsigned long numRows; // number of rows in image
    unsigned short bitsPerPixel;    //8 or 24; allow 24 here
        long count;
    
    /* open the file */
    if ((fp = fopen(file,"rb")) == NULL)
    {
        printf("cannot open file %s.\n",file);
        exit(1);
    }
    
    /* check to see if it is a valid bitmap file */
    if (fgetc(fp)!='B' || fgetc(fp)!='M')
    {
        fclose(fp);
        printf("%s is not a bitmap file.\n",file);
        exit(1);
    }
    
    getLong(fp);
    getShort(fp);
    getShort(fp);
    getLong(fp);
    getLong(fp);
    numCols = getLong(fp); 
    numRows = getLong(fp);
    getShort(fp);      
    bitsPerPixel = getShort(fp);    //8 or 24; allow 24 here
    getLong(fp);      // must be 0 for uncompressed
    getLong(fp);       // total bytes in image
    getLong(fp);    // always 0
    getLong(fp);    // always 0
    getLong(fp);    // 256 for 8 bit, otherwise 0
    getLong(fp);       // always 0
    
    if(bitsPerPixel != 24)
    {
        printf("Error bitsperpixel not 24\n");
        exit(1);
    }
    //add bytes at end of each row so total # is a multiple of 4
    // round up 3*numCols to next mult. of 4
    nBytesInRow = (int) ((3 * numCols + 3)/4) * 4;
    numPadBytes = (int) (nBytesInRow - 3 * numCols); // need this many
    pm->nRows = (int) numRows; // set class's data members
    pm->nCols = (int) numCols;
    pm->pixel = (RGB *) malloc(3 * numRows * numCols);//make space for array
    if(!pm->pixel) return; // out of memory!
    count = 0;
    for(row = 0; row < numRows; row++) // read pixel values
    {
        for(col = 0; col < numCols; col++)
        {
            int r,g,b;
            b = fgetc(fp); g = fgetc(fp); r = fgetc(fp); //read bytes
            pm->pixel[count].r = r; //place them in colors
            pm->pixel[count].g = g;
            pm->pixel[count++].b = b;
        }
        for (int i=0; i<numPadBytes; i++)
            fgetc(fp);
    }
    fclose(fp);
}

void setTexture(RGBpixmap *p, GLuint textureID)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p->nCols, p->nRows, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, p->pixel);
    
}
unsigned long getLong(FILE *fp) //helper function
{
    unsigned long ip = 0;
    char ic = 0;
    unsigned char uc = ic;
    ic = fgetc(fp); uc = ic; ip = uc;
    ic = fgetc(fp); uc = ic; ip |=((unsigned long)uc << 8);
    ic = fgetc(fp); uc = ic; ip |=((unsigned long)uc << 16);
    ic = fgetc(fp); uc = ic; ip |=((unsigned long)uc << 24);
    return ip;
}
unsigned short getShort(FILE *fp) //helper function
{
    char ic;
    unsigned short ip;
    ic = fgetc(fp); ip = ic;  //first byte is little one
    ic = fgetc(fp);  ip |= ((unsigned short)ic << 8); // or in high order byte
    return ip;
}