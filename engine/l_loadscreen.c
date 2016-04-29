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
#include "vfs_cache.h"

static float rot;
static loader_asset_t *tmp_asset;

#define L_PRECACHE(asset_filename) tmp_asset = (loader_asset_t*)malloc(sizeof(loader_asset_t)); \
                                   snprintf(tmp_asset->filename,PATH_MAX-1,"%s",asset_filename);\
                                   LL_APPEND(  ((loader_vals_t*)global_state->stage_vals)->assets, tmp_asset);

void clean_init_screen() {
     pthread_mutex_destroy(&(((loader_vals_t*)global_state->stage_vals)->loader_mutex));
     
     free(global_state->stage_vals);
}

void* loader_thread(void* data) {
      printf("l_loadscreen.c:loader_thread() - Begin loading assets\n");
      pthread_mutex_lock(&(((loader_vals_t*)global_state->stage_vals)->loader_mutex));

      loader_asset_t *asset_to_load, *tmp;
      LL_FOREACH_SAFE( ((loader_vals_t*)global_state->stage_vals)->assets, asset_to_load,tmp) {
         printf("l_loadscreen.c:loader_thread() - Precaching %s\n", asset_to_load->filename);
         vfs_precache(asset_to_load->filename);
         LL_DELETE( ((loader_vals_t*)global_state->stage_vals)->assets, asset_to_load);
      }

      pthread_mutex_unlock(&(((loader_vals_t*)global_state->stage_vals)->loader_mutex));
      printf("l_loadscreen.c:loader_thread() - Finished loading assets!\n");
      return NULL;
}

void init_load_screen() {
     printf("l_loadscreen.c:init_load_screen() - Initialising loading screen\n");
     IMPORT(video_pre_render)
     IMPORT(video_post_render)
     IMPORT(draw_triangle_rot)
     rot = 1.0f;
     if(global_state->stage_vals == NULL) { // if we're here, it means this is the first loading screen
       printf("l_loadscreen.c:init_load_screen() - First start!\n");
       global_state->stage_vals = malloc(sizeof(loader_vals_t));
       ((loader_vals_t*)global_state->stage_vals)->next_stage = INIT_SPLASH;
       ((loader_vals_t*)global_state->stage_vals)->assets = NULL;
       printf("l_loadscreen.c:init_load_screen() - Precache setup begin\n");
       L_PRECACHE("/textures/logo.tga")
       printf("l_loadscreen.c:init_load_screen() - Precache setup end\n");
     }
     printf("l_loadscreen.c:init_load_screen() - Starting asset loader thread:\n");
     pthread_mutex_init(&(((loader_vals_t*)global_state->stage_vals)->loader_mutex), NULL);
     pthread_create(&(((loader_vals_t*)global_state->stage_vals)->loader_thread),NULL,loader_thread,NULL);
}

void update_load_screen() {
     video_pre_render();
     draw_triangle_rot((global_state->screen_w/2)-50,((global_state->screen_h/2)-50),rot);
     video_post_render();
     // this is silly, but means we see SOME of the loading animation even if it's super quick
     if(rot >= 360.0f) {
        rot = 1.0f;
        if(pthread_mutex_trylock(&(((loader_vals_t*)global_state->stage_vals)->loader_mutex))==0) {
           global_state->app_stage = ((loader_vals_t*)global_state->stage_vals)->next_stage;
           clean_init_screen();
        }
     } else {
        rot += 5.0f;
     }
}

