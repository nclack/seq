/*
  This defines commonly used debugging macros and reporting functions.

  It's meant to be private to this source; this does not describe a public
  interface.  DO NOT include this in other headers.

  Requires these headers
    stdarg.h
    stdio.h
    stdlib.h
  are included before this file.
 */

#define LOG(...)           LOGGER.debug(__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
#define REPORT(msg1,msg2)  LOG("\t%s\n\t%s\n",msg1,msg2)
#define ERR(...)           LOGGER.error(__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
#define DBG(...)           LOGGER.debug(__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
#define WRN(...)           LOGGER.warn(__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
#define HERE               REPORT("Here","")
#define TRY(e)             do{if(!(e)){REPORT("Expression evaluated as false.",#e);goto Error;}}while(0)

static void err(const char *file,int line,const char *function,const char *format,...)
{ char buf[4096]={0};
  va_list ap;
  va_start(ap,format);
  vsnprintf(buf,sizeof(buf),format,ap);
  va_end(ap);
  fprintf(stderr,"%s(%d) - %s()\n%s\n",file,line,function,buf);
  return;
}
static void wrn(const char *file,int line,const char *function,const char *format,...)
{ char buf[4096]={0};
  va_list ap;
  va_start(ap,format);
  vsnprintf(buf,sizeof(buf),format,ap);
  va_end(ap);
  fprintf(stderr,"%s(%d) - %s()\n%s\n",file,line,function,buf);
  return;
}
static void dbg(const char *file,int line,const char *function,const char *format,...)
{ char buf[4096]={0};
  va_list ap;
  va_start(ap,format);
  vsnprintf(buf,sizeof(buf),format,ap);
  va_end(ap);
  fprintf(stdout,"%s(%d) - %s()\n%s\n",file,line,function,buf);
  return;
}

static struct _LOGGER_t
{ void (*error)(const char *file,int line,const char *function,const char *format,...);
  void (*warn)(const char *file,int line,const char *function,const char *format,...);
  void (*debug)(const char *file,int line,const char *function,const char *format,...);
} LOGGER={err,wrn,dbg};
