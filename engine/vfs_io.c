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
//      I/O for the VFS
//
//-----------------------------------------------------------------------------

#include <physfs.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void vfs_read(void* buf,char* filename,unsigned int size) {
     PHYSFS_File *fd = PHYSFS_openRead((const char*)filename);
     PHYSFS_read(fd,buf,(PHYSFS_uint32)size,1);
     PHYSFS_close(fd);
}

void vfs_extract(char *vfs_filename, char* extract_to) {
     FILE *ext_fd = fopen((const char*)extract_to,"w");
     PHYSFS_File *vfs_fd = PHYSFS_openRead((const char*)vfs_filename);
     PHYSFS_uint32 file_size = PHYSFS_fileLength(vfs_fd);
     char* buf = (char*)malloc(file_size);
     PHYSFS_read(vfs_fd,buf,file_size,1);
     PHYSFS_close(vfs_fd);
     fwrite(buf,file_size,1,ext_fd);
     fclose(ext_fd);
     free(buf);
}
