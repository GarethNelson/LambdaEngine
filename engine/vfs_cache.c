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

#include <stdio.h>
#include <physfs.h>
#include <stdlib.h>
#include "vfs_cache.h"
#include "vfs_io.h"

vfs_cache_entry_t *vfs_cache_hash = NULL;
vfs_cache_entry_t *temp_assets    = NULL;
vfs_cache_entry_t *global_assets  = NULL;

void vfs_precache(char* filename) {
      vfs_cache_entry_t *cache_entry;
      HASH_FIND_STR(vfs_cache_hash,filename,cache_entry);
      if(cache_entry != NULL) {
         return;
      }
      cache_entry = malloc(sizeof(vfs_cache_entry_t));
      strncpy(cache_entry->filename,(const char*)filename,PATH_MAX);
      cache_entry->file_len = vfs_filelen(filename);
      cache_entry->data     = malloc(cache_entry->file_len);
      vfs_read(cache_entry->data,filename,cache_entry->file_len);
      HASH_ADD_STR(vfs_cache_hash,filename,cache_entry);
}

void* vfs_cache_read(char* filename) {
      vfs_cache_entry_t *cache_entry;
      HASH_FIND_STR(vfs_cache_hash,filename,cache_entry);
      if(cache_entry==NULL) {
         vfs_precache(filename);
         HASH_FIND_STR(vfs_cache_hash,filename,cache_entry);
      }
      return cache_entry->data;
}

unsigned int vfs_cache_filelen(char* filename) {
      vfs_cache_entry_t *cache_entry;
      HASH_FIND_STR(vfs_cache_hash,filename,cache_entry);
      if(cache_entry==NULL) {
         return vfs_filelen(filename);
      }
      return cache_entry->file_len;
}

void vfs_cache_mark_global(char* filename) {
}

void vfs_cache_mark_temp(char* filename) {
}

void vfs_cache_cleanup() {
}
