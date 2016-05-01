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
static GLuint menu_title_tex;
static int menu_title_w;
static int menu_title_h;
static int menu_title_text_w;
static int menu_title_text_h;
static int menu_title_x;
static int menu_title_y;
static float offset;

#define MAX_MENU_ITEMS 10

static int menu_item_w_active[MAX_MENU_ITEMS+1];
static int menu_item_w_inactive[MAX_MENU_ITEMS+1];
static int menu_item_h[MAX_MENU_ITEMS+1];
static int menu_item_text_w[MAX_MENU_ITEMS+1];
static int menu_item_text_h[MAX_MENU_ITEMS+1];
static int menu_item_x[MAX_MENU_ITEMS+1];
static int menu_item_y[MAX_MENU_ITEMS+1];
static GLuint menu_item_active_tex[MAX_MENU_ITEMS+1];
static GLuint menu_item_inactive_tex[MAX_MENU_ITEMS+1];

void clean_mainmenu() {
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

void mainmenu_up_handler(void* param) {
     ((mainmenu_vals_t*)global_state->stage_vals)->cur_item--;
     if( ((mainmenu_vals_t*)global_state->stage_vals)->cur_item == 0) {
       ((mainmenu_vals_t*)global_state->stage_vals)->cur_item = 1;
     }
}

void mainmenu_down_handler(void* param) {
     ((mainmenu_vals_t*)global_state->stage_vals)->cur_item++;
     if( ((mainmenu_vals_t*)global_state->stage_vals)->cur_item  > ((mainmenu_vals_t*)global_state->stage_vals)->item_count) {
       ((mainmenu_vals_t*)global_state->stage_vals)->cur_item = ((mainmenu_vals_t*)global_state->stage_vals)->item_count;
     }
}

void mainmenu_action_handler(void* param) {
     switch( ((mainmenu_vals_t*)global_state->stage_vals)->cur_item) {
         case 1: // New game
           clean_mainmenu();
           global_state->app_stage = INIT_INGAME;
         break;
         case 2: // Options
         break;
         case 3: // Quit
           clean_mainmenu();
           global_state->app_stage = SHUTDOWN;
         break;
     }
}

#define MENUITEM(ID,ITEMTEXT) predraw_text(menu_item_font, 255,255,255, ITEMTEXT, &menu_item_w_active[ID], &menu_item_h[ID], &menu_item_text_w[ID], &menu_item_text_h[ID], &menu_item_active_tex[ID]); \
                              predraw_text(menu_item_font, 128,128,128, ITEMTEXT, &menu_item_w_inactive[ID], &menu_item_h[ID],&menu_item_text_w[ID], &menu_item_text_h[ID], &menu_item_inactive_tex[ID]); \
                              menu_item_x[ID] = ((global_state)->screen_w/2) - (menu_item_text_w[1]/4); \
                              menu_item_y[ID] = (menu_item_y[ID-1]+(menu_item_h[ID-1]/2));
     

void init_mainmenu() {
     printf("l_mainmenu.c:init_mainmenu() - Initialising main menu:\n");
     IMPORT(video_pre_render)
     IMPORT(video_post_render)
     IMPORT(load_texture)
     IMPORT(load_font)
     IMPORT(draw_text)
     IMPORT(predraw_text)
     IMPORT(draw_tiled_quad)
     IMPORT(draw_transparent_quad)
     IMPORT(draw_scroll_bg)

     menu_title_font = load_font("/fonts/default.ttf",128);
     menu_item_font  = load_font("/fonts/default.ttf",64);
     bg_tex = load_texture("/textures/bg_tex.png");
     global_state->stage_vals = malloc(sizeof(mainmenu_vals_t));
     offset = 0.0f;
     ((mainmenu_vals_t*)global_state->stage_vals)->cur_item=1;
     ((mainmenu_vals_t*)global_state->stage_vals)->item_count=3;

     predraw_text(menu_title_font, 255,0,0, "Lambda Engine", &menu_title_w, &menu_title_h, &menu_title_text_w, &menu_title_text_h, &menu_title_tex);
     menu_title_x = ((global_state->screen_w)/2) - (menu_title_text_w/4);
     menu_title_y = ((global_state->screen_h)/4);
     menu_item_y[0] = menu_title_y+128;
     
     MENUITEM(1,"New Game")     
     MENUITEM(2,"Options")
     MENUITEM(3,"Quit")

     SET_SINGLE_HOOK(input_up, &mainmenu_up_handler);
     SET_SINGLE_HOOK(input_down,&mainmenu_down_handler);
     SET_SINGLE_HOOK(input_action,&mainmenu_action_handler);
}

void update_mainmenu() {
     int i;
     offset++;
     if(offset>=64) {
        offset = 0.0f;
     }
     video_pre_render();
      draw_scroll_bg(0,0,64,64,offset,bg_tex);
      draw_transparent_quad(menu_title_x,menu_title_y,menu_title_w,menu_title_h,menu_title_tex);

      for(i=1; i<=((mainmenu_vals_t*)global_state->stage_vals)->item_count; i++) {
          if (i == (((mainmenu_vals_t*)global_state->stage_vals)->cur_item)) {
             draw_transparent_quad(menu_item_x[i],menu_item_y[i],menu_item_w_active[i],64,menu_item_active_tex[i]);
          } else {
             draw_transparent_quad(menu_item_x[i]+10,menu_item_y[i],menu_item_w_inactive[i],64,menu_item_inactive_tex[i]);
          }
      }

     video_post_render();

}

