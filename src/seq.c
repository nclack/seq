/* Author: Nathan Clack <clackn@janelia.hhmi.org>
 * Date  : May 2009 
 *
 * Copyright 2010 Howard Hughes Medical Institute.
 * All rights reserved.
 * Use is subject to Janelia Farm Research Campus Software Copyright 1.1
 * license terms (http://license.janelia.org/license/jfrc_copyright_1_1.html).
 */
/*
 * Reader for Streampix 3 (Norpix) movie files.  These files typically have a
 * .seq extension.
 *
 * Image data is read into the Image object used by Eugene Myers image
 * processing library.
 *
 * Author: Nathan Clack
 * Date  : September 2008
 * Notes : Time stamp functions are untested.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include "private/prelude.h"
#include "seq.h"

#undef    SEQ_TESTING
#define   SEQ_PARANOID

#ifdef SEQ_PARANOID
#define   SEQ_ASSERT(stmt)    if(stmt) goto Error
#else
#define   SEQ_ASSERT(stmt)    stmt
#endif

void Seq_Set_Logging(seq_logger_t err, seq_logger_t wrn, seq_logger_t dbg)
{ if(err) LOGGER.error=err;
  if(wrn) LOGGER.error=wrn;
  if(dbg) LOGGER.error=dbg;
}

SeqReader *Seq_Open( const char* path )
{ FILE *fp;
  SeqReader *h;
  TRY(h=(SeqReader*)malloc(sizeof(SeqReader)));
  fp = fopen( path, "rb" );
  if( !fp )
    goto ErrorIO;
  h->fp = fp;

  SEQ_ASSERT( fseek(fp, 548, SEEK_SET)                     );
  SEQ_ASSERT( fread( &(h->width         ), 4, 1, fp ) != 1 );
  SEQ_ASSERT( fread( &(h->height        ), 4, 1, fp ) != 1 );
  SEQ_ASSERT( fread( &(h->bitdepth      ), 4, 1, fp ) != 1 );
  SEQ_ASSERT( fread( &(h->bitdepthreal  ), 4, 1, fp ) != 1 );
  SEQ_ASSERT( fread( &(h->sizebytes     ), 4, 1, fp ) != 1 );
  SEQ_ASSERT( fread( &(h->fmt           ), 4, 1, fp ) != 1 );
  SEQ_ASSERT( fread( &(h->nframes       ), 4, 1, fp ) != 1 );
  SEQ_ASSERT( fseek(fp, 580, SEEK_SET)                     );
  SEQ_ASSERT( fread( &(h->truesize      ), 4, 1, fp ) != 1 );
  SEQ_ASSERT( fread( &(h->framerate     ), 8, 1, fp ) != 1 );

  h->starttime = Seq_Time_Stamp( h, 0 );
  return h;
Error:
  ERR( "Problem reading header of seq file\n" );
  if(h)  free(h);
  if(fp) fclose(fp);
  return NULL;

ErrorIO:
  ERR( "Could not open file at: %s\n",path );
  return NULL;
}

void Seq_Close( SeqReader *h )
{ if(h)
  { if(h->fp) 
      fclose(h->fp);
    free(h);
  }
}

int  Seq_Read_Images_To_Buffer ( SeqReader *r, int start, int stop, int step,  void *buffer )
{ size_t dz_buf, dz_seq;
  unsigned int i = 0, count=0;
  dz_seq = r->truesize;
  dz_buf = r->sizebytes;  

  //printf("From %d to %d by %d to %p.\n", start, stop, step, buffer);
  for( i=start; i < stop; i+=step )
  { //printf(" Read at %d and write to %d\n",step*i+start ,count);
    SEQ_ASSERT( fseek( r->fp, 1024+i*dz_seq       , SEEK_SET ) );
    SEQ_ASSERT( fread( (uint8_t*)buffer + (count++)*dz_buf,1, dz_buf, r->fp ) != dz_buf   );
  } 

  return 1;
Error:
  ERR( "Couldn't read stack.\n"
         "         Failed on %d.\n",i);     
  return 0;   
}

int  Seq_Read_Image_To_Buffer ( SeqReader *h, int index, void *buffer )
{ size_t offset = 1024 + index * h->truesize;
  SEQ_ASSERT( fseek( h->fp, offset, SEEK_SET)                            );
  SEQ_ASSERT( fread( buffer   , 1, h->sizebytes, h->fp ) != h->sizebytes ); 
  return 1;
Error:
  WRN("Seq reader: Couldn't read image at index %d\n",index);
  return 0;   
}



int  Seq_Read_Stack_To_Buffer ( SeqReader *r, void *buffer )
{ size_t dz_buf, dz_seq;
  unsigned int i = 0;
  dz_seq = r->truesize;
  dz_buf = r->sizebytes;  

  for( i=0; i < r->nframes; i++ )
  { SEQ_ASSERT( fseek( r->fp, 1024+i*dz_seq       , SEEK_SET ) );
    SEQ_ASSERT( fread( (uint8_t*)buffer +i*dz_buf,1, dz_buf, r->fp ) != dz_buf   );
  } 

  return 1;
Error:
  ERR( "Couldn't read stack.\n"
         "         Failed on %d.\n",i);  
  return 0;   
}

double Seq_Time_Stamp( SeqReader *h, int index )
{ size_t offset = 1024 + index * h->truesize + h->sizebytes;
  double t1,t2;

  SEQ_ASSERT( fseek( h->fp, offset, SEEK_SET)                     );
  SEQ_ASSERT( fread( &t1, 8, 1 , h->fp )                     != 1 );
  SEQ_ASSERT( fread( &t2, 8, 1 , h->fp )                     != 1 );
  return t1 + t2/1000.0;

Error:
  ERR("Seq reader: Error reading time stamp at index %d\n", index );
  return 0.0;
}

double Seq_Time_From_Start( SeqReader *h, int index )
{ return Seq_Time_Stamp( h, index ) - h->starttime;
}
