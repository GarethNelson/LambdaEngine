//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 2016 by Gareth Nelson (gareth@garethnelson.com)
//
// This library is free software: you can redistribute it and/or modify
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
//     Header file for the def_format library
//
//-----------------------------------------------------------------------------

#ifndef __DEF_FORMAT_H_
#define __DEF_FORMAT_H_
#include <stdio.h>

typedef struct {
    char* field_name;
    char* field_val;
} def_field_t;

typedef struct {
    char* record_type;
    unsigned int field_count;
    def_field_t* fields;
    unsigned int type_count; // used by def_getall
} def_record_t;

typedef struct {
    unsigned int record_count;
    def_record_t* records;
} def_database_t;

// Load a database into memory from a file or a file descriptor
// Note that this does NOT close the file descriptor for you
def_database_t* read_def_fd(FILE* fd);
def_database_t* load_def_file(char* filename);

// Write out a database to disc
void write_def_fd(FILE* fd, def_database_t* database);
void save_def_file(char* filename, def_database_t* database);

// Lookup a particular value in the first record of the specified type
char* def_lookup_val(def_database_t* database, char* record_type, char* field_name);

// Lookup a particular record by searching for a field with the specified value
//  Useful for things such as locating a user by username etc
def_record_t* def_lookup_record(def_database_t* database, char* record_type, char* field_name, char* field_val);

// Returns a dynamically allocated array consisting of all the records of the specified type
// It is the caller's responsibility to free() the array
// The first record in the array will have type_count set to the count of records returned
def_record_t* def_getall(def_database_t* database, char* record_type);

#endif
