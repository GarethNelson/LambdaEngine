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
#include "uthash.h"

/// this indicates what overall state (i.e what stage) the program is in
typedef enum {
    STARTUP,
    LOADSCREEN,
    SPLASH,
    MAINMENU,
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
} global_state_t;

// *********************************************
//  SPLASH data
// *********************************************
// TODO - multiple logos?
typedef struct {
    float cur_alpha;
} splash_vals_t;



// *********************************************
//  LOADSCREEN data
typedef struct {
    e_lambda_stage next_stage; // after loading resources, what stage do we go to next?
                               // realistically will always be MAINMENU, SPLASH or INGAME
    char next_map[40];         // if next_stage is INGAME, specify the map
    float rot_val;
} loader_vals_t;

// *********************************************

/*
 Transition between states is done manually:
 1 - Save any important data from stage_vals to global_vals and do general cleanup
 2 - free() global_state->stage_vals
 3 - malloc() a new global_state->stage_vals
 4 - access your data by casting global_state->state_vals to the relevant struct
 */

#endif
