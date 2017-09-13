#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL/SDL_ttf.h>
#include <emscripten.h>
#include <unistd.h>
#include <stdlib.h>

void printTile(Uint32* pixels, Uint32* tile, Uint32 x, Uint32 y){
  for(int rowNum=0; rowNum<8; rowNum++){
    for(int columnNum=0; columnNum<8; columnNum++){
      pixels[(y + rowNum)*256 + (x + columnNum)] = tile[rowNum*8 + columnNum];
    }   
  }
}

int main()
{
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_Window *window;
  SDL_Renderer *renderer;

  Uint32 palette[32];
  char rawPalette[32];

  Uint32 rawTiles[0x800]; // 0x400 * 32 bit rows
  Uint32* tiles[512];

  SDL_CreateWindowAndRenderer(256, 192, 0, &window, &renderer);

  // do pixel stuff
  SDL_Surface *screen = SDL_CreateRGBSurface(0, 256, 192, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
  SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 192);

  // alex kidd load screen background colour
  Uint32* pixels = (Uint32*) screen->pixels;
  for(int i=0; i<256*192; i++){
    pixels[i] = 0xFFFFFFAA;
  }

  // load palette
  FILE* paletteFile = fopen("assets/logopalette.dat", "rb");
  fread(rawPalette, 1, 32, paletteFile);
  fclose(paletteFile);

  // convert these to colours
  for(int i=0; i<32; i++){
    char red = rawPalette[i] & 0x3;
    char green = (rawPalette[i] >> 2) & 0x3;
    char blue = (rawPalette[i] >> 4) & 0x3;
    palette[i] = (0xFF << 24) | ((red * 0x55) << 16) | ((green * 0x55) << 8) | (blue * 0x55);
  }

  // load tiles
  FILE* tileFile = fopen("assets/start_screen_tiles.dat", "rb");
  fread(rawTiles, 1, 0x2000, tileFile);
  fclose(tileFile);

  // convert some and apply palette
  // we'll go with tile #67 - a few colours, top right part of the A

  for(int tileNum=0; tileNum<512; tileNum++){
    Uint32 tileOffset = tileNum * 8;
    Uint32* tile = malloc(8*8*sizeof(Uint32));
    for(int rowNum=0; rowNum<8; rowNum++){
      // we do row at a time
      // 4 bits per pixel, striped across
      // weak version here for now
      Uint32 rawRow[8];
      Uint32 rowData = rawTiles[tileOffset + rowNum];

      rawRow[0] = (rowData & 0x80808080) >> 7;
      rawRow[1] = (rowData & 0x40404040) >> 6;
      rawRow[2] = (rowData & 0x20202020) >> 5;
      rawRow[3] = (rowData & 0x10101010) >> 4;
      rawRow[4] = (rowData & 0x08080808) >> 3;
      rawRow[5] = (rowData & 0x04040404) >> 2;
      rawRow[6] = (rowData & 0x02020202) >> 1;
      rawRow[7] = (rowData & 0x01010101);

      for(int columnNum=0; columnNum<8; columnNum++){
        Uint32 offset = ((rawRow[columnNum] & 0x01000000) >> 21) | ((rawRow[columnNum] & 0x00010000) >> 14) | ((rawRow[columnNum] & 0x00000100) >> 7) | (rawRow[columnNum] & 0x00000001);
        tile[rowNum*8 + columnNum] = palette[offset];
      }
    }
    tiles[tileNum] = tile;
  }

  // print some tiles to screen for good measure
  // this is in the tilemap file but we'll handle that tomorrow
  printTile(pixels, tiles[66], 9*8,  2*8);
  printTile(pixels, tiles[67], 10*8, 2*8);
  printTile(pixels, tiles[68], 11*8, 2*8);
  printTile(pixels, tiles[69], 13*8, 2*8);
  printTile(pixels, tiles[70], 14*8, 2*8);
  printTile(pixels, tiles[71], 15*8, 2*8);
  printTile(pixels, tiles[72], 17*8, 2*8);

  printTile(pixels, tiles[73], 8*8,  3*8);
  printTile(pixels, tiles[74], 9*8,  3*8);
  printTile(pixels, tiles[75], 10*8, 3*8);
  printTile(pixels, tiles[76], 11*8, 3*8);
  printTile(pixels, tiles[77], 12*8, 3*8);
  printTile(pixels, tiles[78], 13*8, 3*8);
  printTile(pixels, tiles[79], 14*8, 3*8);
  printTile(pixels, tiles[80], 15*8, 3*8);
  printTile(pixels, tiles[81], 16*8, 3*8);
  printTile(pixels, tiles[82], 17*8, 3*8);
  printTile(pixels, tiles[83], 18*8, 3*8);

  printTile(pixels, tiles[84], 8*8,  4*8);
  printTile(pixels, tiles[85], 9*8,  4*8);
  printTile(pixels, tiles[86], 10*8, 4*8);
  printTile(pixels, tiles[87], 11*8, 4*8);
  printTile(pixels, tiles[88], 12*8, 4*8);
  printTile(pixels, tiles[89], 13*8, 4*8);
  printTile(pixels, tiles[90], 14*8, 4*8);
  printTile(pixels, tiles[91], 15*8, 4*8);
  printTile(pixels, tiles[92], 16*8, 4*8);
  printTile(pixels, tiles[93], 17*8, 4*8);
  printTile(pixels, tiles[94], 18*8, 4*8);

  printTile(pixels, tiles[95],  8*8,  5*8);
  printTile(pixels, tiles[96],  9*8,  5*8);
  printTile(pixels, tiles[97],  10*8, 5*8);
  printTile(pixels, tiles[98],  11*8, 5*8);
  printTile(pixels, tiles[99],  12*8, 5*8);
  printTile(pixels, tiles[100], 13*8, 5*8);
  printTile(pixels, tiles[101], 14*8, 5*8);
  printTile(pixels, tiles[102], 15*8, 5*8);
  printTile(pixels, tiles[103], 16*8, 5*8);
  printTile(pixels, tiles[104], 17*8, 5*8);

  // ... you get the idea

  SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  printf("done\n");

  return 0;
}

