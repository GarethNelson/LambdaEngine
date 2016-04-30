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

#include <SDL.h>
#include <SDL_ttf.h>

#define __IN_RENDER_
#include <lambda_api.h>

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

// taken from the SDL_ttf glfont.c sample
static int power_of_two(int input)
{
    int value = 1;

    while ( value < input ) {
        value <<= 1;
    }
    return value;
}

GLuint SDL_GL_LoadTexture(SDL_Surface *surface)
{
    GLfloat texcoord[4];
    GLuint texture;
    int w, h;
    SDL_Surface *image;
    SDL_Rect area;
    Uint8  saved_alpha;
    SDL_BlendMode saved_mode;

    /* Use the surface width and height expanded to powers of 2 */
    w = power_of_two(surface->w);
    h = power_of_two(surface->h);
    texcoord[0] = 0.0f;         /* Min X */
    texcoord[1] = 0.0f;         /* Min Y */
    texcoord[2] = (GLfloat)surface->w / w;  /* Max X */
    texcoord[3] = (GLfloat)surface->h / h;  /* Max Y */

    image = SDL_CreateRGBSurface(
            SDL_SWSURFACE,
            w, h,
            32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
            0x000000FF,
            0x0000FF00,
            0x00FF0000,
            0xFF000000
#else
            0xFF000000,
            0x00FF0000,
            0x0000FF00,
            0x000000FF
#endif
               );
    if ( image == NULL ) {
        return 0;
    }

    /* Save the alpha blending attributes */
    SDL_GetSurfaceAlphaMod(surface, &saved_alpha);
    SDL_SetSurfaceAlphaMod(surface, 0xFF);
    SDL_GetSurfaceBlendMode(surface, &saved_mode);
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);

    /* Copy the surface into the GL texture image */
    area.x = 0;
    area.y = 0;
    area.w = surface->w;
    area.h = surface->h;
    SDL_BlitSurface(surface, &area, image, &area);

    /* Restore the alpha blending attributes */
    SDL_SetSurfaceAlphaMod(surface, saved_alpha);
    SDL_SetSurfaceBlendMode(surface, saved_mode);

    /* Create an OpenGL texture for the image */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,
             0,
             GL_RGBA,
             w, h,
             0,
             GL_RGBA,
             GL_UNSIGNED_BYTE,
             image->pixels);
    SDL_FreeSurface(image); /* No longer needed */

    return texture;
}


void* load_font(char* vfs_filename, unsigned int size) {
  if(!TTF_WasInit()) {
     TTF_Init();
  }
  IMPORT(vfs_cache_filelen)
  IMPORT(vfs_cache_read)
  printf("lambda_render/r_primitives.c:load_font() - Loading %s...",vfs_filename);
  TTF_Font *font;

  unsigned int f_size = vfs_cache_filelen(vfs_filename);
  void* f_buf = vfs_cache_read(vfs_filename);
  font = TTF_OpenFontRW(SDL_RWFromMem(f_buf,f_size),1,size);

  printf("DONE!\n");
  return (void*)font;
}

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

void draw_tiled_quad(float x, float y, float w, float h, float tile_w, float tile_h, GLuint tex_id) {
     glLoadIdentity();
     glBindTexture(GL_TEXTURE_2D, tex_id);
     glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
     glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
     glColor4f(1.0f,1.0f,1.0f,1.0f);
     glBegin( GL_QUADS );
            glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y );
            glTexCoord2f(w/tile_w, 0.0f); glVertex2f(x+w, y );
            glTexCoord2f(w/tile_w, h/tile_h); glVertex2f(x+w,  y+h );
            glTexCoord2f(0.0f, h/tile_h); glVertex2f(x,  y+h );
     glEnd();
}

void draw_text(float x, float y, void* font, char* text) {
     TTF_Font *sdl_font = (TTF_Font*)font;
     SDL_Surface *sdl_output;
     SDL_Color white = { 0xFF, 0xFF, 0xFF, 0 };
     SDL_Color black = { 0x00, 0x00, 0x00, 0 };
     sdl_output = TTF_RenderText_Solid(sdl_font,(const char*)text,white);
     GLuint text_tex = SDL_GL_LoadTexture(sdl_output);
     draw_quad(x,y,sdl_output->w,sdl_output->h,text_tex);
     SDL_FreeSurface(sdl_output);
}


// TODO - switch to VBOs



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
