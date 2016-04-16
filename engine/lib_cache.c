//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 2016 by Gareth Nelson (gareth@garethnelson.com)
//
// This file is part of the Lambda engine.
//
// The Lambda engine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// The Lambda engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
//
// $Log:$
//
// DESCRIPTION:
//      Handles cache for library modules
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <limits.h>
#include <physfs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "vfs_io.h"

char cache_path[PATH_MAX];


void init_lib_cache() {
     const char* user_dir;
     struct stat cache_info;

     printf("lib_cache.c:init_lib_cache() - Starting cache system:\n");
     user_dir = PHYSFS_getUserDir();
     snprintf(cache_path,sizeof(cache_path), "%s.lambda/cache",user_dir);
     printf("lib_cache.c:init_lib_cache() - cache path is %s\n",cache_path);
     
     if(stat(cache_path, &cache_info) != 0) {
        mkdir((const char*)cache_path, 0700);
        printf("lib_cache.c:init_lib_cache() - Created new cache directory\n");
     }
}

void update_lib_cache(char *filename) {
     printf("lib_cache.c:update_lib_cache() - Updating cache from VFS for %s...",filename);
     char cache_filepath[PATH_MAX];
     char cache_md5path[PATH_MAX];
     char vfs_filepath[PATH_MAX];
     char vfs_md5path[PATH_MAX];

     snprintf(cache_filepath,sizeof(cache_filepath), "%s/%s", cache_path, filename);
     snprintf(cache_md5path,sizeof(cache_md5path), "%s/%s.md5", cache_path, filename);
     snprintf(vfs_filepath,sizeof(vfs_filepath),"/libs/%s",filename);
     snprintf(vfs_md5path,sizeof(vfs_md5path),"/libs/%s.md5",filename);
     
     vfs_extract(vfs_filepath,cache_filepath);
     vfs_extract(vfs_md5path,cache_md5path);

     printf("DONE!\n");
}

void refresh_lib_cache(char *filename) {
     char cache_filepath[PATH_MAX];
     char cache_md5path[PATH_MAX];
     char vfs_filepath[PATH_MAX];
     char vfs_md5path[PATH_MAX];
     
     char md5_vfs[32];
     char md5_cache[32];

     struct stat st;
     
     printf("lib_cache.c:refresh_lib_cache() - Refreshing cache for %s:\n",filename);
     snprintf(cache_filepath,sizeof(cache_filepath), "%s/%s", cache_path, filename);
     snprintf(cache_md5path,sizeof(cache_md5path), "%s/%s.md5", cache_path, filename);
     snprintf(vfs_filepath,sizeof(vfs_filepath),"/libs/%s",filename);
     snprintf(vfs_md5path,sizeof(vfs_md5path),"/libs/%s.md5",filename);
     
     if(stat(cache_filepath, &st) != 0) {
        printf("lib_cache.c:refresh_lib_cache() - File not found in cache, will update:\n");
        update_lib_cache(filename);
        return;
     }

     if(stat(cache_md5path, &st) == 0) {
        FILE *cache_md5_fd = fopen((const char*)cache_md5path,"r");
        fread((void*)md5_cache, sizeof(md5_cache),1,cache_md5_fd);
        fclose(cache_md5_fd);
     } else {
        printf("lib_cache.c:refresh_lib_cache() - Cache missing MD5, will update:\n");
        update_lib_cache(filename);
        return;
     }
     
     vfs_read((void*)md5_vfs,vfs_md5path,sizeof(md5_vfs));
     if(strncmp((const char*)md5_vfs,(const char*)md5_cache,sizeof(md5_vfs))==0) {
        printf("lib_cache.c:refresh_lib_cache() - Cache already up to date\n");
     } else {
        printf("lib_cache.c:refresh_lib_cache() - MD5 mismatch, will update:\n");
        update_lib_cache(filename);
        return;
     }
}
