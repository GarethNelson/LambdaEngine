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
#ifdef __linux__
#define __USE_GNU
#endif
#include <dlfcn.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#define __IN_RENDER_
#include <lambda_api.h>

#include <ft2build.h>
#include FT_FREETYPE_H

GLuint load_texture(char* vfs_filename) {
  IMPORT(vfs_cache_filelen)
  IMPORT(vfs_cache_read)
  printf("lambda_render/r_primitives.c:load_texture() - Loading %s...",vfs_filename);

  unsigned int tex_size = vfs_cache_filelen(vfs_filename);
  void* tex_buf = vfs_cache_read(vfs_filename);
  
  GLuint tex_id = SOIL_load_OGL_texture_from_memory((const unsigned char*)tex_buf,tex_size,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,0);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  printf("DONE!\n");
  return tex_id;

}

// TODO - switch to VBOs

void draw_quad(float x, float y, float w, float h,GLuint tex_id) {
     glLoadIdentity();
     glBindTexture(GL_TEXTURE_2D, tex_id);
     glColor4f(1.0f,1.0f,1.0f,1.0f);
     glBegin( GL_QUADS );
            glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y );
            glTexCoord2f(1.0f, 0.0f); glVertex2f(x+w, y );
            glTexCoord2f(1.0f, 1.0f); glVertex2f(x+w,  y+h );
            glTexCoord2f(0.0f, 1.0f); glVertex2f(x,  y+h );
     glEnd();
}

void draw_triangle_rot(float x, float y, float rot) {
     glLoadIdentity();
     glTranslatef( x + 25.0f, y + 25.0, 0.f );
     glRotatef(rot,0.0f,0.0f,1.0f);
     glBegin(GL_TRIANGLES);
            glColor3f(1.0f,0.0f,0.0f); glVertex2f(25.0f,0.0f);
            glColor3f(0.0f,1.0f,0.0f); glVertex2f(0.0f,25.0f);
            glColor3f(0.0f,0.0f,1.0f); glVertex2f(50.0f,50.0f);
     glEnd();
}

void draw_quad_blend(float x, float y, float w, float h, GLuint tex_id, float alpha) {

     draw_quad(x,y,w,h,tex_id);
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     glDisable(GL_DEPTH_TEST);
     glDisable(GL_TEXTURE_2D);
     glBegin(GL_QUADS);
            glColor4f( .0f, .0f, .0f,alpha );
            glVertex2f(x, y );
            glVertex2f(x+w, y );
            glVertex2f(x+w,  y+h );
            glVertex2f(x,  y+h );
     glEnd();
     glEnable(GL_DEPTH_TEST);
     glDisable(GL_BLEND);
     glEnable(GL_TEXTURE_2D);
}
