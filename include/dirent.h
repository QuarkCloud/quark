
#ifndef __QKC_DIRENT_H
#define __QKC_DIRENT_H 1

#include <quark_compile.h>

enum
{
    DT_UNKNOWN = 0,
# define DT_UNKNOWN	DT_UNKNOWN
    DT_FIFO = 1,
# define DT_FIFO	DT_FIFO
    DT_CHR = 2,
# define DT_CHR		DT_CHR
    DT_DIR = 4,
# define DT_DIR		DT_DIR
    DT_BLK = 6,
# define DT_BLK		DT_BLK
    DT_REG = 8,
# define DT_REG		DT_REG
    DT_LNK = 10,
# define DT_LNK		DT_LNK
    DT_SOCK = 12,
# define DT_SOCK	DT_SOCK
    DT_WHT = 14
# define DT_WHT		DT_WHT
};

struct dirent
{
    unsigned char d_type;
    char d_name[256];		
};

typedef struct __st_dirstream DIR ; //ÓÃ»§Í¸Ã÷

#ifdef __cplusplus
extern "C" {
#endif

QKCAPI DIR *opendir(const char * name);
QKCAPI int closedir(DIR * dir);
QKCAPI struct dirent *readdir(DIR * dir);
QKCAPI int readdir_r(DIR * dir , struct dirent *ent ,  struct dirent **result);

QKCAPI int scandir (const char * dir, struct dirent ***namelist, int (*selector) (const struct dirent *),
		    int (*cmp) (const struct dirent **,const struct dirent **) );


QKCAPI int scandir64 (const char * dir,struct dirent64 ***namelist, int (*selector) (const struct dirent64 *),
		      int (*cmp) (const struct dirent64 **,const struct dirent64 **));

#ifdef __cplusplus
}
#endif



#endif  /** __QKC_DIRENT_H */
