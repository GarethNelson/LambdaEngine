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
#include <lambda_state.h>
#include <lambda_api.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

static void   (*video_pre_render)();
static void   (*video_post_render)();
static void   (*draw_triangle_rot)(float x, float y, float rot);
static float rot;

extern global_state_t global_state;


void clean_init_screen() {
     free(global_state.stage_vals);
}

void init_load_screen() {
     printf("l_loadscreen.c:init_load_screen() - Initialising loading screen...");
     video_pre_render  = dlsym(RTLD_DEFAULT,"video_pre_render");
     video_post_render = dlsym(RTLD_DEFAULT,"video_post_render");
     draw_triangle_rot = dlsym(RTLD_DEFAULT,"draw_triangle_rot");
     rot = 1.0f;
     global_state.stage_vals = malloc(sizeof(loader_vals_t));
     ((loader_vals_t*)global_state.stage_vals)->next_stage = INIT_SPLASH; // TODO - move this to l_main.c
     printf("DONE!\n");
}

void update_load_screen() {
     video_pre_render();
     draw_triangle_rot((SCREEN_WIDTH/2)-50,((SCREEN_HEIGHT/2)-50),rot);
     video_post_render();
     if(rot >= 360.0f) {
        global_state.app_stage = ((loader_vals_t*)global_state.stage_vals)->next_stage;
        clean_init_screen();
     }
     rot += 5.0f;
}

