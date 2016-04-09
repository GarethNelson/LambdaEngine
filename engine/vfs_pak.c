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
//      Tools for dealing with PAK files
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <physfs.h>
#include <limits.h>

/// Loads a pak file and prepares it for usage
/// filename must be a relative path from the install path
/// e.g data/core01.pak or mods/mymod.pak
int vfs_loadpak(char* filename) {
    int retval;
    char* base_dir;
    char full_filename[PATH_MAX];
    base_dir = (char*)PHYSFS_getBaseDir();
    snprintf(full_filename, sizeof(full_filename), "%s../%s", base_dir, filename);
    printf("vfs_pak.c:vfs_loadpak() - mounting %s at /...", full_filename);
    retval = PHYSFS_mount((const char*)full_filename,"",1);
    if (retval==0) {
       printf("FAILED!\n");
       printf("vfs_pak.c:vfs_loadpak() - PhysFS error: %s\n", PHYSFS_getLastError());
       return 1;
    }
    printf("DONE!\n");
    return 0;
}
