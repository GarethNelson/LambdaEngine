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
//     fading splashscreen implementation
//
//-----------------------------------------------------------------------------

#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "l_state.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

static void   (*video_pre_render)();
static void   (*video_post_render)();
static GLuint (*load_texture)(char* vfs_filename);
static void   (*draw_quad_blend)(float x, float y, float w, float h, GLuint tex_id, float alpha);

extern global_state_t global_state;

GLuint logo_tex;

void clean_splash() {
     free(global_state.stage_vals);
}

void init_splash() {
     printf("l_splash.c:init_splash() - Initialising splash:\n");
     video_pre_render  = dlsym(RTLD_DEFAULT,"video_pre_render");
     video_post_render = dlsym(RTLD_DEFAULT,"video_post_render");
     load_texture      = dlsym(RTLD_DEFAULT,"load_texture");
     draw_quad_blend   = dlsym(RTLD_DEFAULT,"draw_quad_blend");
     global_state.stage_vals = malloc(sizeof(splash_vals_t));
     ((splash_vals_t*)global_state.stage_vals)->cur_alpha = 0.0f;
     logo_tex = load_texture("/textures/logo.tga");
}

void update_splash() {
     if(((splash_vals_t*)global_state.stage_vals)->cur_alpha >= 1.0f ) {
        global_state.app_stage = INIT_MAINMENU;
        clean_splash();
     }
     ((splash_vals_t*)global_state.stage_vals)->cur_alpha += 0.025f;
     video_pre_render();
      draw_quad_blend(0,0,(float)SCREEN_WIDTH,(float)SCREEN_HEIGHT, logo_tex, ((splash_vals_t*)global_state.stage_vals)->cur_alpha );
     video_post_render();

}

