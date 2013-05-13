#include "mex.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include "prelude.h"
#include "seq.h"

/// n=mexSeqLength(filename)
/// Returns the number of frames
void mexFunction(int nlhs,mxArray *plhs[],int nrhs,const mxArray *prhs[])
{ char *filename=0;
  SeqReader *ctx=0;
  mwSize dims[]={1,1};
  SET_REPORTERS;
  TRY(nlhs==1 && 1<=nrhs);
  TRY(filename=(char*)mxArrayToString(prhs[0]));
  TRY(ctx=Seq_Open(filename));
  TRY(plhs[0]=mxCreateNumericArray(1,dims,mxUINT32_CLASS,mxREAL));
  *(uint32_t*)mxGetData(plhs[0])=ctx->nframes;
  return;
Error:
  mexErrMsgTxt("mexSeqLength call failed.\n");
  return;
}