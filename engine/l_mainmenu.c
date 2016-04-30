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
//     Main menu implementation
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


static GLuint bg_tex;
static void* menu_title_font;
static void* menu_item_font;

void clean_mainmenu() {
     free(global_state->stage_vals);
}

void init_mainmenu() {
     printf("l_mainmenu.c:init_mainmenu() - Initialising main menu:\n");
     IMPORT(video_pre_render)
     IMPORT(video_post_render)
     IMPORT(load_texture)
     IMPORT(load_font)
     IMPORT(draw_text)
     IMPORT(draw_tiled_quad)
     menu_title_font = load_font("/fonts/default.ttf",128);
     menu_item_font  = load_font("/fonts/default.ttf",64);
     bg_tex = load_texture("/textures/bg_tex.png");
}

void update_mainmenu() {
     video_pre_render();
      draw_tiled_quad(0,0,global_state->screen_w,global_state->screen_h,128,128,bg_tex);
      draw_text((global_state->screen_w/2)-256,((global_state->screen_h/4)-128),menu_title_font,"Lambda Engine");
      draw_text((global_state->screen_w/2)-200,((global_state->screen_h/4)+64), menu_item_font,"New Game");
      draw_text((global_state->screen_w/2)-200,((global_state->screen_h/4)+128),menu_item_font,"Quit");
     video_post_render();

}

