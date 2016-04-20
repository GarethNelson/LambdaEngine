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

/// this indicates what overall state (i.e what stage) the program is in
typedef enum {
    STARTUP,
    LOADSCREEN,
    MAINMENU,
    INGAME,
    SHUTDOWN
} e_lambda_stage;

typedef struct {
    e_lambda_stage app_stage;
    void* stage_vals; // pointer to a struct that contains the actual stage-specific state data
} global_state_t;

#endif
