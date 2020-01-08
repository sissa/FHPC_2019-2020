

#include <stdlib.h>
#include <stdio.h> 



void write_pgm_image( void *image, int maxval, int xsize, int ysize, const char *image_name)
{
  FILE* image_file; 
  image_file = fopen(image_name, "w"); 
  
  // Writing header
  // The header's format is as follows, all in ASCII.
  // "whitespace" is either a blank or a TAB or a CF or a LF
  // - The Magic Number (see below the magic numbers)
  // - the image's width
  // - the height
  // - a white space
  // - the image's height
  // - a whitespace
  // - the maximum color value, which must be between 0 and 65535
  //
  // if he maximum color value is in the range [0-255], then
  // a pixel will be expressed by a single byte; if the maximum is
  // larger than 255, then 2 bytes will be needed for each pixel
  //

  int color_depth = 1+((maxval>>8)>0);       // 1 if maxval < 256, 2 otherwise

  fprintf(image_file, "P5\n%d %d\n%d\n", xsize, ysize, maxval);
  
  // Writing file
  fwrite( image, color_depth, xsize*ysize, image_file);  

  fclose(image_file); 
  return ;

  /* ---------------------------------------------------------------

     TYPE    MAGIC NUM     EXTENSION   COLOR RANGE
           ASCII  BINARY

     PBM   P1     P4       .pbm        [0-1]
     PGM   P2     P5       .pgm        [0-255]
     PPM   P3     P6       .ppm        [0-2^16[
  
  ------------------------------------------------------------------ */
}




#define XWIDTH 256
#define YWIDTH 256
#define MAXVAL 65535


#if ((0x100 & 0xf) == 0x0)
#define I_M_LITTLE_ENDIAN 1
#define swap(mem) (( (mem) & (short int)0xff00) >> 8) +	\
  ( (mem) & (short int)0x00ff << 8)
#else
#define I_M_LITTLE_ENDIAN 0
#define swap(mem) (mem)
#endif

int main( int argc, char **argv ) 
{ 
    char      *cImage;   // the image when a single byte is used for each pixel
    short int *sImage;   // the image when a two bytes are used for each pixel
    void      *ptr;
    
    int xsize      = XWIDTH;
    int ysize      = YWIDTH;
    int maxval     = MAXVAL;

    // print information about endianismo
    printf("this machine is %s\n", (I_M_LITTLE_ENDIAN)?"little endian":"big endian");

    // you can use also the system-defined macro LITTLE_ENDIAN
    printf("this machine is %s\n", (LITTLE_ENDIAN)?"little endian":"big endian");
    
    if ( argc > 1 )
      {
	maxval = atoi( *(argv+1) ) % 65536;
	if ( argc > 3 )
	  {
	    xsize = atoi( *(argv+2) );
	    ysize = atoi( *(argv+2) );
	  }
      }


    int minval      = 0; 
    int delta       = (maxval - minval) / ysize;
    
    if(delta < 1 )
      delta = 1;
    
    if( maxval < 256 )
      // generate a gradient with 1 byte of color depth
      {
	cImage = (char*)calloc( xsize*ysize, sizeof(char) );
	unsigned char _maxval = (char)maxval;
	int idx = 0;
	for ( int yy = 0; yy < ysize; yy++ )
	  {
	    unsigned char value = minval + yy*delta;
	    for( int xx = 0; xx < xsize; xx++ )
	      cImage[idx++] = (value > _maxval)?_maxval:value;
	  }
	ptr = (void*)cImage;
      }
    else
      // generate a gradient with 2 bytes of color depth
      {
	sImage = (short int*)calloc( xsize*ysize, sizeof(short int) );
	unsigned short int _maxval = swap((short int)maxval);
	int idx = 0;
	for ( int yy = 0; yy < ysize; yy++ )
	  {
	    short int value = (short int) (minval + yy*delta);
	    value = swap( value );    // swap high and low bytes, the format expect big-endianism

	    for( int xx = 0; xx < xsize; xx++ )
	      sImage[idx++] = (value > _maxval)?_maxval:value;
	  }
	ptr = (void*)sImage;	
      }

    
    write_pgm_image( ptr, maxval, xsize, ysize, "image.pgm" );
      
    free(ptr);

    return 0;
} 



