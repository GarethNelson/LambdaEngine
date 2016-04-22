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

#define __IN_MAIN_
#include <lambda_api.h>
#include <lambda_state.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

static GLuint logo_tex;

void clean_splash() {
     free(global_state->stage_vals);
}

void init_splash() {
     printf("l_splash.c:init_splash() - Initialising splash:\n");
     IMPORT(video_pre_render)
     IMPORT(video_post_render)
     IMPORT(load_texture)
     IMPORT(draw_quad_blend)
     global_state->stage_vals = malloc(sizeof(splash_vals_t));
     ((splash_vals_t*)global_state->stage_vals)->cur_alpha = 1.0f;
     ((splash_vals_t*)global_state->stage_vals)->fade_in = 1;
     logo_tex = load_texture("/textures/logo.tga");
}

void update_splash() {
     if(((splash_vals_t*)global_state->stage_vals)->cur_alpha >= 1.1f) {
        global_state->app_stage = INIT_MAINMENU;
        clean_splash();
     }
     if(((splash_vals_t*)global_state->stage_vals)->cur_alpha <= 0.1f ) {
        sleep(1);
        ((splash_vals_t*)global_state->stage_vals)->fade_in = 0;
     }
     if(((splash_vals_t*)global_state->stage_vals)->fade_in==1) {
        ((splash_vals_t*)global_state->stage_vals)->cur_alpha -= 0.025f;
     } else {
        ((splash_vals_t*)global_state->stage_vals)->cur_alpha += 0.025f;
     }
     video_pre_render();
      draw_quad_blend(0,0,(float)SCREEN_WIDTH,(float)SCREEN_HEIGHT, logo_tex, ((splash_vals_t*)global_state->stage_vals)->cur_alpha );
     video_post_render();

}

