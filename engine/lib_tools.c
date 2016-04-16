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
#include <physfs.h>
#include "lib_tools.h"
#include "lib_cache.h"
#include "utlist.h"

lib_details *libs_list = (lib_details*)NULL;

void init_libs() {
    char **lib_physfs_list;
    char **i;
    lib_details *d;

    printf("lib_tools.c:init_libs() - Locate libraries:\n");
    lib_physfs_list = PHYSFS_enumerateFiles("libs");
    for(i=lib_physfs_list; *i != NULL; i++) {
        if(strlen(*i) > 3 && !strcmp(*i + strlen(*i) - 3, ".so")) {
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

    printf("lib_tools.c:init_libs() - Checking cache...");
    // Check cache to see what we need to update
    printf("DONE!\n");

    printf("lib_tools.c:init_libs() - Need to update %d files in cache\n",1);
    
    // put the below in a loop
    printf("lib_tools.c:init_libs() - Updating 1 of 1 files in cache: whatever.so...");
    // load whatever.so from PhysFS, write it to cache
    printf("DONE!\n");

    printf("lib_tools.c:init_libs() - Need to load %d libraries\n",1);
    
    // put the below in a loop, use the libraries list returned from PhysFS
    printf("lib_tools.c:init_libs() - Loading 1 of 1 libraries: whatever.so:\n");
    // load whatever.so from cache using dlopen()
    // whatever.so should print out it's own debugging details
}
