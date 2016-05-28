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
//     Lambda collision tiles extractor
//
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <def_format.h>

#include <wand/magick_wand.h>

void print_usage() {
     fprintf(stderr,"usage: lambdaextractcol -r red_color -g green_color -b blue_color -o output_dir [-f filename] [-v]\n");
}

int main(int argc, char** argv) {
    extern char *optarg;
    extern int optind;

    char* input_filename     = NULL;
    char* output_path        = NULL;
    unsigned int red_c       = 0;
    unsigned int green_c     = 0;
    unsigned int blue_c      = 0;

    int c = 0;
    int verbose = 0;

    if(argc==1) {
       print_usage();
       exit(0);
    }
    while ((c = getopt(argc, argv, "r:g:b:o:f:v")) != -1) {
       switch(c) {
          case 'f':
             input_filename = strdup(optarg);
          break;

          case 'o':
             output_path = strdup(optarg);
          break;

          case 'r':
             red_c = atoi(optarg);
          break;

          case 'g':
             green_c = atoi(optarg);
          break;

          case 'b':
             blue_c = atoi(optarg);
          break;

          case 'v':
             verbose = 1;
          break;

          case '?':
             print_usage();
             exit(1);
          break;
       }
    }

    FILE* input_fd;

    if(input_filename==NULL) {
       input_fd=stdin;
    } else {
       input_fd=fopen(input_filename,"r");
    }

    if(input_fd==NULL) {
       fprintf(stderr,"%s: Unable to open input file \"%s\" - %s\n",argv[0],input_filename,strerror(errno));
       exit(1);
    }
    if(output_path==NULL) {
       fprintf(stderr,"%s: missing -o option\n",argv[0]);
       print_usage();
       exit(1);
    }

    if(verbose) {
       fprintf(stderr,"Loading definitions\n");
    }

    def_database_t* visual_defs = read_def_fd(input_fd);
    def_record_t*   tiles       = def_getall(visual_defs,"TILE");
    if(verbose) {
       fprintf(stderr,"Loaded definitions for %d tiles\n",tiles->type_count);
    }

    if(verbose) {
       fprintf(stderr,"Loading textures for analysis\n");
    }

    setbuf(stdout,NULL);
    setbuf(stderr,NULL);
    MagickWandGenesis();
    unsigned int i=0;
    char* texture_val;
    char texture_id[6];
    char texture_path[PATH_MAX];
    MagickWand **wands = (MagickWand**)malloc(sizeof(MagickWand*)*tiles->type_count);
    MagickBooleanType status;
    unsigned int f=0;
    for(i=0; i< tiles->type_count; i++) {
        // TODO - look into caching here (the engine does it fine using the VFS cache, tools should too)
        wands[i]    = NewMagickWand();
        for(f=0; f< tiles[i].field_count; f++) {
            if(strcmp(tiles[i].fields[f].field_name,"TEXTURE")==0) {
               texture_val = tiles[i].fields[f].field_val;
            }
        }
        snprintf(texture_path,PATH_MAX-1,"%s/%s",output_path,texture_val);
        fprintf(stderr,"Trying to load %s now...",texture_path);
        status      = MagickReadImage(wands[i],texture_path);
        if(status) {
           if(verbose) {
              fprintf(stderr,"DONE!\n");
           }
        } else {
           if(verbose) {
              fprintf(stderr,"FAIL!\n");
           } else {
              fprintf(stderr,"Failed to load %s\n",texture_path);
           }
           exit(1);
        }
    }

    if(verbose) {
       fprintf(stderr,"Loaded textures, starting analysis\n");
    }

    // TODO - make this something other than just stdout
    unsigned int tile_x=0;
    unsigned int tile_y=0;
    unsigned int tile_w=0;
    unsigned int tile_h=0;
    double pixel_r=0;
    double pixel_g=0;
    double pixel_b=0;
    PixelWand* color_wand;
    color_wand = NewPixelWand();
    for(i=0; i< tiles->type_count; i++) {
        tile_w = (unsigned int)MagickGetImageWidth(wands[i]);
        tile_h = (unsigned int)MagickGetImageHeight(wands[i]);
        for(tile_x=0; tile_x<tile_w; tile_x++) {
            for(tile_y=0; tile_y<tile_h; tile_y++) {
                MagickGetImagePixelColor(wands[i],tile_x,tile_y,color_wand);
                pixel_r = PixelGetRed(color_wand)*255;
                pixel_g = PixelGetGreen(color_wand)*255;
                pixel_b = PixelGetBlue(color_wand)*255;
                if((pixel_r==(float)red_c) && (pixel_g==(float)green_c) && (pixel_b==(float)blue_c)) {
                  printf("TILE\n");
                  printf("ID: %d\n",i);
                  printf("%%%%\n");
                  break;
                }
            }
            if((pixel_r==red_c) && (pixel_g==green_c) && (pixel_b==blue_c)) {
               break;
            }
        }
    }

    MagickWandTerminus();

}
