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
//     Definitions of the FSM used to control engine flow
//
//-----------------------------------------------------------------------------
#ifndef __L_STATE_H_
#define __L_STATE_H_
#include <pthread.h>
#include <limits.h>
#include "uthash.h"
#include "utlist.h"
#include "utarray.h"

/// this indicates what overall state (i.e what stage) the program is in
typedef enum {
    STARTUP,
    INIT_LOADSCREEN,
    LOADSCREEN,
    INIT_SPLASH,
    SPLASH,
    INIT_MAINMENU,
    MAINMENU,
    INIT_INGAME,
    INGAME,
    SHUTDOWN
} e_lambda_stage;

typedef struct {
    char  hash_k[40];
    void* hash_v; // we never free() these unless we're replacing them
    UT_hash_handle hh;
} global_vals_hash;

typedef struct {
    e_lambda_stage app_stage;
    void* stage_vals;  // pointer to a struct that contains the actual stage-specific state data
    global_vals_hash* global_vals; // uthash
    unsigned int frame_delta; // timekeeping, ms - handled by video module
    unsigned int last_frame;
    unsigned int display_fps;  // render FPS
    int screen_w;
    int screen_h;
} global_state_t;


// *********************************************
//  General events data
typedef enum {
    QUIT,    // close the whole game
    ACTION,  // do stuff
    BACK,    // backup (for menus and such)
    UP,      // go down
    DOWN,    // go up
    LEFT,    // go right
    RIGHT,   // i'm joking, seriously
} e_lambda_event;

typedef struct {
    e_lambda_event event_type;
    // this space reserved for future expansion - event parameters, bitches
} lambda_event_t;
// *********************************************


// *********************************************
//  SPLASH data
// TODO - multiple logos?
typedef struct {
    float cur_alpha;
    unsigned int fade_in;
} splash_vals_t;
// *********************************************


// *********************************************
//  LOADSCREEN data
typedef struct loader_asset_t {
    char filename[PATH_MAX];
    struct loader_asset_t *next;
} loader_asset_t;

typedef struct {
    e_lambda_stage next_stage; // after loading resources, what stage do we go to next?
                               // realistically will always be MAINMENU, SPLASH or INGAME
    char next_map[40];         // if next_stage is INGAME, specify the map
    loader_asset_t *assets;    // used with utlist
    pthread_t loader_thread;
    pthread_mutex_t loader_mutex;
} loader_vals_t;

// *********************************************

// *********************************************
//  MAINMENU data
typedef struct {
    unsigned int cur_item;
} mainmenu_vals_t;
// *********************************************

/*
 Transition between states is done manually:
 1 - Save any important data from stage_vals to global_vals and do general cleanup
 2 - free() global_state->stage_vals
 3 - malloc() a new global_state->stage_vals
 4 - access your data by casting global_state->state_vals to the relevant struct
 */

#endif
