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
//     Ingame rendering code
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
#include <lambda_api.h>
#include <lambda_state.h>


void clean_ingame() {
     IMPORT(i_default)
     // TODO - add generic reset to the input module
     SET_SINGLE_HOOK(input_action, i_default);
     SET_SINGLE_HOOK(input_back,   i_default);
     SET_SINGLE_HOOK(input_up,     i_default);
     SET_SINGLE_HOOK(input_down,   i_default);
     SET_SINGLE_HOOK(input_left,   i_default);
     SET_SINGLE_HOOK(input_right,  i_default);
     free(global_state->stage_vals);
}

void ingame_up_handler(void* param) {
}

void ingame_down_handler(void* param) {
}

void ingame_action_handler(void* param) {
}

void ingame_left_handler(void* param) {
}

void ingame_right_handler(void* param) {
}

void init_ingame() {
     printf("l_ingame.c:init_ingame() - Initialising map:\n");

     SET_SINGLE_HOOK(input_up, &ingame_up_handler);
     SET_SINGLE_HOOK(input_down,&ingame_down_handler);
     SET_SINGLE_HOOK(input_action,&ingame_action_handler);
     SET_SINGLE_HOOK(input_left,&ingame_left_handler);
     SET_SINGLE_HOOK(input_right,&ingame_right_handler);
}

void update_ingame() {
     video_pre_render();

     video_post_render();

}

