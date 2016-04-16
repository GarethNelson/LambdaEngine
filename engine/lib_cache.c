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
#include <physfs.h>
#include "uthash.h"

char cache_path[PATH_MAX];

struct md5_table_entry {
    char *filename;   // key
    char *md5_digest;
    UT_hash_handle hh;
};

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

void refresh_lib_cache(char *filename) {
     printf("lib_cache.c:refresh_lib_cache() - Refreshing cache for %s...",filename);
     printf("DONE!\n");
}
