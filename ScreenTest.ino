#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define _sclk 52
#define _miso 50
#define _mosi 51
#define _cs 41
#define _dc 40
#define _rst 45

int movement = 1;
int spacing = 16;
int wMap[3][3][15][15] = {};

Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _mosi, _sclk, _rst, _miso);
void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Adafruit 2.2\" SPI TFT Test!");
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9340_BLACK);
  initializeMap();
  drawMap();
}
void loop() 
{
  drawHero(movement,movement,3);
  drawHero(14-movement,14-movement,3);
  drawHero(movement,14-movement,3);
  drawHero(14-movement,movement,3);
  drawHero(7,movement,3);
  drawHero(7,14-movement,3);
  drawHero(movement,7,3);
  drawHero(14-movement,7,3);
  delay(500);
  deleteChunck(movement,movement);
  deleteChunck(14-movement,14-movement);
  deleteChunck(movement,14-movement);
  deleteChunck(14-movement,movement);
  deleteChunck(7,movement);
  deleteChunck(7,14-movement);
  deleteChunck(movement,7);
  deleteChunck(14-movement,7);
  
  
  movement+=1;
  if(movement > 13)
    movement = 2;
  
}

void drawHero(int x,int y,int heroDirection)
{
  //Colours, BLACK,BLUE,RED,GREEN,CYAN,MAGENTA,YELLOW,WHITE
  //tft.drawFastVLine(x,y,length, colour)
  //tft.drawFastHLine(x,y,length, colour)
  //tft.drawPixel(x*10,y*10,ILI9340_BLACK)
  //tft.drawRect(x,y,width,height,colour)
  heroDirection%=4;
  //Hero facing left
  if(heroDirection == 0)
  {
    tft.fillRect(x*spacing,y*spacing,15,15,ILI9340_RED);
  //Hero facing up  
  }else if(heroDirection == 1)
  {
    tft.fillRect(x*spacing,y*spacing,15,15,ILI9340_BLUE);
  //Hero facing right  
  }else if(heroDirection == 2)
  {
    tft.fillRect(x*spacing,y*spacing,15,15,ILI9340_YELLOW);
  //Hero facing down  
  }else
  {
    //Draw Hat
    tft.drawFastHLine(x*spacing,y*spacing,11,0x1CB0);
    tft.drawFastHLine(x*spacing+1,y*spacing+1,11,0x1CB0);
    tft.drawFastHLine(x*spacing+2,y*spacing+2,11,0x1CB0);
    tft.drawFastHLine(x*spacing+2,y*spacing+3,4,0x1CB0);
    tft.drawFastHLine(x*spacing+2,y*spacing+4,3,0x1CB0);
    tft.drawFastHLine(x*spacing+2,y*spacing+5,2,0x1CB0);
    //Draw Skin
    tft.drawFastHLine(x*spacing+6,y*spacing+3,7,0xEEEF);
    tft.drawFastVLine(x*spacing+5,y*spacing+4,4,0xEEEF);
    tft.drawFastVLine(x*spacing+12,y*spacing+4,3,0xEEEF);
    tft.fillRect(x*spacing+8,y*spacing+4,2,2,0xEEEF);
    tft.fillRect(x*spacing+2,y*spacing+6,10,2,0xEEEF);
    tft.drawPixel(x*spacing+4,y*spacing+5,0xEEEF);
    tft.fillRect(x*spacing,y*spacing+10,2,2,0xEEEF);
    tft.fillRect(x*spacing+14,y*spacing+8,2,2,0xEEEF);
    //Draw Tunic
    tft.fillRect(x*spacing,y*spacing+8,14,2,0x1CB0);
    tft.fillRect(x*spacing+2,y*spacing+10,10,1,0x1CB0);
    tft.fillRect(x*spacing+2,y*spacing+12,10,2,0x1CB0);
    //Draw Belt
    tft.fillRect(x*spacing+2,y*spacing+10,10,2,0xDD00);
    tft.fillRect(x*spacing+8,y*spacing+10,2,2,ILI9340_YELLOW);
    
    //Draw Sword
    tft.drawFastHLine(x*spacing+14,y*spacing+7,2,ILI9340_BLUE);
    tft.fillRect(x*spacing+14,y*spacing+10,2,2,ILI9340_BLUE);
    tft.fillRect(x*spacing+14,y*spacing+2,2,7,0xDEDB);
    //Draw Boots
    tft.drawFastHLine(x*spacing+2,y*spacing+14,2,0xA3C0);
    tft.drawFastHLine(x*spacing+2,y*spacing+15,3,0xA3C0);
    tft.drawFastHLine(x*spacing+10,y*spacing+14,2,0xA3C0);
    tft.drawFastHLine(x*spacing+10,y*spacing+15,3,0xA3C0); 
  }
}

void deleteChunck(int x,int y)
{
  tft.fillRect(x*spacing,y*spacing,16,16,ILI9340_BLACK);
}

void initializeMap()
{
  for(int i = 0;i<15;i++)
  {
    wMap[0][0][0][i] = 1;
    wMap[0][0][14][i] = 1;
    wMap[0][0][i][0] = 1;
    wMap[0][0][i][14] = 1;
  }
}

void drawMap()
{
  for(int i = 0;i<15;i++)
  {
    for(int j = 0;j<15;j++)
    {
      drawTile(i*spacing,j*spacing,wMap[0][0][i][j]);
    }
  }
}

void drawTile(int xPos,int yPos,int tileType)
{
  if(tileType == 1)
  {
    tft.fillRect(xPos,yPos,spacing,spacing,ILI9340_WHITE);
  }
}

