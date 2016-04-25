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
#include <SDL.h>
#define __IN_INPUT_
#include "lambda_api.h"

void i_frame(void* param) {
     SDL_Event e;
     while (SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) {
           global_state->app_stage = SHUTDOWN;
        }
     }
}

void i_shutdown(void* param) {
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
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
     ADD_HOOK_CALLBACK(lambda_frame,&i_frame)
     ADD_HOOK_CALLBACK(lambda_shutdown,&i_shutdown)
     printf("lambda_input/i_init.c:init_module() - module loaded\n");
}
