
#ifndef __QKC_STDIO_H
#define __QKC_STDIO_H 1

#include <quark_compile.h>
#include <stdarg.h>
#include <sys/types.h>

__BEGIN_DECLS

#define _IOREAD         0x0001
#define _IOWRT          0x0002

#define _IOFBF          0x0000
#define _IOLBF          0x0040
#define _IONBF          0x0004

#define _IOMYBUF        0x0008
#define _IOEOF          0x0010
#define _IOERR          0x0020
#define _IOSTRG         0x0040
#define _IORW           0x0080

#define _IOAPPEND       0x0200

/* The possibilities for the third argument to `setvbuf'.  */
//#define _IOFBF 0 		/* Fully buffered.  */
//#define _IOLBF 1		/* Line buffered.  */
//#define _IONBF 2		/* No buffering.  */

#define BUFSIZ  512
#define EOF (-1)

/* The possibilities for the third argument to `fseek'.
   These values should not be changed.  */
#define SEEK_SET	0	/* Seek from beginning of file.  */
#define SEEK_CUR	1	/* Seek from current position.  */
#define SEEK_END	2	/* Seek from end of file.  */


typedef struct _iobuf FILE;

QKCAPI FILE *  __qkc_iob_func__(unsigned idx);

#define stdin  (__qkc_iob_func__(0))
#define stdout (__qkc_iob_func__(1))
#define stderr (__qkc_iob_func__(2))

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

#if _MSC_VER >= 1912

QKCAPI int sprintf(char * s, const char * format, ...);
QKCAPI int printf(const char * format, ...);
QKCAPI int vsnprintf(char * s, size_t maxlen, const char * format, va_list arg);

QKCAPI int fscanf(FILE * stream, const char * format, ...);
QKCAPI int sscanf(const char * s, const char * format, ...);
QKCAPI int fprintf(FILE * stream, const char * format, ...);

#else

QUARK_LINKAGE int sprintf (char * s , const char * format , ...);
QUARK_LINKAGE int printf(const char * format, ...);
QUARK_LINKAGE int vsnprintf(char * s, size_t maxlen, const char * format, va_list arg);
QUARK_LINKAGE int fscanf(FILE * stream, const char * format, ...);
QUARK_LINKAGE int sscanf(const char * s, const char * format, ...);
QUARK_LINKAGE int fprintf(FILE * stream, const char * format, ...);

#endif

QKCAPI int snprintf (char * s , size_t maxlen , const char * format , ...);

QUARK_LINKAGE int vfprintf (FILE * s , const char * format , va_list arg);
QUARK_LINKAGE int vprintf (const char * format , va_list arg);
QUARK_LINKAGE int vsprintf (char * s , const char * format , va_list arg);
//QKCAPI int snprintf (char * s , size_t maxlen , const char * format , ...);

QUARK_LINKAGE int scanf (const char * format , ...);

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

QUARK_LINKAGE void perror(const char * msg);

__END_DECLS

#endif /**__QKC_STDIO_H*/

