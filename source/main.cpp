#include <nds.h>

#include "gfx_ball.h"
#include "gfx_brick.h"
#include "gfx_gradient.h"

// Tile constants
const int TILE_EMPTY = 0; // let tile 0 be empty
const int TILE_BRICK = 1; // let tile 1 be brick
const int TILE_GRADIENT = 2; // let tile 2 be start of gradient

// Bg Palettes
const int PAL_BRICKS = 0;
const int PAL_GRADIENT = 1;
const u16 BACKDROP_COLOUR = RGB8(190, 255, 255); // 190, 255, 255

// Gets memory addr of the nth tile in gfx bank
u16* tileToBgRam(int tile)
{
    return BG_GFX + tile*16;
}

// Gets memory addr of the nth palette in palette bank
u16* palToBgRam(int palette)
{
    return BG_PALETTE + palette*16;
}

// Stores our 'image' data into the banks to be retrieved later
void setupGraphics()
{
    vramSetBankE(VRAM_E_MAIN_BG);
    vramSetBankF(VRAM_F_MAIN_SPRITE);
    
    // Generate the first blank tile by clearing it to zero
    for(int n = 0; n < 16; n++)
    {   
        BG_GFX[n] = 0;
    }
    
    // Copy bg graphics
    // (DMA channel selection, source addr, destination addr, how much data to copy)
    dmaCopyHalfWords(3, gfx_brickTiles, tileToBgRam(TILE_BRICK), gfx_brickTilesLen);
    dmaCopyHalfWords(3, gfx_gradientTiles, tileToBgRam(TILE_GRADIENT), gfx_gradientTilesLen);

    // Palettes assigned to palette memory
    dmaCopyHalfWords(3, gfx_brickPal, palToBgRam(PAL_BRICKS), gfx_brickPalLen);
    dmaCopyHalfWords(3, gfx_gradientPal, palToBgRam(PAL_GRADIENT), gfx_gradientPalLen);

    // Set backdrop color
    BG_PALETTE[0] = BACKDROP_COLOUR;
}

int main(void) 
{
    irqInit();
    irqEnable(IRQ_VBLANK);

    setupGraphics();

    while (true)
    {
        swiWaitForVBlank();
        videoSetMode(MODE_0_2D);
    }    

	return 0;
}
