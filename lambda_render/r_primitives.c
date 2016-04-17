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
//      Part of the lambda_render module, implements basic 2D geometry
//
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <SOIL.h>
#include <dlfcn.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

// TODO - make these dynamic
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

GLuint load_texture(char* vfs_filename) {
  printf("lambda_render/r_primitives.c:load_texture() - Loading %s...",vfs_filename);
  fflush(stdout);
  // TODO - switch to using the global symbol table in l_main.c
  void (*vfs_read)(void* buf,char* filename,unsigned int size);
  unsigned int (*vfs_filelen)(char* filename);
  vfs_read      = dlsym(RTLD_DEFAULT,"vfs_read");
  vfs_filelen   = dlsym(RTLD_DEFAULT,"vfs_filelen");

  unsigned int tex_size = vfs_filelen(vfs_filename);
  void* tex_buf = malloc(tex_size);
  vfs_read((void*)tex_buf,vfs_filename,tex_size);
  
  GLuint tex_id = SOIL_load_OGL_texture_from_memory((const unsigned char*)tex_buf,(int)vfs_filelen,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,
                                                    SOIL_FLAG_MIPMAPS |  SOIL_FLAG_COMPRESS_TO_DXT);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  printf("DONE!\n");
  return tex_id;

}

// TODO - switch to VBOs
void draw_quad(float x, float y, float w, float h,GLuint tex_id) {
     glBindTexture(GL_TEXTURE_2D, tex_id);
     glBegin( GL_QUADS );
//            glColor3f( 0.f, 1.f, 1.f );
            glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y );
            glTexCoord2f(1.0f, 0.0f); glVertex2f(x+w, y );
            glTexCoord2f(1.0f, 1.0f); glVertex2f(x+w,  y+h );
            glTexCoord2f(0.0f, 1.0f); glVertex2f(x,  y+h );
     glEnd();
}

