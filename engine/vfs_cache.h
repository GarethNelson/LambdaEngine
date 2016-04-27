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
//      Cache layer for the VFS
//
//-----------------------------------------------------------------------------

#ifndef __VFS_CACHE_H_
#define __VFS_CACHE_H_

#include <stdio.h>
#include <physfs.h>
#include <limits.h>
#include <uthash.h>
#include <utlist.h>
#include "vfs_io.h"

typedef struct vfs_cache_entry_t {
    char filename[PATH_MAX];
    unsigned int file_len;
    unsigned int is_global;
    unsigned int is_temp;
    void *data;
    struct vfs_cache_entry_t *next;
    UT_hash_handle hh;
} vfs_cache_entry_t;

// main hashtable with hilarious name
vfs_cache_entry_t *vfs_cache_hash;

// linked lists for speed - denormalisation bitches
vfs_cache_entry_t *temp_assets;
vfs_cache_entry_t *global_assets;

void* vfs_cache_read(char* filename); 
unsigned int vfs_cache_filelen(char* filename);
void vfs_precache(char* filename);
void vfs_cache_mark_global(char* filename);
void vfs_cache_mark_temp(char* filename);
void vfs_cache_cleanup();

#endif
