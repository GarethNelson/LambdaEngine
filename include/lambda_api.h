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
//     This magical header file includes the API used by all modules
//
//-----------------------------------------------------------------------------

#ifndef __LAMBDA_API_H_
#define __LAMBDA_API_H_

#ifdef __linux__
#define __USE_GNU
#endif

#include <dlfcn.h>
#include <lambda_state.h>
#include <uthash.h>
#include <utlist.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifndef __IN_VIDEO_
static int    (*video_init)();
static void   (*video_pre_render)();
static void   (*video_post_render)();
#endif

#ifndef __IN_RENDER_
static void   (*render_init)();
static GLuint (*load_texture)(char* vfs_filename);
static void   (*draw_triangle_rot)(float x, float y, float rot);
static void   (*draw_quad)(float x,float y, float w, float h,GLuint tex_id);
static void   (*draw_quad_blend)(float x,float y, float w, float h, GLuint tex_id, float alpha);
#endif

typedef struct hook_callback_t {
    void (*func_ptr)(void* param);
    struct hook_callback_t *next;
} hook_callback_t; // note the lack of s - callback, not callbacks

typedef struct hook_callbacks_t {
    char hook_name[40];
    hook_callback_t *callbacks;
    UT_hash_handle hh;
} hook_callbacks_t;



// Only extern stuff from l_main.c if we're not actually in l_main.c
#ifndef __IN_MAIN_
hook_callbacks_t *hook_callbacks;
global_state_t *global_state;
#else
hook_callbacks_t *hook_callbacks;
global_state_t *global_state;
#endif

#define IMPORT(SYMBOL_NAME) SYMBOL_NAME = dlsym(RTLD_DEFAULT,#SYMBOL_NAME);

// the below is required for any external libs, but not in the main executable
#define INIT_LAMBDA_API() global_state   = ((global_state_t**)   dlsym(RTLD_DEFAULT,"global_state"))[0]; \
                          hook_callbacks = ((hook_callbacks_t**) dlsym(RTLD_DEFAULT,"hook_callbacks"))[0]; 




static hook_callbacks_t *hook_callback;
static hook_callback_t *callback_el;

#define CREATE_HOOK(NEW_HOOK_NAME) hook_callback = malloc(sizeof(hook_callbacks_t)); \
                                   strncpy(hook_callback->hook_name,(const char *)#NEW_HOOK_NAME,40); \
                                   hook_callback->callbacks = NULL; \
                                   HASH_ADD_STR(hook_callbacks,hook_name,hook_callback);

#define ADD_HOOK_CALLBACK(HOOK_NAME,CALLBACK) HASH_FIND_STR(hook_callbacks,#HOOK_NAME,hook_callback); \
                                              callback_el = malloc(sizeof(hook_callback_t)); \
                                              callback_el->func_ptr = CALLBACK; \
                                              LL_APPEND(hook_callback->callbacks,callback_el);

#define RUN_HOOK(HOOK_NAME,PARAM) HASH_FIND_STR(hook_callbacks,#HOOK_NAME,hook_callback); \
                                  LL_FOREACH(hook_callback->callbacks,callback_el) { \
                                      callback_el->func_ptr(PARAM);\
                                  }

#endif

