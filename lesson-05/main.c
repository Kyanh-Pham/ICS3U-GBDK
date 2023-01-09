// Copyright (c) 2023 Kyanh Pham
//
// Created by: Kyanh Pham
// Created on: Jan 2023
// This program is the Space Aliens game for the Gameboy

#include <gb/gb.h>
#include <stdio.h>

#include "GBDK_Constants.h"
#include "MetaSprites.c"
#include "SpaceAliens.c"
#include "Background.c"

void main(){
    // keep track of meta sprite position, in a variable
    const int FIXED_Y_POSITION_OF_SHIP = 144;
    int shipXPosition = 76;
    int shipYPosition = FIXED_Y_POSITION_OF_SHIP;

    // load spritesheet referenced as #0
    // load 16 sprites from it
    // from variable SpaceAliens
    set_sprite_data(0, 16, SpaceAliens);

    // load sprites for meta sprite
    set_meta_sprite_tile(0, 1, 2, 3, 4);

    // move meta sprite on screen
    move_meta_sprite(0, shipXPosition, shipYPosition);

    // set background tile sheet to the same tile sheet
    set_bkg_data(0, 16, SpaceAliens);
    // load tiles from (0,0) to (20, 36) from the tile map called BackgroundMap
    set_bkg_tiles(0, 0, 20, 36, Background);

    SHOW_BKG; // turn Background on
    SHOW_SPRITES;
    DISPLAY_ON;

    // game loop
    while (1) {
        // get D-pad input
       switch (joypad()) {
        case J_LEFT:
            // move sprite in the index 0 by (x,y) (-1,0)
            scroll_meta_sprite(0, -1, 0);
            shipXPosition -= 1;
            if (shipXPosition < 8) {
                shipXPosition = 8;
                move_meta_sprite(0, 8, FIXED_Y_POSITION_OF_SHIP);
            }
            break;
        case J_RIGHT:
            // move sprite in the index 0 by (x,y) (+1, 0)
            scroll_meta_sprite(0, 1, 0);
            shipXPosition += 1;
            if (shipXPosition > SCREEN_WIDTH - 8) {
                shipXPosition = SCREEN_WIDTH - 8;
                move_meta_sprite(0, (uint8_t)(SCREEN_WIDTH - 8), FIXED_Y_POSITION_OF_SHIP);
            }
            break;
        default:
            break;
        }
        // scroll background 0 in x and 1 in the Y
        scroll_bkg(0, -1);

        // wait until end of frame (1/60th second)
        wait_vbl_done();
    }
}
