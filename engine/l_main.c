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
#ifdef __linux__
#define __USE_GNU
#endif
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include "vfs_init.h"
#include "lib_tools.h"
#include "l_loadscreen.h"
#include "l_splash.h"

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

global_state_t *global_state;
UT_array *lambda_events;
hook_callbacks_t *hook_callbacks=NULL; // see lambda_api.h
single_callbacks_t *single_callbacks=NULL;
UT_icd events_icd = {sizeof(lambda_event_t),NULL,NULL,NULL};

int main(int argc, char* argv[]) {
    setbuf(stdout,NULL);
    printf("\n*** LAMBDA ENGINE STARTUP ***\n\n");
    printf("l_main.c:main() - Lambda engine starting up\n");
    global_state = (global_state_t*)malloc(sizeof(global_state_t));
    global_state->app_stage = STARTUP;
    utarray_new(lambda_events, &events_icd);
    vfs_init(argv[0]);

    CREATE_HOOK(lambda_post_load)
    CREATE_HOOK(lambda_shutdown)
    CREATE_HOOK(lambda_frame)


    init_libs();
    IMPORT(video_init)
    IMPORT(render_init)
    IMPORT(input_init)


    printf("l_main.c:main() - Starting events system:\n");

    input_init();

    printf("l_main.c:main() - Starting post load hooks:\n");

    RUN_HOOK(lambda_post_load,NULL)


    if(video_init() != 0) {
       printf("l_main.c:main() - Failed to setup video!\n");
       exit(1);
    }
    printf("l_main.c:main() - Video started, preparing render:\n");    

    render_init();
    while(1) {
        RUN_HOOK(lambda_frame,NULL)
        switch(global_state->app_stage) {
            case STARTUP:
               printf("l_main.c:main() - Switching to INIT_LOADSCREEN\n");
               global_state->app_stage = INIT_LOADSCREEN;
               break;
            case INIT_LOADSCREEN:
               init_load_screen();
               printf("l_main.c:main() - Switching to LOADSCREEN\n");
               global_state->app_stage = LOADSCREEN;
               break;
            case LOADSCREEN:
               update_load_screen();
               break;
            case INIT_SPLASH:
               init_splash();
               printf("l_main.c:main() - Switching to SPLASH\n");
               global_state->app_stage = SPLASH;
               break;
            case SPLASH:
               update_splash();
               break;
            case INIT_MAINMENU:
               break;
            case MAINMENU:
               break;
            case INIT_INGAME:
               break;
            case INGAME:
               break;
            case SHUTDOWN:
               RUN_HOOK(lambda_shutdown,NULL)
               exit(0);
               break;

        }
    }

}
