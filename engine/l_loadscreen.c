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
//     Loading screen implementation
//
//-----------------------------------------------------------------------------

#include <unistd.h>
#include <stdio.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#define __IN_MAIN_
#include <lambda_state.h>
#include <lambda_api.h>
#include <pthread.h>


static float rot;



void clean_init_screen() {
     free(global_state->stage_vals);
}

void init_load_screen() {
     printf("l_loadscreen.c:init_load_screen() - Initialising loading screen...");
     IMPORT(video_pre_render)
     IMPORT(video_post_render)
     IMPORT(draw_triangle_rot)
     rot = 1.0f;
     if(global_state->stage_vals == NULL) {
       global_state->stage_vals = malloc(sizeof(loader_vals_t));
       ((loader_vals_t*)global_state->stage_vals)->next_stage = INIT_SPLASH; // TODO - move this to l_main.c
     }
     printf("DONE!\n");
}

void update_load_screen() {
     video_pre_render();
     draw_triangle_rot((global_state->screen_w/2)-50,((global_state->screen_h/2)-50),rot);
     video_post_render();
     if(rot >= 360.0f) {
        global_state->app_stage = ((loader_vals_t*)global_state->stage_vals)->next_stage;
        clean_init_screen();
     }
     rot += 5.0f;
}

