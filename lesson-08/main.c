// Copyright (c) 2023 Kyanh Pham
//
// Created by: Kyanh Pham
// Created on: Jan 2023
// This program is the Space Aliens game for the Gameboy

#include <gb/gb.h>
#include <stdio.h>
#include <stdbool.h>
#include<stdint.h>

#include "GBDK_Constants.h"
#include "MetaSprites.c"
#include "SpaceAliens.c"
#include "SpaceAliensData.c"
#include "SpaceAliensMap.c"
#include "Background.c"

typedef enum {
    SPLASH,
    START,
    GAME,
    GAME_OVER
} screen_t;

// splash scene
screen_t splash() {
    // user input
    uint8_t joypadData;
    screen_t next_screen = GAME;

    // time
    unsigned int timeCounter =0;

    printf("\n\n\n\n\n\n");
    printf("   MT Game Studios");
    printf("        Presents");
    printf("\n\n\n\n\n\n");
    printf("    Space Aliens");

    DISPLAY_ON;

    // game loop
    while(1) {
        // get D-pad input
        joypadData = joypad();

        // If A button is pressed
        if (joypadData & J_START) {
            return next_screen;
        }

        // move even if start is not pressed after 2 seconds
        timeCounter++;
        if (timeCounter > (3 * 60)) {
            return next_screen;
        }

        // wait until end of frame (1/60th second)
        wait_vbl_done();
    }
}


// game scene
screen_t game() {
        //user input
    uint8_t joypadData;

    // check if A button is single pressed
    bool aButtonJustPressed = false;
    bool aButtonStillPressed = false;

    // these registers must be in this specific order!
    NR52_REG = 0x80; // is 1000 0000 in binary and turns on sound
    NR50_REG = 0x77; // sets the volume for both left and right channel just set to max 0x77
    NR51_REG = 0xFF; // is 1111 1111 in binary, select which channels we want

    // keep track of meta sprite position, in a variable
    const int FIXED_Y_POSITION_OF_SHIP = 144;
    int shipXPosition = 76;
    int shipYPosition = FIXED_Y_POSITION_OF_SHIP;

    // load spritesheet referenced as #0
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
       joypadData = joypad();

        // If LEFT D-pad is pressed
        if (joypadData & J_LEFT) {
            // move sprite in the index 0 by (x,y) (-1,0)
            scroll_meta_sprite(0, -1, 0);
            shipXPosition -= 1;
            if (shipXPosition < 8) {
                shipXPosition = 8;
                move_meta_sprite(0, 8, FIXED_Y_POSITION_OF_SHIP);
            }
        }

        // If RIGHT D-pad is pressed
        if (joypadData & J_RIGHT) {
            // move sprite in the index 0 by (x,y) (+1, 0)
            scroll_meta_sprite(0, 1, 0);
            shipXPosition += 1;
            if (shipXPosition > SCREEN_WIDTH - 8) {
                shipXPosition = SCREEN_WIDTH - 8;
                move_meta_sprite(0, (uint8_t)(SCREEN_WIDTH - 8), FIXED_Y_POSITION_OF_SHIP);
            }
        }

        // If A button is pressed
        if (joypadData & J_A) {
            if (aButtonJustPressed == true) {
                aButtonStillPressed = true;
            }
            else {
                aButtonJustPressed = true;
                // play a sound
                NR10_REG = 0x16;
                NR11_REG = 0x82;
                NR12_REG = 0x69;
                NR13_REG = 0x59;
                NR14_REG = 0xC6;
            }
        }
        else {
            aButtonJustPressed = false;
            aButtonStillPressed = false;
        }

        // scroll background 0 in x and 1 in the Y
        scroll_bkg(0, -1);

        // wait until end of frame (1/60th second)
        wait_vbl_done();
    }
}


void main(){
    // Current screen, defaults to TITLE
    // from here : link
    screen_t current_screen = SPLASH;

    // these registers must be in this specific order
    NR52_REG = 0x80; // is 1000 0000 in binary and turns on sound
    NR50_REG = 0x77; // sets the volume for both left and right channel just set to max 0x77
    NR51_REG = 0xFF; // is 1111 1111 in binary, select which channels we want

    while(1) {
        if (current_screen == SPLASH) {
            current_screen = splash();
        }
        else if (current_screen == START) {
            printf("start");
            // current_screen = start();
        }
        else if (current_screen == GAME) {
            current_screen = game();
        }
        else if (current_screen == GAME_OVER) {
            printf("game_over");
            // current_screen = game_over();
        }
    }
}
