#include "gba.h"	//GBA register definitions
#include "keypad.h"     //button registers
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "pacman.h"       //my generic sprite header file
#include "pacsprite.h"  //pacman sprite image data and palette

//create an OAM variable and make it point to the address of OAM
u16* OAM = (u16*)0x7000000;

//create the array of sprites (128 is the maximum)
OAMEntry sprites[128];

//create the rotation and scaling array (overlaps the OAMEntry array memory)
pRotData rotData = (pRotData)sprites;

//Copy our sprite array to OAM
void CopyOAM()
{
	u16 loop;
	u16* temp;
	temp = (u16*)sprites;
	for(loop = 0; loop < 128*4; loop++)
	{
		OAM[loop] = temp[loop];
	}
}

//Set sprites to off screen
void InitializeSprites()
{
	u16 loop;
	for(loop = 0; loop < 128; loop++)
	{
		sprites[loop].attribute0 = 160;  //y to > 159
		sprites[loop].attribute1 = 240;  //x to > 239
	}
}

//wait for the screen to stop drawing
void WaitForVsync()
{
	while((volatile u16)REG_VCOUNT != 160){}
}


int main()
{
	u16 loop;       //generic loop variable
    s16 x = 100;	//variables to hold position of sprite on screen
	s16 y = 60;

    SetMode(MODE_1 | OBJ_ENABLE | OBJ_MAP_1D); //set mode 1 and enable sprites and 1d mapping

	for(loop = 0; loop < 256; loop++)          //load the palette into memory
		OBJPaletteMem[loop] = pacspritePalette[loop];

	InitializeSprites();                       //set all sprites off screen (stops artifact)

       	sprites[0].attribute0 = COLOR_256 | SQUARE | y;	//setup sprite info, 256 colour, shape and y-coord
        sprites[0].attribute1 = SIZE_16 | x;            //size 16x16 and x-coord
        sprites[0].attribute2 = 0;                      //pointer to tile where sprite starts

        for(loop = 0; loop < 128; loop++)               //load sprite image data
       	{
       		OAMData[loop] = pacspriteData[loop];
       	}

	while(1)                                //main loop
        {
		WaitForVsync();			//waits for the screen to stop drawing
		CopyOAM();			//Copies sprite array into OAM.
	}
}

