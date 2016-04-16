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
//      Code for setting up the VFS layer using PhysFS
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <physfs.h>
#include <limits.h>
#include "vfs_pak.h"

void vfs_init(char* argv) {
     int retval;
     struct stat st;
     const char* user_dir;
     char config_dir[PATH_MAX];
     
     printf("vfs_init.c:vfs_init() - Init PhysFS...");
     retval = PHYSFS_init(argv);
     if(retval==0) {
       printf("FAILED!\n");
       printf("vfs_init.c:vfs_init() - PhysFS error: %s\n", PHYSFS_getLastError());
       exit(EXIT_FAILURE);
     }
     printf("DONE!\n");

     printf("vfs_init.c:vfs_init() - Refresh lambda config directory...");
     user_dir = PHYSFS_getUserDir();
     snprintf(config_dir,sizeof(config_dir),"%s.lambda",user_dir);
     if(stat(config_dir, &st) != 0) {
        mkdir(config_dir, 0700);
     }
     printf("DONE!\n");

     printf("vfs_init.c:vfs_init() - Load core01.pak:\n");
     retval = vfs_loadpak("data/core01.pak");
     if(retval != 0) {
       printf("vfs_init.c:vfs_init() - Could not load core01.pak, this file is required!\n");
       exit(EXIT_FAILURE);
     }

     printf("vfs_init.c:vfs_init() - Load core02.pak:\n");
     retval = vfs_loadpak("data/core02.pak");
     if(retval != 0) {
       printf("vfs_init.c:vfs_init() - Could not load core02.pak, this file is required!\n");
       exit(EXIT_FAILURE);
     }
}
