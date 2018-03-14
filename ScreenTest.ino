#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"
/*
#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif
*/

class CHARACTER
{
  public:
  LED(int type,int posX,int posY,int cDirection,int room,int health,bool cStaus);
  int _health;
  int _posX;
  int _posY;
  int _room;
  bool _cStatus;
  
  private:
  int _type;
};

CHARACTER::CHARACTER(int type,int posX,int posY,int room,int health,bool cStaus)
{
  _type = type;
  _posX = posX;
  _posY = posY;
  _room = room;
  _health = health;
  _cStatus = cStatus;
}

#define _sclk 52
#define _miso 50
#define _mosi 51
#define _cs 41
#define _dc 40
#define _rst 45

int movement = 1;
int spacing = 16;
int wMap[3][3][15][15] = {};
int inputPin = 22;

Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _mosi, _sclk, _rst, _miso);
void setup() 
{

  pinMode(inputPin, INPUT);
  
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
  static int charPosition[20/*Number of in game characters*/][4/*Character type, alive or dead,x pos,y pos*/];
  faceDirection(charPosition);
  //Inventory
  //If(attack or move)- Method
  deleteChunck(movement,movement);
  drawHero(movement,movement,0);
  delay(200);
  deleteChunck(movement,movement);
  drawHero(movement,movement,1);
  delay(200);
  deleteChunck(movement,movement);
  drawHero(movement,movement,2);
  delay(200);
  deleteChunck(movement,movement);
  drawHero(movement,movement,3);
  delay(200);
  if(digitalRead(inputPin))
  {
    deleteChunck(movement,movement);
    movement++;
  }
}

