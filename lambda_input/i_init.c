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
//     Initialisation code for the input module
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <utarray.h>
#include <SDL.h>
#define __IN_INPUT_
#include "lambda_api.h"





void i_shutdown(void* param) {
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

void i_default(void* param) {
     printf("lambda_input/i_init.c:i_default() - default handler called!\n");
}

void i_handle_sdl_key_event(SDL_Event *e) {
     // TODO - Make this configurable
     switch(e->key.keysym.sym) {
        case SDLK_RETURN:
          RUN_SINGLE_HOOK(input_action,NULL)
        break;
        case SDLK_ESCAPE:
          RUN_SINGLE_HOOK(input_back,NULL)
        break;
        case SDLK_UP:
          RUN_SINGLE_HOOK(input_up,NULL)
        break;
        case SDLK_DOWN:
          RUN_SINGLE_HOOK(input_down,NULL)
        break;
        case SDLK_LEFT:
          RUN_SINGLE_HOOK(input_left,NULL)
        break;
        case SDLK_RIGHT:
          RUN_SINGLE_HOOK(input_right,NULL)
        break;
     }
}

void i_frame(void* param) {
     utarray_clear(lambda_events);
     SDL_Event e;
     while (SDL_PollEvent(&e)) {
        switch(e.type) {
           case SDL_QUIT:
             global_state->app_stage = SHUTDOWN;
           break;
           case SDL_KEYDOWN:
             i_handle_sdl_key_event(&e);
           break;
        }

     }
}

void i_postload(void* param) {
     printf("lambda_input/i_init.c:i_postload() - Setting up hooks...");
     ADD_HOOK_CALLBACK(lambda_frame,&i_frame)
     SET_SINGLE_HOOK(input_action, &i_default)
     SET_SINGLE_HOOK(input_back,   &i_default)
     SET_SINGLE_HOOK(input_up,     &i_default)
     SET_SINGLE_HOOK(input_down,   &i_default)
     SET_SINGLE_HOOK(input_left,   &i_default)
     SET_SINGLE_HOOK(input_right,  &i_default)
     printf("DONE!\n");
}

void input_init() {
     printf("lambda_input/i_init.c:input_init() - Starting up SDL...");
     if(SDL_WasInit(0)==0) {
        SDL_Init(0);
     }
     SDL_InitSubSystem(SDL_INIT_EVENTS);
     printf("DONE!\n");
}

void __attribute__((constructor)) init_module() {
     INIT_LAMBDA_API()
     ADD_HOOK_CALLBACK(lambda_post_load,&i_postload)
     ADD_HOOK_CALLBACK(lambda_shutdown,&i_shutdown)
     printf("lambda_input/i_init.c:init_module() - module loaded\n");
}
