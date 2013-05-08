#include "seq.h"
/*
 * TESTS
 */
#ifdef SEQ_TESTING
int test1( char *path )
{ Image *im;
  SeqReader *f = Seq_Open( path );
  if(f)
  { im = Seq_Read_Image( f, 0 );
    Write_Image( "test_seq_read_0.tif", im );
    Free_Image(im);
    return 1;
  }
  return 0;
}

int test2( char *path )
{ Stack *s;
  SeqReader *f = Seq_Open( path );
  if(f)
  { s = Seq_Read_Stack( f );
    Write_Stack( "test_seq_stack_read.tif", s );
    Free_Stack( s );
    return 1;
  }
  return 0;
}

int main( int argc, char* argv[] )
{ 
  printf("Testing single image read: %d\n", test1(argv[1]));
  printf("Testing full stack read  : %d\n", test2(argv[1]));
  return 0;
}
#endif