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
//     Lambda map compiler
//
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include <uthash.h>
#include "ini.h"

typedef struct {
    char* filename;
    unsigned int hz_count;
} hotzonemap_t;

typedef struct {
    char* id;
    unsigned int r;
    unsigned int g;
    unsigned int b;
    UT_hash_handle hh;
} hotzone_t;

typedef struct {
    char* filename;
    unsigned int e_count;
} entitymap_t;

typedef struct {
    char* id;
    unsigned int r;
    unsigned int g;
    unsigned int b;
    UT_hash_handle hh;
} entity_t;

typedef struct {
    char* map_name;
    unsigned int tile_res_w;
    unsigned int tile_res_h;
} mapinfo_t;

typedef struct {
    char* filename;
} mapvis_t;

typedef struct {
    mapinfo_t     mapinfo;
    mapvis_t      mapvis;
    hotzonemap_t  hotzone_map;
    entitymap_t   entity_map;
    hotzone_t*    hotzones;
    entity_t*     entities;
} mapdef_t;

static mapdef_t mapdef;
static hotzone_t *hotzone_tmp;
static entity_t  *entity_tmp;

static char* hotzone_id=NULL; // used for caching which section we're in
static char* entity_id=NULL;
#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

// bad style i know
void update_hz_colour(char* hotzone_id, char* colstr) {
     unsigned int r;
     unsigned int g;
     unsigned int b;
     sscanf(colstr,"%u,%u,%u",&r,&g,&b);
     HASH_FIND_STR(mapdef.hotzones,hotzone_id,hotzone_tmp);
     hotzone_tmp->r = r;
     hotzone_tmp->g = g;
     hotzone_tmp->b = b;
}

void update_ent_colour(char* entity_id, char* colstr) {
     unsigned int r;
     unsigned int g;
     unsigned int b;
     sscanf(colstr,"%u,%u,%u",&r,&g,&b);
     HASH_FIND_STR(mapdef.entities,entity_id,entity_tmp);
     entity_tmp->r = r;
     entity_tmp->g = g;
     entity_tmp->b = b;
}

static int def_handler(void* user, const char* section, const char* name, const char* value) {
       if(MATCH("MAPINFO","MAPNAME")) {
          mapdef.mapinfo.map_name = strdup(value);
       } else if (MATCH("MAPINFO","TILERESW")) {
          mapdef.mapinfo.tile_res_w = atoi(value);
       } else if (MATCH("MAPINFO","TILERESH")) {
          mapdef.mapinfo.tile_res_h = atoi(value);
       } else if (MATCH("MAPVIS","FILENAME")) {
          mapdef.mapvis.filename = strdup(value);
       } else if (MATCH("ENTITYMAP","FILENAME")) {
          mapdef.entity_map.filename = strdup(value);
       } else if (MATCH("ENTITYMAP","ECOUNT")) {
          mapdef.entity_map.e_count = atoi(value);
          mapdef.entities           = NULL;
       } else if (MATCH("HOTZONEMAP","FILENAME")) {
          mapdef.hotzone_map.filename = strdup(value);
       } else if (MATCH("HOTZONEMAP","HZCOUNT")) {
          mapdef.hotzone_map.hz_count = atoi(value);
          mapdef.hotzones             = NULL;
       } else if (MATCH("HOTZONE","ID")){
          hotzone_id = strdup(value);
          HASH_FIND_STR(mapdef.hotzones,value,hotzone_tmp);
          if (hotzone_tmp==NULL) {
              hotzone_tmp = malloc(sizeof(hotzone_t));
              hotzone_tmp->id = strdup(value);
              hotzone_tmp->r  = 0;
              hotzone_tmp->g  = 0;
              hotzone_tmp->b  = 0;
              HASH_ADD_STR(mapdef.hotzones,id,hotzone_tmp);
          } else {
              fprintf(stderr,"Duplicate hotzone ID %s\n", value);
              return 0;
          }
       } else if (MATCH("HOTZONE","COLOUR")) {
          if(hotzone_id==NULL) {
              fprintf(stderr,"Could not locate hotzone ID\n");
              return 0;
          } else {
              update_hz_colour(hotzone_id,strdup(value));
          }
       } else if (MATCH("ENTITY","ID")) {
          entity_id = strdup(value);
          HASH_FIND_STR(mapdef.entities,value,entity_tmp);
          if (entity_tmp==NULL) {
              entity_tmp = malloc(sizeof(entity_t));
              entity_tmp->id = strdup(value);
              entity_tmp->r  = 0;
              entity_tmp->g  = 0;
              entity_tmp->b  = 0;
              HASH_ADD_STR(mapdef.entities,id,entity_tmp);
          } else {
              fprintf(stderr,"Duplicate entity ID %s\n", value);
              return 0;
          }
       } else if (MATCH("ENTITY","COLOUR")) {
          if(entity_id==NULL) {
              fprintf(stderr,"Could not locate entity ID\n");
              return 0;
          } else {
              update_ent_colour(entity_id,strdup(value));
          } 
       } else {
          fprintf(stderr,"Unknown section/name pair %s:%s\n",section,name);
          return 0;
       }
       return 1;
}

void print_usage() {
     printf("usage: lambdamc -i map_source_dir [-o map_output_dir]\n");
}

int main(int argc, char** argv) {
    extern char *optarg;
    extern int optind;

    char* src_dir =NULL;
    char* dest_dir=NULL;
    char mapinfo_def_filename[PATH_MAX];
    int c = 0;
    int parse_error = 0;

    if(argc==1) {
       print_usage();
       exit(0);
    }
    while ((c = getopt(argc, argv, "i:o:")) != -1) {
       switch(c) {
          case 'i':
             src_dir = optarg;
          break;

          case 'o':
             dest_dir = optarg;
          break;

          case '?':
             print_usage();
             exit(1);
          break;
       }
    }

    if(src_dir==NULL) {
        fprintf(stderr, "%s: missing -i option\n",argv[0]);
        print_usage();
        exit(1);
    }
    if(dest_dir==NULL) {
        dest_dir = strdup(src_dir);
    }
    
    snprintf(mapinfo_def_filename,PATH_MAX-1,"%s/%s",src_dir,"mapinfo.def");
    
    parse_error = ini_parse(mapinfo_def_filename, def_handler, NULL);
    if (parse_error != 0) {
       if (parse_error >0) {
          fprintf(stderr,"First error on line %d of %s\n",parse_error,mapinfo_def_filename);
       } else if(parse_error==-1) {
          fprintf(stderr,"Could not open %s\n",mapinfo_def_filename);
       }
    }
    
}
