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
//      Tools for handling dynamic loading
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <physfs.h>
#include <dlfcn.h>
#include "lib_tools.h"
#include "lib_cache.h"
#include "utlist.h"

lib_details *libs_list = (lib_details*)NULL;

void load_lib(char *filename) {
     void (*init_module)();
     char lib_full_path[PATH_MAX];
     get_cache_path(filename, lib_full_path);
     printf("lib_tools.c:load_lib() - Full path to load is %s, loading:\n",lib_full_path);
     void* handle=dlopen(lib_full_path,RTLD_NOW|RTLD_GLOBAL);
     if(handle==NULL) {
        printf("lib_tools.c:load_lib() - dlopen() error: %s\n",dlerror());
     }
#ifdef __linux
     init_module = dlsym(handle,"init_module");
     init_module();
#endif   
}

void init_libs() {
    char **lib_physfs_list;
    char **i;
    lib_details *d, *tmp;

    printf("lib_tools.c:init_libs() - Locate libraries:\n");
    lib_physfs_list = PHYSFS_enumerateFiles("libs");
    for(i=lib_physfs_list; *i != NULL; i++) {
        printf("%s\n",*i);
#ifdef __APPLE__
        if(strlen(*i) > 6 && !strcmp(*i + strlen(*i) - 6, ".dylib")) {
#else
        if(strlen(*i) > 3 && !strcmp(*i + strlen(*i) - 3, ".so")) {
#endif
           // I know this isn't portable, but fuck it
           d = malloc(sizeof(lib_details));
           strncpy(d->filename,*i,MAX_LIBFN_LEN);
           d->loaded = 0;
           LL_APPEND(libs_list,d);
           printf("lib_tools.c:init_libs() - Located %s\n", *i);
        }
    }
    PHYSFS_freeList(lib_physfs_list);

    printf("lib_tools.c:init_libs() - Start cache system:\n");
    init_lib_cache();

    printf("lib_tools.c:init_libs() - Refreshing cache:\n");
    LL_FOREACH_SAFE(libs_list,d,tmp) {
       refresh_lib_cache(d->filename);
    }
    
    printf("lib_tools.c:init_libs() - Loading libs:\n");
    LL_FOREACH_SAFE(libs_list,d,tmp) {
       printf("lib_tools.c:init_libs() - Loading %s:\n",d->filename);
       load_lib(d->filename);
    }
}
