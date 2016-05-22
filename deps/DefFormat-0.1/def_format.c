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
//     Actual implementation of the .def format library
//
//-----------------------------------------------------------------------------

#include "def_format.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

def_database_t* read_def_fd(FILE* fd) {
    def_database_t* retval = (def_database_t*)malloc(sizeof(def_database_t));
    retval->record_count = 0;
    retval->records      = NULL;
    char* line           = NULL;
    size_t len          = 0;
    ssize_t bytes_read;
    unsigned int f_count = 0;

    char* section = NULL;
    char* in_line = NULL;
    while((bytes_read = getline(&in_line,&len,fd)) != -1) {
       line = strdup(in_line);
       if(line[0]=='#') {
          continue;
       }
       if(strncmp(line,"%%",2)==0) {
             section = NULL;
             continue;
       }
       if(line[strlen(line)-1]=='\n') {
          line[strlen(line)-1]='\0';
       }
       if(strlen(line)<2) {
          continue;
       }
       if(strchr(line,':')==NULL) {
          section = strdup((const char*)line);
          retval->record_count++;
          retval->records = realloc((void*)retval->records,(sizeof(def_record_t)*retval->record_count));
          retval->records[retval->record_count - 1].record_type = strdup(section);
          retval->records[retval->record_count - 1].field_count = 0;
          retval->records[retval->record_count - 1].fields      = NULL;
       } else { 
          f_count = retval->records[retval->record_count-1].field_count+1;
          retval->records[retval->record_count -1].field_count = f_count;
          retval->records[retval->record_count -1].fields = realloc((void*)retval->records[retval->record_count-1].fields,sizeof(def_field_t)*f_count);
          retval->records[retval->record_count -1].fields[f_count-1].field_name = strdup(strsep(&line,":"));
          retval->records[retval->record_count -1].fields[f_count-1].field_val  = strdup(strsep(&line,":"))+1; // +1 to get rid of leading whitespace
       }   
       free(line);
       line = NULL;
    }
    return retval;
}

def_database_t* load_def_file(char* filename) {
    FILE* fd = fopen(filename,"r");
    def_database_t* retval = read_def_fd(fd);
    fclose(fd);
    return retval;
}

void write_def_fd(FILE* fd, def_database_t* database) {
     int rec=0;
     int f=0;
     for(rec=0; rec< database->record_count; rec++) {
         fprintf(fd,"%s\n",database->records[rec].record_type);
         for(f=0; f< database->records[rec].field_count; f++) {
             fprintf(fd,"%s: %s\n",database->records[rec].fields[f].field_name,
                                   database->records[rec].fields[f].field_val);
         }
         fprintf(fd,"%%%%\n");
     }
     fprintf(fd,"\n"); 
}

void save_def_file(char* filename, def_database_t* database) {
     FILE* fd = fopen(filename,"w");
     write_def_fd(fd, database);
     fclose(fd);
}

def_record_t* def_getall(def_database_t* database, char* record_type) {
    // TODO - optimise this better
    int c=0;
    def_record_t* retval;
    int i=0;
    retval = (def_record_t*)malloc(sizeof(def_record_t)*database->record_count);
    for(i=0; i<database->record_count; i++) {
        if(strcmp(database->records[i].record_type,record_type)==0) {
           retval[c] = database->records[i];
           c++;
        }
    }
    retval[0].type_count=c;
    return retval;
}

char* def_lookup_val(def_database_t* database, char* record_type, char* field_name) {
      char* retval = NULL;
      // yes this is inefficient, but it's meant for cases where only one of this type exists
      int f=0;
      def_record_t* record = def_getall(database, record_type);
      for(f=0; f< record->field_count; f++) {
          if(strcmp(record->fields[f].field_name,field_name)==0) {
             retval = record->fields[f].field_val;
             break;
          }
      }
      return retval;
}

def_record_t* def_lookup_record(def_database_t* database, char* record_type, char* field_name, char* field_val) {
      def_record_t* retval = (def_record_t*)NULL;
      def_record_t* records = def_getall(database,record_type);
      int f=0;
      int rec=0;
      for(rec=0; rec < records->type_count; rec++) {
        for(f=0; f< records[rec].field_count; f++) {
            if(strcmp(records[rec].fields[f].field_name,field_name)==0) {
               if(strcmp(records[rec].fields[f].field_val,field_val)==0) {
                  retval = &(records[rec]);
                  return retval;
               }
            }
        }
      }
      return retval;
}


