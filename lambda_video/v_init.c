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
//      Part of the lambda_video module, used to create an OpenGL context
//
//-----------------------------------------------------------------------------

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#define __IN_VIDEO_
#include <lambda_api.h>

SDL_Window *screen;
SDL_GLContext glcontext;

void v_frame(void* param) {
     unsigned int frame_timeout = (global_state->last_frame)+33;
     while (!SDL_TICKS_PASSED(SDL_GetTicks(), frame_timeout)) {
         usleep(1);
     }
     unsigned int now = SDL_GetTicks();
     global_state->frame_delta = now-global_state->last_frame;
     global_state->last_frame  = now;
     float fps       = (1.0f/global_state->frame_delta)*1000.0f;
     global_state->display_fps = (unsigned int)fps;
}

void v_post_init(void* param) { // post init callback
     printf("lambda_video/v_init.c:v_post_init() - Setting up hook callbacks...");
     ADD_HOOK_CALLBACK(lambda_frame,&v_frame)
     global_state->last_frame = SDL_GetTicks();
     printf("DONE!\n");
}

void v_shutdown(void* param) {
     SDL_QuitSubSystem(SDL_INIT_TIMER);
     SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void __attribute__((constructor)) init_module() {
     INIT_LAMBDA_API()
     CREATE_HOOK(v_pre_render)
     ADD_HOOK_CALLBACK(lambda_post_load,&v_post_init)
     ADD_HOOK_CALLBACK(lambda_shutdown,&v_shutdown)
     printf("lambda_video/v_init.c:init_module() - module loaded\n");
}



// TODO - move these to something like v_hooks.c
void video_pre_render() {
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
     //glMatrixMode( GL_MODELVIEW );
     glLoadIdentity();

}

void video_post_render() {
     SDL_PumpEvents();
     SDL_GL_SwapWindow(screen);
}

int video_init() {
    printf("lambda_video/v_init.c:video_init() - Starting up SDL...");
    if(SDL_WasInit(0)==0) {
       SDL_Init(0);
    }
    SDL_InitSubSystem(SDL_INIT_TIMER);
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetSwapInterval(1);
    printf("DONE!\n");
    
    printf("lambda_video/v_init.c:video_init() - Creating window...");
    global_state->screen_w = 1024; // TODO - make this dynamic
    global_state->screen_h = 768;
    screen = SDL_CreateWindow("Lambda Engine",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,global_state->screen_w,global_state->screen_h,
                                              SDL_WINDOW_OPENGL);

    printf("DONE!\n");

    printf("lambda_video/v_init.c:video_init() - Configuring OpenGL context...");
    glcontext = SDL_GL_CreateContext(screen);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, global_state->screen_w, global_state->screen_h, 0.0, 1.0, -1.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glEnable( GL_TEXTURE_2D );
    glClearColor( 0.0, 0.0, 0.0, 1 );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    printf("DONE!\n");
    return 0;
}
