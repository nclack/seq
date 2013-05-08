#include "mex.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include "prelude.h"
#include "seq.h"

/// im=mexSeqRead(filename,idx)
/// Caller must enforce input types are correct.
void mexFunction(int nlhs,mxArray *plhs[],int nrhs,const mxArray *prhs[])
{ TRY(nlhs==1 && nrhs==2);
  char *filename=0;
  SeqReader *ctx=0;
  int32_t i;
  SET_REPORTERS;

  TRY(filename=(char*)mxArrayToString(prhs[0]));
  i=*(int32_t*)mxGetData(prhs[1]);
  i-=1; // Assume input is in Matlab's 1 based indexing.  Convert to 0 based indexing.
  TRY(ctx=Seq_Open(filename));
  TRY(0<=i && i<ctx->nframes);

  { mwSize dims[]={ctx->width,ctx->height}; // matlab has transposed storage order
    mxClassID typeid;
    switch(ctx->bitdepthreal)
    { case  8: typeid=mxUINT8_CLASS; break;
      case 16: typeid=mxUINT16_CLASS; break;
      case 32: typeid=mxUINT32_CLASS; break;
      case 64: typeid=mxUINT64_CLASS; break;
      default:
        REPORT("Error: Unexpected bit depth.","");
        goto Error;
    }
    TRY(plhs[0]=mxCreateNumericArray(2,dims,typeid,mxREAL));
  }
  TRY(Seq_Read_Image_To_Buffer(ctx,i,mxGetData(plhs[0])));
  return;
Error:
  mexErrMsgTxt("mexSeqRead call failed.\n");
  return;
}