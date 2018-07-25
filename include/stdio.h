
#ifndef __QKC_STDIO_H
#define __QKC_STDIO_H 1

#include <quark_compile.h>
#include <stdarg.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* The possibilities for the third argument to `setvbuf'.  */
#define _IOFBF 0 		/* Fully buffered.  */
#define _IOLBF 1		/* Line buffered.  */
#define _IONBF 2		/* No buffering.  */

#define BUFSIZ  512
#define EOF (-1)

/* The possibilities for the third argument to `fseek'.
   These values should not be changed.  */
#define SEEK_SET	0	/* Seek from beginning of file.  */
#define SEEK_CUR	1	/* Seek from current position.  */
#define SEEK_END	2	/* Seek from end of file.  */


struct _iobuf {
    char *_ptr;
    int   _cnt;
    char *_base;
    int   _flag;
    int   _file;
    int   _charbuf;
    int   _bufsiz;
    char *_tmpfname;
};
typedef struct _iobuf FILE;

QUARK_LINKAGE FILE * __iob_func(void);

#define stdin  (&__iob_func()[0])
#define stdout (&__iob_func()[1])
#define stderr (&__iob_func()[2])


QUARK_LINKAGE int remove (const char *filename);
QUARK_LINKAGE int rename (const char *old , const char * new_name);
QUARK_LINKAGE FILE *tmpfile (void);
QUARK_LINKAGE char *tmpnam (char *s);
QKCAPI char *tmpnam_r (char *s);

QUARK_LINKAGE int fclose (FILE *stream);
QKCAPI int fcloseall (void);
QUARK_LINKAGE int fflush (FILE *stream);
QUARK_LINKAGE FILE *fopen (const char * filename , const char * modes);
QUARK_LINKAGE FILE *freopen (const char * filename , const char * modes , FILE * stream);
QKCAPI FILE *fdopen (int fd , const char *modes);

QUARK_LINKAGE void setbuf (FILE * stream , char * buf);
QUARK_LINKAGE int setvbuf (FILE * stream , char * buf , int modes , size_t n);

QUARK_LINKAGE int fprintf (FILE * stream , const char * format , ...);
QUARK_LINKAGE int printf (const char * format , ...);
QUARK_LINKAGE int sprintf (char * s , const char * format , ...);

QKCAPI int snprintf (char * s , size_t maxlen , const char * format , ...);

QUARK_LINKAGE int vfprintf (FILE * s , const char * format , va_list arg);
QUARK_LINKAGE int vprintf (const char * format , va_list arg);
QUARK_LINKAGE int vsprintf (char * s , const char * format , va_list arg);
QKCAPI int snprintf (char * s , size_t maxlen , const char * format , ...);
QUARK_LINKAGE int vsnprintf (char * s , size_t maxlen , const char * format , va_list arg);

QUARK_LINKAGE int fscanf (FILE * stream , const char * format , ...);
QUARK_LINKAGE int scanf (const char * format , ...);
QUARK_LINKAGE int sscanf (const char * s , const char * format , ...);

QUARK_LINKAGE int fgetc (FILE *stream);
QUARK_LINKAGE int getc (FILE *stream);
QUARK_LINKAGE int getchar (void);
QUARK_LINKAGE int fputc (int c , FILE *stream);
QUARK_LINKAGE int putc (int c , FILE *stream);
QUARK_LINKAGE int putchar (int c);
QKCAPI int getw (FILE *stream);
QKCAPI int putw (int w , FILE *stream);
QUARK_LINKAGE char *fgets (char * s , int n , FILE * stream);
QUARK_LINKAGE char *gets (char *s);
QUARK_LINKAGE int fputs (const char * s , FILE * stream);
QUARK_LINKAGE int puts (const char *s);
QUARK_LINKAGE int ungetc (int c , FILE *stream);

QUARK_LINKAGE size_t fread (void * ptr , size_t size , size_t n , FILE * stream);
QUARK_LINKAGE size_t fwrite (const void * ptr , size_t size , size_t n , FILE * s);

QUARK_LINKAGE int fseek (FILE *stream , long int off , int whence);
QUARK_LINKAGE long int ftell (FILE *stream);
QUARK_LINKAGE void rewind (FILE *stream);
QKCAPI int fseeko (FILE *stream , off_t off , int whence);
QKCAPI off_t ftello (FILE *stream);
QUARK_LINKAGE int fgetpos (FILE * stream , fpos_t * pos);
QUARK_LINKAGE int fsetpos (FILE *stream , const fpos_t *pos);

QUARK_LINKAGE void clearerr (FILE *stream);
QUARK_LINKAGE int feof (FILE *stream);
QUARK_LINKAGE int ferror (FILE *stream);

QUARK_LINKAGE extern int _sys_nerr;
QUARK_LINKAGE extern const char *const _sys_errlist[];

QKCAPI int fileno (FILE *stream);
QKCAPI void flockfile (FILE *stream);
QKCAPI void funlockfile (FILE *stream);

#ifdef __cplusplus
}
#endif


#endif /**__QKC_STDIO_H*/

