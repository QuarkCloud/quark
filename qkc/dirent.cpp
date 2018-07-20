
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct __st_dirstream
{
    HANDLE handle ;    
    struct dirent entrys[2] ;
    int index ;
    bool completed ;
} ;

bool find_data_to_entry(const WIN32_FIND_DATA * data , struct dirent * entry)
{
    if(data == NULL || entry == NULL)
        return false ;

    DWORD attr = data->dwFileAttributes ;
    if(attr == FILE_ATTRIBUTE_DIRECTORY)
        entry->d_type = DT_DIR;
    else if(attr == FILE_ATTRIBUTE_NORMAL)
        entry->d_type = DT_REG;
    else 
        entry->d_type = DT_UNKNOWN ;

    const char * fname = data->cFileName ;
    size_t fnsize = ::strlen(fname) ;
    if(fnsize >= sizeof(entry->d_name))
        fnsize = sizeof(entry->d_name) - 1;
    ::memcpy(entry->d_name , fname , fnsize) ;
    entry->d_name[fnsize] = '\0' ;
    return true ;
}

bool validate_dir(const DIR * dir)
{
    if(dir == NULL || dir->handle == NULL || dir->handle == INVALID_HANDLE_VALUE)
        return false ;
    else
        return true ;
}

DIR *opendir(const char * name)
{
    WIN32_FIND_DATA data ;
    HANDLE handle = FindFirstFile(name , &data) ;
    if(handle == INVALID_HANDLE_VALUE)
        return NULL ;

    DIR * dir = (DIR *)::malloc(sizeof(DIR)) ;
    if(dir == NULL)
        return NULL ;
    ::memset(dir , 0 , sizeof(DIR)) ;
    dir->handle = handle ;
    dir->completed = false ;
    find_data_to_entry(&data , dir->entrys) ;    
    return dir ;
}

int closedir(DIR * dir)
{
    if(validate_dir(dir) == false)
        return 0 ;

    ::CloseHandle(dir->handle) ;
    ::free(dir) ;
    return 0 ;
}


struct dirent *readdir(DIR * dir)
{
    if(validate_dir(dir) == false || dir->completed == true)
        return NULL ;

    int old_index = dir->index % 2 ;
    int new_index = (dir->index + 1) % 2 ;
    WIN32_FIND_DATA data ;
    if(::FindNextFile(dir->handle , &data) == TRUE)
    {
        find_data_to_entry(&data , dir->entrys + new_index) ;
        dir->index = new_index ;
    }
    else
    {
        dir->completed = true ;
    }

    return dir->entrys + old_index ;
}

int readdir_r(DIR * dir , struct dirent *ent ,  struct dirent **result)
{
    struct dirent * r = readdir(dir) ;
    if(r == NULL)
    {
        if(result != NULL)
            *result = NULL ;
        return 0 ;
    }

    if(ent != NULL)
    {
        ::memcpy(ent , r , sizeof(struct dirent)) ;
        if(result != NULL)
            *result = ent ;
    }
    return 0 ;
}

int scandir (const char * dir, struct dirent ***namelist, int (*selector) (const struct dirent *),
		    int (*cmp) (const struct dirent **,const struct dirent **) )
{
    return -1 ;
}


int scandir64 (const char * dir,struct dirent64 ***namelist, int (*selector) (const struct dirent64 *),
		      int (*cmp) (const struct dirent64 **,const struct dirent64 **))
{
    return -1 ;
}