void faceDirection(int charPosition[][4])
{
  int dPad[4] = {};
  dPad[0] = 0; //Button input
  dPad[1] = 0; //Button input
  dPad[2] = 0; //Button input
  dPad[3] = 0; //Button input
  for(int i = 0;i<4;i++)
  {
    if(dPad[i] == 1)
    {
      drawHero(charPosition[0][2],charPosition[0][3],i);
      //directionFacing = i;
    }
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
    //Draw Hat
    tft.drawFastHLine(x*spacing+5,y*spacing,11,0x1CB0);
    tft.drawFastHLine(x*spacing+4,y*spacing+1,11,0x1CB0);
    tft.drawFastHLine(x*spacing+3,y*spacing+2,11,0x1CB0);
    tft.drawFastHLine(x*spacing+10,y*spacing+3,4,0x1CB0);
    tft.drawFastHLine(x*spacing+11,y*spacing+4,3,0x1CB0);
    tft.drawFastHLine(x*spacing+12,y*spacing+5,2,0x1CB0);
    //Draw Skin
    tft.drawFastHLine(x*spacing+3,y*spacing+3,7,0xEEEF);
    tft.drawFastVLine(x*spacing+10,y*spacing+4,2,0xEEEF);
    tft.drawFastVLine(x*spacing+3,y*spacing+4,3,0xEEEF);
    tft.fillRect(x*spacing+6,y*spacing+4,2,2,0xEEEF);
    tft.fillRect(x*spacing+4,y*spacing+6,10,2,0xEEEF);
    tft.drawPixel(x*spacing+11,y*spacing+5,0xEEEF);
    tft.fillRect(x*spacing,y*spacing+8,2,2,0xEEEF);
    tft.fillRect(x*spacing+14,y*spacing+10,2,2,0xEEEF);
    //Draw Tunic
    tft.fillRect(x*spacing+2,y*spacing+8,14,2,0x1CB0);
    tft.fillRect(x*spacing+4,y*spacing+12,10,2,0x1CB0);
    //Draw Belt
    tft.fillRect(x*spacing+4,y*spacing+10,10,2,0xDD00);
    tft.fillRect(x*spacing+6,y*spacing+10,2,2,ILI9340_YELLOW);
    
    //Draw Sword
    tft.drawFastHLine(x*spacing,y*spacing+7,2,0xA000);
    tft.fillRect(x*spacing,y*spacing+10,2,2,0xA000);
    tft.fillRect(x*spacing,y*spacing+2,2,7,0xDEDB);
    //Draw Boots
    tft.drawFastHLine(x*spacing+4,y*spacing+14,2,0xA3C0);
    tft.drawFastHLine(x*spacing+3,y*spacing+15,3,0xA3C0);
    tft.drawFastHLine(x*spacing+12,y*spacing+14,2,0xA3C0);
    tft.drawFastHLine(x*spacing+11,y*spacing+15,3,0xA3C0);
  //Hero facing up  
  }else if(heroDirection == 1)
  {
    tft.drawFastHLine(x*spacing+5,y*spacing,6,0x1CB0);
    tft.drawFastHLine(x*spacing+4,y*spacing+1,8,0x1CB0);
    tft.fillRect(x*spacing+3,y*spacing+2,10,4,0x1CB0);
    tft.drawFastHLine(x*spacing+6,y*spacing+6,4,0x1CB0);
    tft.drawFastHLine(x*spacing+7,y*spacing+7,2,0x1CB0);
    //Draw Skin
    tft.drawFastHLine(x*spacing+4,y*spacing+6,2,0xEEEF);
    tft.drawFastHLine(x*spacing+4,y*spacing+7,3,0xEEEF);
    tft.drawFastHLine(x*spacing+10,y*spacing+6,2,0xEEEF);
    tft.drawFastHLine(x*spacing+9,y*spacing+7,3,0xEEEF);
    tft.drawFastVLine(x*spacing+1,y*spacing+9,2,0xEEEF);
    tft.fillRect(x*spacing+12,y*spacing+10,2,2,0xEEEF);
    //Draw Tunic
    tft.fillRect(x*spacing+3,y*spacing+8,11,2,0x1CB0);
    tft.fillRect(x*spacing+2,y*spacing+9,2,2,0x1CB0);
    tft.fillRect(x*spacing+4,y*spacing+12,8,2,0x1CB0);
    //Draw Belt
    tft.fillRect(x*spacing+4,y*spacing+10,8,2,0xDD00);
    //Draw Sword
    tft.drawFastHLine(x*spacing+1,y*spacing+8,2,0xA000);
    tft.fillRect(x*spacing+1,y*spacing+11,2,2,0xA000);
    tft.fillRect(x*spacing+1,y*spacing+1,2,7,0xDEDB);
    //Draw Boots
    tft.fillRect(x*spacing+4,y*spacing+14,2,2,0xA3C0);
    tft.fillRect(x*spacing+10,y*spacing+14,2,2,0xA3C0);
    
  //Hero facing right  
  }else if(heroDirection == 2)
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
    tft.drawFastHLine(x*spacing+14,y*spacing+7,2,0xA000);
    tft.fillRect(x*spacing+14,y*spacing+10,2,2,0xA000);
    tft.fillRect(x*spacing+14,y*spacing+2,2,7,0xDEDB);
    //Draw Boots
    tft.drawFastHLine(x*spacing+2,y*spacing+14,2,0xA3C0);
    tft.drawFastHLine(x*spacing+2,y*spacing+15,3,0xA3C0);
    tft.drawFastHLine(x*spacing+10,y*spacing+14,2,0xA3C0);
    tft.drawFastHLine(x*spacing+10,y*spacing+15,3,0xA3C0); 
    
  //Hero facing down  
  }else
  {
    //Draw Hat
    tft.drawFastHLine(x*spacing+5,y*spacing,6,0x1CB0);
    tft.drawFastHLine(x*spacing+4,y*spacing+1,8,0x1CB0);
    tft.drawFastHLine(x*spacing+5,y*spacing+2,6,0x1CB0);
    tft.fillRect(x*spacing+3,y*spacing+2,2,2,0x1CB0);
    tft.fillRect(x*spacing+11,y*spacing+2,2,2,0x1CB0);
    tft.drawFastVLine(x*spacing+3,y*spacing+4,2,0x1CB0);
    tft.drawFastVLine(x*spacing+12,y*spacing+4,2,0x1CB0);
    //Draw Skin
    tft.drawFastHLine(x*spacing+5,y*spacing+3,6,0xEEEF);
    tft.drawFastVLine(x*spacing+4,y*spacing+4,2,0xEEEF);
    tft.drawFastVLine(x*spacing+11,y*spacing+4,2,0xEEEF);
    tft.fillRect(x*spacing+7,y*spacing+4,2,2,0xEEEF);
    tft.fillRect(x*spacing+4,y*spacing+6,8,2,0xEEEF);
    tft.fillRect(x*spacing+2,y*spacing+10,2,2,0xEEEF);
    tft.fillRect(x*spacing+13,y*spacing+9,2,2,0xEEEF);
    //Draw Tunic
    tft.fillRect(x*spacing+2,y*spacing+8,11,2,0x1CB0);
    tft.fillRect(x*spacing+4,y*spacing+12,8,2,0x1CB0);
    //Draw Belt
    tft.fillRect(x*spacing+4,y*spacing+10,8,2,0xDD00);
    tft.fillRect(x*spacing+7,y*spacing+10,2,2,ILI9340_YELLOW);
    
    //Draw Sword
    tft.drawFastHLine(x*spacing+13,y*spacing+8,2,0xA000);
    tft.fillRect(x*spacing+13,y*spacing+11,2,2,0xA000);
    tft.fillRect(x*spacing+13,y*spacing+1,2,7,0xDEDB);
    //Draw Boots
    tft.fillRect(x*spacing+4,y*spacing+14,2,2,0xA3C0);
    tft.fillRect(x*spacing+10,y*spacing+14,2,2,0xA3C0);
  }
}
