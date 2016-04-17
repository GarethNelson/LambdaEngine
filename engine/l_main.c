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
//      Program entry point
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include "vfs_init.h"
#include "lib_tools.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

// TODO - make a global symbol table for all this stuff, pass a pointer to all modules
// TODO - put symbols from the main program AND modules into the symbol table
// TODO - update the symbol table only after all modules are loaded
static int    (*video_init)();
static void   (*video_pre_render)();
static void   (*video_post_render)();
static void   (*render_init)();
static GLuint (*load_texture)(char* vfs_filename);
static void   (*draw_quad)(float x,float y, float w, float h,GLuint tex_id);
static GLuint logo_tex;

void render_logo() {
     draw_quad(0,0,(float)SCREEN_WIDTH,(float)SCREEN_HEIGHT, logo_tex);
}

int main(int argc, char* argv[]) {
    setbuf(stdout,NULL);
    printf("\n*** LAMBDA ENGINE STARTUP ***\n\n");
    printf("l_main.c:main() - Lambda engine starting up\n");
    vfs_init(argv[0]);
    init_libs();

    video_init        = dlsym(RTLD_DEFAULT,"video_init");
    video_pre_render  = dlsym(RTLD_DEFAULT,"video_pre_render");
    video_post_render = dlsym(RTLD_DEFAULT,"video_post_render");
    load_texture      = dlsym(RTLD_DEFAULT,"load_texture");
    draw_quad         = dlsym(RTLD_DEFAULT,"draw_quad");
    render_init       = dlsym(RTLD_DEFAULT,"render_init");
    if(video_init() != 0) {
       printf("l_main.c:main() - Failed to setup video!\n");
       exit(1);
    }
    printf("l_main.c:main() - Video started, preparing render:\n");    

    render_init();

    logo_tex = load_texture("/textures/logo.png");
    
    printf("l_main.c:main() - Displaying splashscreen...");
    video_pre_render();
    render_logo();
    video_post_render();
    sleep(3);
    printf("DONE!\n");
 
/*    int running=1; 
    while(running) {
        video_pre_render();
        // process_events();
        // render_frame();
        video_post_render();       
    }*/
}
