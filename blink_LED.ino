#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"

#define _sclk 52
#define _miso 50
#define _mosi 51
#define _cs 41
#define _dc 40
#define _rst 45

Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _mosi, _sclk, _rst, _miso);

//Character class for Hero and Enemies
class CHARACTER {
  public:
  int health;
  int posX;
  int posY;
  int cDirection;
  int row;
  int col;
  bool cStatus;
  int type;
  int cTile;
  void properties(int _type, int _posX, int _posY, int _cDirection, int _row, int _col, int _health, bool _cStatus, int _cTile) {
    type = _type;
    posX = _posX;
    posY = _posY;
    cDirection = _cDirection;
    row = _row;
    col = _col;
    health = _health;
    cStatus = _cStatus;
    cTile = _cTile;
  }
};

class ROOM
{
  public:
  int enemies;
  int switches;
  int centres[3][2];
  void properties(int _enemies, int _switches, int _centre1X, int _centre1Y, int _centre2X, int _centre2Y, int _centre3X, int _centre3Y) {
    enemies = _enemies;
    switches = _switches;
    centres[0][0] = _centre1X;
    centres[0][1] = _centre1Y;
    centres[1][0] = _centre2X;
    centres[1][0] = _centre2Y;
    centres[2][0] = _centre3X;
    centres[2][0] = _centre3Y;
  }
};

int wMap[3][3][15][15] = {{{{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}, //L Room 3
                            {10,14,14,14,14,14,14,14,14,14,14,14,14,14,10},
                            {10,14,14,14,14,0,0,0,0,0,0,14,14,14,10},
                            {10,14,14,0,0,0,0,0,0,0,0,0,14,14,10},
                            {10,14,0,0,0,0,0,0,0,0,0,0,14,14,10},
                            {10,14,0,0,0,0,0,0,0,0,0,0,14,14,10},
                            {10,14,0,0,0,0,0,0,0,0,0,0,0,14,10},
                            {10,14,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {10,14,0,0,0,0,0,0,0,0,0,0,0,14,10},
                            {10,14,0,0,0,0,0,0,0,0,0,0,14,14,10},
                            {10,14,0,0,0,0,0,0,0,0,0,0,14,14,10},
                            {10,14,14,0,0,0,0,0,0,0,0,0,14,14,10},
                            {10,14,14,14,14,0,0,0,0,0,0,14,14,14,10},
                            {10,14,14,14,14,14,14,14,14,14,14,14,14,14,10},
                            {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}},
                         
                           {{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}, //T Room 3
                            {10,16,16,16,16,16,16,16,16,16,16,16,16,16,10},
                            {10,16,2,2,2,2,2,2,2,2,2,2,2,16,10},
                            {10,16,2,2,2,2,2,2,2,2,2,2,2,16,10},
                            {10,16,2,2,2,16,16,2,2,16,16,2,2,16,10},
                            {10,16,2,2,2,16,16,2,2,16,16,2,2,16,10},
                            {60,63,66,2,2,2,2,2,2,2,2,2,2,2,2},
                            {61,64,67,2,2,2,2,2,2,2,2,2,2,2,2},
                            {62,65,68,2,2,2,2,2,2,2,2,2,2,2,2},
                            {10,16,2,2,2,16,16,2,2,16,16,2,2,16,10},
                            {10,16,2,2,2,16,16,2,2,16,16,2,2,16,10},
                            {10,16,2,2,2,2,2,2,2,2,2,2,2,16,10},
                            {10,16,2,2,2,2,2,2,2,2,2,2,2,16,10},
                            {10,16,16,16,16,16,16,16,16,16,16,16,16,16,10},
                            {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}},
                            
                           {{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}, //D Room 3
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,3,0,3,0,0,0,0,0,3,0,3,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,10,3,0,3,11,0,0,0,0,10},
                            {10,0,0,0,10,10,0,0,0,11,11,0,0,0,10},
                            {10,0,0,10,10,10,0,0,0,11,11,11,0,0,10},
                            {10,0,0,10,10,10,0,0,3,11,11,11,0,0,0},
                            {10,0,0,10,10,10,0,0,0,11,11,11,0,0,10},
                            {10,0,0,0,10,10,0,0,0,11,11,0,0,0,10},
                            {10,0,0,0,0,10,3,0,3,11,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,3,0,3,0,0,0,0,0,3,0,3,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}}},

                          {{{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}, //L Room 2
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,14,14,14,14,14,14,14,0,0,10},
                            {10,0,0,0,0,1,14,14,14,14,14,14,14,0,10},
                            {10,0,0,14,14,1,1,14,10,14,14,14,14,0,10},
                            {10,0,14,14,14,14,1,0,10,10,14,14,14,0,10},
                            {10,14,14,14,14,14,10,0,0,10,0,14,14,0,10},
                            {0,0,1,1,1,1,0,0,0,10,7,1,1,0,0},
                            {10,14,14,14,14,14,10,0,0,10,0,14,14,0,10},
                            {10,0,14,14,14,14,1,0,10,10,14,14,14,0,10},
                            {10,0,0,14,14,1,1,14,10,14,14,14,14,0,10},
                            {10,0,0,0,0,1,14,14,14,14,14,14,14,0,10},
                            {10,0,0,0,0,14,14,14,14,14,14,14,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}},
                            
                           {{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}, //T Room 2
                            {10,15,15,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,15,15,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,15,15,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,15,15,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,10,5,5,15,15,15,15,15,15,15,15,15,10,10},
                            {2,2,1,1,1,1,1,1,1,1,1,1,1,0,10},
                            {2,2,1,1,1,1,1,1,1,1,1,1,1,0,0},
                            {2,2,1,1,1,1,1,1,1,1,1,1,1,0,10},
                            {10,10,5,5,15,15,15,15,15,15,15,15,15,10,10},
                            {10,15,15,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,15,15,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,15,15,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,15,15,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}},

                           {{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}, //D Room 2
                            {10,0,0,12,12,12,12,12,12,12,12,12,0,0,10},
                            {10,0,0,0,12,12,12,12,12,12,12,0,0,0,10},
                            {10,0,0,0,0,12,12,12,12,12,0,0,0,0,10},
                            {10,0,3,0,0,0,12,12,12,0,0,0,3,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {0,0,0,0,0,0,3,0,3,0,0,0,0,0,0},
                            {10,0,0,0,11,0,0,0,0,0,10,0,0,0,10},
                            {10,0,0,11,11,11,0,0,0,10,10,10,0,0,10},
                            {10,0,0,0,11,0,0,0,0,0,10,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,3,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}}},

                          {{{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}, //L Room 1
                            {10,14,14,14,0,0,0,0,0,0,0,14,14,14,10},
                            {10,14,14,0,0,0,0,0,0,0,0,0,14,14,10},
                            {10,14,0,0,0,0,0,0,0,0,0,0,0,14,10},
                            {10,0,0,0,10,0,0,0,0,0,10,0,0,0,10},
                            {10,0,0,0,0,0,0,10,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,14,14,14,0,0,0,0,0,10},
                            {0,0,0,0,0,10,14,14,14,10,0,0,0,0,10},
                            {10,0,0,0,0,0,14,14,14,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,10,0,0,0,0,0,0,10},
                            {10,0,0,0,10,0,0,0,0,0,10,0,0,0,10},
                            {10,14,0,0,0,0,0,0,0,0,0,0,0,14,10},
                            {10,14,14,0,0,0,0,0,0,0,0,0,14,14,10},
                            {10,14,14,14,0,0,0,0,0,0,0,14,14,14,10},
                            {10,10,10,10,10,10,10,0,10,10,10,10,10,10,10}},

                           {{10,10,10,10,10,10,10,0,10,10,10,10,10,10,10}, //T Room 1
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,10,10,10,10,10,10,0,10,10,10,10,10,10,10}},

                           {{10,10,10,10,10,10,10,0,10,10,10,10,10,10,10}, //D Room 1
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,3,0,0,0,0,0,0,0,3,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,10,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,10,10,10,0,0,0,0,0,10},
                            {0,0,0,3,0,10,10,10,10,10,0,3,0,0,10},
                            {10,0,0,0,0,0,10,10,10,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,10,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,3,0,0,0,0,0,0,0,3,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}}}};
         
//Spacing sets the size of each block, 16x16 pixels
const int spacing = 16;
bool dPad[4];
//Initialize array of character objects
CHARACTER Characters[22];
// Initialize array of room objects
ROOM Rooms[3][3];
//Boolean statments to make sure the button press is only registered once
bool moveState;
bool lastMoveState = 1;
bool attackState;
bool lastAttackState = 1;
bool weaponState;
bool lastWeaponState = 1;
int wRow = 2;
int wCol = 1;
int weapon = 0;
const int ENEMYCOUNT = 21;
const int BREAKABLECOUNT = 13;
int bossHp = 20;
bool bossRoom = false;
int weapons = 1;

void setup() {
  randomSeed(analogRead(0));
  
  // Character properties: type,posX,posY,cDirection,row,col,health,cStatus,cTile
  // Character 0 is the Hero
  Characters[0].properties(0,2,2,3,2,1,24,1,0);
  // Characters of types 1 through 5 are enemies
  // Top left enemies
  Characters[1].properties(2,7,4,0,0,0,5,1,0);
  Characters[2].properties(1,4,8,0,0,0,3,1,0);
  Characters[3].properties(1,10,8,0,0,0,3,1,0);
  // Top right enemies
  Characters[4].properties(4,3,4,0,0,2,5,1,0);
  Characters[5].properties(3,11,3,0,0,2,3,1,0);
  Characters[6].properties(3,2,8,0,0,2,3,1,0);
  Characters[7].properties(4,12,8,0,0,2,5,1,0);
  // Middle left enemies
  Characters[8].properties(1,2,2,0,1,0,3,1,0);
  Characters[9].properties(2,12,2,0,1,0,5,1,0);
  // Middle right enemies
  Characters[10].properties(3,5,3,0,1,2,3,1,0);
  Characters[11].properties(4,12,4,0,1,2,5,1,0);
  Characters[12].properties(3,4,10,0,1,2,3,1,0);
  // Bottom left enemies
  Characters[13].properties(1,5,2,0,2,0,3,1,0);
  Characters[14].properties(1,9,2,0,2,0,3,1,0);
  Characters[15].properties(1,3,7,0,2,0,3,1,0);
  Characters[16].properties(1,7,10,0,2,0,3,1,0);
  // Bottom centre enemies
  Characters[17].properties(5,13,2,1,2,1,3,1,0);
  Characters[18].properties(5,2,13,3,2,1,3,1,0);
  // Bottom right enemies
  Characters[19].properties(3,5,3,0,2,2,3,1,0);
  Characters[20].properties(3,12,5,0,2,2,3,1,0);
  Characters[21].properties(3,9,12,0,2,2,3,1,0);
  // Characters of type 6 are pots
  // Bottom left pots
  Characters[22].properties(8,0,0,0,0,0,0,0,10);
  Characters[23].properties(6,1,6,0,2,0,1,1,0);
  Characters[24].properties(6,1,8,0,2,0,1,1,0);
  Characters[25].properties(6,6,13,0,2,0,1,1,0);
  Characters[26].properties(6,8,13,0,2,0,1,1,0);
  // Character7 of type 7 are switches
  // Top left switches
  Characters[29].properties(7,1,3,0,0,0,1,1,14);
  Characters[30].properties(7,13,3,0,0,0,1,1,14);
  Characters[31].properties(7,3,13,0,0,0,1,1,14);
  Characters[32].properties(7,11,13,0,0,0,1,1,14);
  // Top right switch
  Characters[33].properties(7,7,6,0,0,2,1,1,0);
  // Middle left switch
  Characters[34].properties(7,7,8,0,1,0,1,1,0);
  // Room properties: enemies, switches, centre1X, centre1Y, centre2X, centre2Y, centre3X, centre3Y
  // Top Rooms
  Rooms[0][0].properties(3,4,7,7,0,0,0,0);
  Rooms[0][1].properties(1,0,7,7,0,0,0,0);
  Rooms[0][2].properties(4,1,4,6,4,8,0,0);
  // Middle Rooms
  Rooms[1][0].properties(2,1,13,7,0,7,0,0);
  Rooms[1][1].properties(0,0,7,7,0,0,0,0);
  Rooms[1][2].properties(2,0,9,10,9,4,0,7);
  // Bottom Rooms
  Rooms[2][0].properties(4,0,7,7,0,0,0,0);
  Rooms[2][1].properties(4,0,7,7,0,0,0,0);
  Rooms[2][2].properties(3,0,7,7,0,0,0,0);
  
  //Initialize pins for d-pad and buttons
  for(int i = 22;i<29;i++){
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
  //Initialize the screen
  tft.begin();
  //Set the screen to landscape and clear it
  tft.setRotation(3);
  tft.fillScreen(ILI9340_BLACK);
  //initializeMap();
  drawMap();
  hearts();
  heroMove();
  enemiesMove();
  gameBorder();
}

void loop() {
  //Check current state of buttons
  moveState = digitalRead(26);
  attackState = digitalRead(27);
  weaponState = digitalRead(28);
  //Check if user wants to change character direction
  if(Characters[0].health > 0)
    faceDirection();
  //If the weapon button's state changes
  if ((weaponState && !lastWeaponState)&& Characters[0].health>0) {
    if (weapon != 2){
      weapon++;
      drawHero(Characters[0].posX,Characters[0].posY);
    }
    else {
      weapon = 0;
      drawHero(Characters[0].posX,Characters[0].posY);
    }
  }
  if (weapon == 0)
    drawTile(17*spacing, 5*spacing, 14);
  if (weapon == 1)
    drawTile(17*spacing, 5*spacing, 15);
  if (weapon == 2)
    drawTile(17*spacing, 5*spacing, 0);
  lastWeaponState = weaponState;
  //If the move button's state changes
  if((moveState && !lastMoveState) && Characters[0].health>0) {
    heroMove();
    enemiesMove();
    lastMoveState = moveState;
  }
  //If the button is in the same state, do nothing
  else{
    lastMoveState = moveState;
  }
  //Check if the attack button's state changes
  if((attackState && !lastAttackState) && Characters[0].health>0){
    heroAttack();
    enemiesMove();
    lastAttackState = attackState;
  }
  else{
    lastAttackState = attackState;
  }
}

//Place the character after a map change
void placeChar() {
  //Remove Hero from current spot
  drawTile(Characters[0].posX*spacing,Characters[0].posY*spacing,Characters[0].cTile);
  wMap[wRow][wCol][Characters[0].posX][Characters[0].posY] = Characters[0].cTile;
  //Check where he should be placed
   switch(Characters[0].cDirection) {
      case 0: Characters[0].posX = 13;
         Characters[0].posY = 7;
         wCol--;
         Characters[0].col--;
         break;
      case 1: Characters[0].posX = 7;
         Characters[0].posY = 13;
         wRow--;
         Characters[0].row--;
         break;
      case 2: Characters[0].posX = 1;
         Characters[0].posY = 7;
         wCol++;
         Characters[0].col++;
         break;
      case 3: Characters[0].posX = 7;
         Characters[0].posY = 1;
         wRow++;
         Characters[0].row++;
         break;
   }
   drawMap();
   gameBorder();
   hearts();
   placeBreakables();
   //Place the Hero on the map
   wMap[wRow][wCol][Characters[0].posX][Characters[0].posY] = 42;
   //Draw the hero
   drawHero(Characters[0].posX,Characters[0].posY);
}

void heroMove() {
  int newPos[2] = {};
  if(Characters[0].cDirection == 0) {
    newPos[0] = -1;
  }
  else if(Characters[0].cDirection == 1) {
    newPos[1] = -1;
  }
  else if(Characters[0].cDirection == 2) {
    newPos[0] = 1;
  }
  else {
    newPos[1] = 1;
  }
  if(wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] < 10 || (wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] >= 50 && wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] <= 59)) {
    drawTile(Characters[0].posX*spacing,Characters[0].posY*spacing,Characters[0].cTile);
    wMap[wRow][wCol][Characters[0].posX][Characters[0].posY] = Characters[0].cTile;
    if(wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] == 5) {
      Characters[0].health+=2;
      hearts();
    }
    else if (wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] == 7) {
      weapons++;
    }
    else {
      Characters[0].cTile = wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]];
      if(wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] == 3) {
        Characters[0].health-=1;
        hearts();
      }
    }
    wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] = 42;
    Characters[0].posX = Characters[0].posX + newPos[0];
    Characters[0].posY = Characters[0].posY + newPos[1];
    drawHero(Characters[0].posX,Characters[0].posY);
    if((Characters[0].row == 0 && Characters[0].col == 1) && bossRoom == 1) {
      bossMechanics(0);
    }
  }
  if(Characters[0].posX <= 0 || Characters[0].posX >= 14 || Characters[0].posY <= 0 || Characters[0].posY >= 14){
    placeChar();
  }
}

void heroAttack(){
  int attacks[2] = {};
  if(Characters[0].cDirection == 0)
    attacks[0] = -1;
  else if(Characters[0].cDirection == 1)
    attacks[1] = -1;
  else if(Characters[0].cDirection == 2)
    attacks[0] = 1;
  else
    attacks[1] = 1;
  if (weapon == 0) {
    if((Characters[0].row == 0 && Characters[0].col == 1) && bossRoom == 1) {
      bossMechanics(0);
    }
    if(wMap[wRow][wCol][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]] >= 100){
      int currentEnemy = wMap[wRow][wCol][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]]-100;
      Characters[currentEnemy].health -= 2;
      if(Characters[currentEnemy].health <= 0) {
        Characters[currentEnemy].cStatus = 0;
        if (Characters[currentEnemy].type < 6)
          Rooms[Characters[currentEnemy].row][Characters[currentEnemy].col].enemies--;
        if (Characters[currentEnemy].type == 7)
          Rooms[Characters[currentEnemy].row][Characters[currentEnemy].col].switches--;
        if(random(8) == 0 && Characters[currentEnemy].type < 7){
          Characters[currentEnemy].cTile = 5;
        }
        wMap[wRow][wCol][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]] = Characters[currentEnemy].cTile;
        drawTile(Characters[currentEnemy].posX*spacing,Characters[currentEnemy].posY*spacing,Characters[currentEnemy].cTile);
      }
    }
    else if(wMap[wRow][wCol][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]] >= 60 && wMap[wRow][wCol][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]] <= 68) {
      bossHp -= 2;
      if(bossHp <= 0){
        Characters[0].health = 0;
        tft.setCursor(58,116);
        tft.setTextSize(3);
        tft.setTextColor(0xFFFF);
        tft.print("YOU WON");
      }
      bossMechanics(1);
    }
  }
  else if (weapon == 1) {
    if((Characters[0].row == 0 && Characters[0].col == 1) && bossRoom == 1) {
      bossMechanics(0);
    }
    if(wMap[wRow][wCol][Characters[0].posX + 2*attacks[0]][Characters[0].posY + 2*attacks[1]] >= 100){
      int currentEnemy = wMap[wRow][wCol][Characters[0].posX + 2*attacks[0]][Characters[0].posY + 2*attacks[1]]-100;
      Characters[currentEnemy].health--;
      if(Characters[currentEnemy].health <= 0) {
        Characters[currentEnemy].cStatus = 0;
        if (Characters[currentEnemy].type < 6)
          Rooms[Characters[currentEnemy].row][Characters[currentEnemy].col].enemies--;
        if (Characters[currentEnemy].type == 7)
          Rooms[Characters[currentEnemy].row][Characters[currentEnemy].col].switches--;
        if(random(8) == 0 && Characters[currentEnemy].type < 7){
          Characters[currentEnemy].cTile = 5;
        }
        wMap[wRow][wCol][Characters[0].posX + 2*attacks[0]][Characters[0].posY + 2*attacks[1]] = Characters[currentEnemy].cTile;
        drawTile(Characters[currentEnemy].posX*spacing,Characters[currentEnemy].posY*spacing,Characters[currentEnemy].cTile);
      }
    }
    else if(wMap[wRow][wCol][Characters[0].posX + 2*attacks[0]][Characters[0].posY + 2*attacks[1]] >= 60 && wMap[wRow][wCol][Characters[0].posX + 2*attacks[0]][Characters[0].posY + 2*attacks[1]] <= 68) {
      bossHp--;
      if(bossHp <= 0){
        Characters[0].health = 0;
        tft.setCursor(58,116);
        tft.setTextSize(3);
        tft.setTextColor(0xFFFF);
        tft.print("YOU WON");
      }
      bossMechanics(2);
    }
  }
  else if (weapon == 2) {
    if((Characters[0].row == 0 && Characters[0].col == 1) && bossRoom == 1) {
      bossMechanics(0);
    }
    int i = 0;
    while(wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] < 10 || wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] == 14 || wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] == 15 || (wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] >= 50 && wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] <= 59) || wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] == 42) {
      i++;
      if (wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] >= 100) {
        int currentEnemy = wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]]-100;
        Characters[currentEnemy].health--;
        if(Characters[currentEnemy].health <= 0) {
          Characters[currentEnemy].cStatus = 0;
          if (Characters[currentEnemy].type < 6)
            Rooms[Characters[currentEnemy].row][Characters[currentEnemy].col].enemies--;
          if (Characters[currentEnemy].type == 7)
            Rooms[Characters[currentEnemy].row][Characters[currentEnemy].col].switches--;
          if(random(8) == 0 && Characters[currentEnemy].type < 7){
            Characters[currentEnemy].cTile = 5;
          }
          wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] = Characters[currentEnemy].cTile;
          drawTile(Characters[currentEnemy].posX*spacing,Characters[currentEnemy].posY*spacing,Characters[currentEnemy].cTile);
        }
      }
      else if(wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] >= 60 && wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] <= 68) {
        bossHp--;
        if(bossHp <= 0){
          Characters[0].health = 0;
          tft.setCursor(58,116);
          tft.setTextSize(3);
          tft.setTextColor(0xFFFF);
          tft.print("YOU WON");
        }
      }
      else if (wMap[wRow][wCol][Characters[0].posX + (i+1)*attacks[0]][Characters[0].posY + (i+1)*attacks[1]] < 10 || wMap[wRow][wCol][Characters[0].posX + (i+1)*attacks[0]][Characters[0].posY + (i+1)*attacks[1]] == 14 || wMap[wRow][wCol][Characters[0].posX + (i+1)*attacks[0]][Characters[0].posY + (i+1)*attacks[1]] == 15 || (wMap[wRow][wCol][Characters[0].posX + (i+1)*attacks[0]][Characters[0].posY + (i+1)*attacks[1]] >= 50 && wMap[wRow][wCol][Characters[0].posX + (i+1)*attacks[0]][Characters[0].posY + (i+1)*attacks[1]] <= 59)){
        for (int j = 0; j < 16; j++) {
          drawArrow(i, j);
          j++;
        }
      }
    }
  }
}

void gameBorder() {
  int borderColour = 0x096F;
  tft.fillRect(15*spacing,0*spacing,spacing*5,spacing,borderColour);
  tft.fillRect(15*spacing,1*spacing,spacing,spacing*13,borderColour);
  tft.fillRect(15*spacing,14*spacing,spacing*5,spacing,borderColour);
  tft.fillRect(19*spacing,1*spacing,spacing,spacing*13,borderColour);
  for(int i = 1;i<4;i++){
    borderTile(15+i,0,0);
  }
  for(int i = 0;i<13;i++) {
    borderTile(15,1+i,1);
  }
  borderTile(0,0,2);
}

void hearts() {
  if(Characters[0].health > 24)
        Characters[0].health = 24;
  tft.fillRect(16*spacing,1*spacing,48,32,0x0000);
  int fullHearts = Characters[0].health/4;
  int partHeart = Characters[0].health%4;
  int i,j = 1,row = 0;
  for(i = 16;i<fullHearts+16;i++){
    drawTile((i-row)*spacing,j*spacing,34);
    if(i == 16+2){
      j++;
      row = 3;
    }
  }
  switch(partHeart){
    case 1: drawTile((i-row)*spacing,j*spacing,31);
            break;
    case 2: drawTile((i-row)*spacing,j*spacing,32);
            break;
    case 3: drawTile((i-row)*spacing,j*spacing,33);
            break;
  }
}

void placeBreakables() {
  for (int i = ENEMYCOUNT + 1; i < ENEMYCOUNT + BREAKABLECOUNT + 1; i++) {
    if (Characters[i].row == wRow && Characters[i].col == wCol && Characters[i].cStatus == 1) {
      wMap[wRow][wCol][Characters[i].posX][Characters[i].posY] = i + 100;
      drawBreakable(i);
    }
  }
}

void enemiesMove(){
  for(int i = 1; i < ENEMYCOUNT + 1; i++){
    if(Characters[i].cStatus == 1 && (Characters[i].col == Characters[0].col && Characters[i].row == Characters[0].row))
      enemyMove(i);
  }
}
void enemyMove(int i) {
   int row,column;
   float moveDirection[2] = {};
   int moves[2] = {};
   row = Characters[i].posX;
   column = Characters[i].posY;
   if(charDistance(Characters[i].posX, Characters[i].posY) > 6) {
      do {
         if(rand()%2 == 0) {
            row = Characters[i].posX + (random(3) - 1);
            column = Characters[i].posY;
         }
         else {
            row = Characters[i].posX;
            column = Characters[i].posY + (random(3) - 1);
         }
      }while(wMap[wRow][wCol][row][column] >= 9);
   }
   else if(charDistance(Characters[i].posX, Characters[i].posY) == 1) {
      //Attack!
      if(Characters[0].posX < Characters[i].posX)
        Characters[i].cDirection = 0;
      else if(Characters[0].posY < Characters[i].posY)
        Characters[i].cDirection = 1;
      else if(Characters[0].posX > Characters[i].posX)
        Characters[i].cDirection = 2;
      else
        Characters[i].cDirection = 3;
      drawEnemy(i);
      if(Characters[i].type == 2 || Characters[i].type == 4)
        Characters[0].health-=2;
      else
        Characters[0].health--;
      hearts();
      if(Characters[0].health <= 0){
        drawTile(Characters[0].posX*spacing, Characters[0].posY*spacing,Characters[i].cTile);
        wMap[wRow][wCol][Characters[0].posX][Characters[0].posY] = Characters[0].cTile;
        tft.setCursor(41,110);
        tft.setTextSize(3);
        tft.setTextColor(0xF800);
        tft.print("GAME OVER");
      }
   }
   else {
      if((Characters[0].posX - Characters[i].posX) != 0)
         moveDirection[0] = (Characters[0].posX - Characters[i].posX)/abs(Characters[0].posX - Characters[i].posX);
      if((Characters[0].posY - Characters[i].posY) != 0)
         moveDirection[1] = (Characters[0].posY - Characters[i].posY)/abs(Characters[0].posY - Characters[i].posY);
      for(int i = 0; i < 2; i++)
         moves[i] = moveDirection[i];
      if(abs(Characters[0].posX - Characters[i].posX) >= abs(Characters[0].posY - Characters[i].posY)) {
         if(wMap[wRow][wCol][Characters[i].posX + moves[0]][Characters[i].posY] <= 9 && moves[0] != 0)
            row += moves[0];
         else if(wMap[wRow][wCol][Characters[i].posX][Characters[i].posY + moves[1]] <= 9 && moves[1] != 0)
            column += moves[1];
      }
      else if(moves[0] != 0 || moves[1] != 0) {
         if(wMap[wRow][wCol][Characters[i].posX][Characters[i].posY + moves[1]] <= 9 && moves[1] != 0)
            column += moves[1];
         else if(wMap[wRow][wCol][Characters[i].posX + moves[0]][Characters[i].posY] <= 9 && moves[0] != 0)
            row += moves[0];
      }
      if (row == Characters[i].posX && column == Characters[i].posY) {
         for (int j = 10; j < 14; j++) {
          if (wMap[wRow][wCol][Characters[i].posX + moves[0]][Characters[i].posY] == j || wMap[wRow][wCol][Characters[i].posX][Characters[i].posY + moves[1]] == j) {
            if(Characters[i].posY <= Rooms[Characters[i].row][Characters[i].col].centres[j-10][1] && Characters[0].posY <= Rooms[Characters[i].row][Characters[i].col].centres[j-10][1] && wMap[wRow][wCol][Characters[i].posX][Characters[i].posY-1] <= 9)
              column--;
            else if(Characters[i].posY >= Rooms[Characters[i].row][Characters[i].col].centres[j-10][1] && Characters[0].posY >= Rooms[Characters[i].row][Characters[i].col].centres[j-10][1] && wMap[wRow][wCol][Characters[i].posX][Characters[i].posY+1] <= 9)
              column++;
            else if(Characters[i].posX <= Rooms[Characters[i].row][Characters[i].col].centres[j-10][0] && Characters[0].posX <= Rooms[Characters[i].row][Characters[i].col].centres[j-10][0] && wMap[wRow][wCol][Characters[i].posX-1][Characters[i].posY] <= 9)
              row--;
            else if(Characters[i].posX >= Rooms[Characters[i].row][Characters[i].col].centres[j-10][0] && Characters[0].posX >= Rooms[Characters[i].row][Characters[i].col].centres[j-10][0] && wMap[wRow][wCol][Characters[i].posX+1][Characters[i].posY] <= 9)
              row++;
          }
        }
      }
   }
   drawTile(Characters[i].posX*spacing, Characters[i].posY*spacing,Characters[i].cTile);
   wMap[wRow][wCol][Characters[i].posX][Characters[i].posY] = Characters[i].cTile;
   Characters[i].cTile = wMap[wRow][wCol][row][column];
   wMap[wRow][wCol][row][column] = 100+i;
   if(Characters[i].posX > row)
    Characters[i].cDirection = 0; 
   else if(Characters[i].posX < row)
    Characters[i].cDirection = 2;
   else if(Characters[i].posY > column)
    Characters[i].cDirection = 1;
   else if(Characters[i].posY < column)
    Characters[i].cDirection = 3;
   Characters[i].posX = row;
   Characters[i].posY = column;
   drawEnemy(i);
}

void bossMechanics(int mechanicNum) {
  if(mechanicNum == 0){
    if(random(10) == 0 && Characters[0].cTile == 2) {
      Characters[0].cTile = 50;
      drawHero(Characters[0].posX,Characters[0].posY);
    }
    else if(Characters[0].cTile == 50) {
      Characters[0].cTile = 51;
      drawHero(Characters[0].posX,Characters[0].posY);
      Characters[0].health-=2;
      hearts();
    }
    else if(Characters[0].cTile == 51) {
      Characters[0].cTile = 52;
      drawHero(Characters[0].posX,Characters[0].posY);
      Characters[0].health-=2;
      hearts();
    }
    else if(Characters[0].cTile == 52) {
      Characters[0].cTile = 2;
      drawHero(Characters[0].posX,Characters[0].posY);
      Characters[0].health-=2;
      hearts();
    }
    //Place Random Lava
    for(int i = 0;i<15;i++) {
      for(int j = 0;j<15;j++) {
        if(wMap[wRow][wCol][i][j] == 2  && random(10) == 0  ) {
          wMap[wRow][wCol][i][j] = 50;
          drawTile(i*spacing,j*spacing,wMap[wRow][wCol][i][j]);
        }
        else if(wMap[wRow][wCol][i][j] == 50) {
          wMap[wRow][wCol][i][j] = 51;
          drawTile(i*spacing,j*spacing,wMap[wRow][wCol][i][j]);
        }
        else if(wMap[wRow][wCol][i][j] == 51) {
          wMap[wRow][wCol][i][j] = 52;
          drawTile(i*spacing,j*spacing,wMap[wRow][wCol][i][j]);
        }
        else if(wMap[wRow][wCol][i][j] == 52) {
          wMap[wRow][wCol][i][j] = 2;
          drawTile(i*spacing,j*spacing,wMap[wRow][wCol][i][j]);
        }
      }
    }
  }
  else if(mechanicNum == 1) {
    for(int i = 2;i<4;i++) {
      if(wMap[wRow][wCol][5][i] != 42) {
        if(wMap[wRow][wCol][5][i] == 2) {
          wMap[wRow][wCol][5][i] = 50;
          drawTile(5*spacing,i*spacing,wMap[wRow][wCol][5][i]);
        }
        else if(wMap[wRow][wCol][5][i] < 52) {
          wMap[wRow][wCol][5][i]++;
          drawTile(5*spacing,i*spacing,wMap[wRow][wCol][5][i]);
        }
        else if(wMap[wRow][wCol][5][i] == 52) {
          wMap[wRow][wCol][5][i] = 2;
          drawTile(5*spacing,i*spacing,wMap[wRow][wCol][5][i]);
        }
      }
      else {
        if(Characters[0].cTile == 2) {
          Characters[0].cTile = 50;
          drawHero(Characters[0].posX,Characters[0].posY);
        }
        else if(Characters[0].cTile < 52) {
          Characters[0].cTile++;
          drawHero(Characters[0].posX,Characters[0].posY);
          Characters[0].health -= 2;
          hearts();
        }
        else {
          Characters[0].cTile = 2;
          drawHero(Characters[0].posX,Characters[0].posY);
          Characters[0].health -= 2;
          hearts();
        }
      }
      if(wMap[wRow][wCol][9][i] != 42) {
        if(wMap[wRow][wCol][9][i] == 2) {
          wMap[wRow][wCol][9][i] = 50;
          drawTile(9*spacing,i*spacing,wMap[wRow][wCol][9][i]);
        }
        else if(wMap[wRow][wCol][9][i] < 52) {
          wMap[wRow][wCol][9][i]++;
          drawTile(9*spacing,i*spacing,wMap[wRow][wCol][9][i]);
        }
        else if(wMap[wRow][wCol][9][i] == 52) {
          wMap[wRow][wCol][9][i] = 2;
          drawTile(9*spacing,i*spacing,wMap[wRow][wCol][9][i]);
        }
      }
      else {
        if(Characters[0].cTile == 2) {
          Characters[0].cTile = 50;
          drawHero(Characters[0].posX,Characters[0].posY);
        }
        else if(Characters[0].cTile < 52) {
          Characters[0].cTile++;
          drawHero(Characters[0].posX,Characters[0].posY);
          Characters[0].health -= 2;
          hearts();
        }
        else {
          Characters[0].cTile = 2;
          drawHero(Characters[0].posX,Characters[0].posY);
          Characters[0].health -= 2;
          hearts();
        }
      }
    }
   
    for(int i = 6;i<9;i++) {
      if(wMap[wRow][wCol][i][3] != 42) {
        if(wMap[wRow][wCol][i][3] == 2) {
          wMap[wRow][wCol][i][3] = 50;
          drawTile(i*spacing,3*spacing,wMap[wRow][wCol][i][3]);
        }
        else if(wMap[wRow][wCol][i][3] < 52) {
          wMap[wRow][wCol][i][3]++;
          drawTile(i*spacing,3*spacing,wMap[wRow][wCol][i][3]);
        }
        else if(wMap[wRow][wCol][i][3] == 52) {
          wMap[wRow][wCol][i][3] = 2;
          drawTile(i*spacing,3*spacing,wMap[wRow][wCol][i][3]);
        }
      }
      else {
        if(Characters[0].cTile == 2) {
          Characters[0].cTile = 50;
          drawHero(Characters[0].posX,Characters[0].posY);
        }
        else if(Characters[0].cTile < 52) {
          Characters[0].cTile++;
          drawHero(Characters[0].posX,Characters[0].posY);
          Characters[0].health -= 2;
          hearts();
        }
        else {
          Characters[0].cTile = 2;
          drawHero(Characters[0].posX,Characters[0].posY);
          Characters[0].health -= 2;
          hearts();
        }
      }
    }
  }
}

float charDistance(int charX,int charY) {
   return sqrt(pow((Characters[0].posX - charX),2) + pow((Characters[0].posY-charY),2));
}

void faceDirection() {
  for(int i = 0;i<4;i++) {
    dPad[i] = digitalRead(22+i);
    if(!dPad[i] && Characters[0].cDirection !=i) {
      Characters[0].cDirection = i;
      drawHero(Characters[0].posX,Characters[0].posY);
    }
  }
}

void deleteChunk(int x,int y) {
  tft.fillRect(x*spacing,y*spacing,16,16,ILI9340_BLACK);
}

void drawMap() {
  tft.fillScreen(ILI9340_BLACK);
  for(int i = 0;i<15;i++) {
    for(int j = 0;j<15;j++) {
      drawTile(i*spacing,j*spacing,wMap[wRow][wCol][i][j]);
    }
  }
}

void drawArrow(int tilesMoved, int pixelsMoved) {
  if (Characters[0].cDirection == 0) {
    int refX = (Characters[0].posX + 1 - tilesMoved)*spacing - pixelsMoved - 1;
    int refY = Characters[0].posY*spacing;
    tft.drawFastHLine(refX - 12, refY + 7, 11, 0xC440);
    tft.drawFastVLine(refX - 13, refY + 5, 5, 0xA513);
    tft.drawFastVLine(refX - 14, refY + 6, 3, 0xA513);
    tft.drawPixel(refX - 15, refY + 7, 0xA513);
    tft.drawFastVLine(refX - 1, refY + 6, 3, 0xD000);
    tft.drawPixel(refX, refY + 5, 0xD000);
    tft.drawPixel(refX, refY + 7, 0xD000);
    tft.drawPixel(refX, refY + 9, 0xD000);
    if (wMap[wRow][wCol][Characters[0].posX - tilesMoved][Characters[0].posY] < 100)
      drawTile((Characters[0].posX - tilesMoved)*spacing, refY, wMap[wRow][wCol][Characters[0].posX - tilesMoved][Characters[0].posY]);
    if (wMap[wRow][wCol][Characters[0].posX - tilesMoved - 1][Characters[0].posY] < 100)
      drawTile((Characters[0].posX - tilesMoved - 1)*spacing, refY, wMap[wRow][wCol][Characters[0].posX - tilesMoved - 1][Characters[0].posY]);
  }
  else if (Characters[0].cDirection == 1) {
    int refX = (Characters[0].posX + 1)*spacing;
    int refY = (Characters[0].posY - tilesMoved)*spacing - pixelsMoved;
    tft.drawFastVLine(refX - 7, refY + 3, 11, 0xC440);
    tft.drawFastHLine(refX - 9, refY + 2, 5, 0xA513);
    tft.drawFastHLine(refX - 8, refY + 1, 3, 0xA513);
    tft.drawPixel(refX - 7, refY, 0xA513);
    tft.drawFastHLine(refX - 8, refY + 14, 3, 0xD000);
    tft.drawPixel(refX - 9, refY + 15, 0xD000);
    tft.drawPixel(refX - 7, refY + 15, 0xD000);
    tft.drawPixel(refX - 5, refY + 15, 0xD000);
    if (wMap[wRow][wCol][Characters[0].posX][Characters[0].posY - tilesMoved] < 100)
      drawTile(Characters[0].posX*spacing, (Characters[0].posY - tilesMoved)*spacing, wMap[wRow][wCol][Characters[0].posX][Characters[0].posY - tilesMoved]);
    if (wMap[wRow][wCol][Characters[0].posX][Characters[0].posY - tilesMoved - 1] < 100)
      drawTile(Characters[0].posX*spacing, (Characters[0].posY - tilesMoved - 1)*spacing, wMap[wRow][wCol][Characters[0].posX][Characters[0].posY - tilesMoved - 1]);
  }
  else if (Characters[0].cDirection == 2) {
    int refX = (Characters[0].posX + 1 + tilesMoved)*spacing + pixelsMoved;
    int refY = Characters[0].posY*spacing;
    tft.drawFastHLine(refX - 13, refY + 7, 11, 0xC440);
    tft.drawFastVLine(refX - 2, refY + 5, 5, 0xA513);
    tft.drawFastVLine(refX - 1, refY + 6, 3, 0xA513);
    tft.drawPixel(refX, refY + 7, 0xA513);
    tft.drawFastVLine(refX - 14, refY + 6, 3, 0xD000);
    tft.drawPixel(refX - 15, refY + 5, 0xD000);
    tft.drawPixel(refX - 15, refY + 7, 0xD000);
    tft.drawPixel(refX - 15, refY + 9, 0xD000);
    if (wMap[wRow][wCol][Characters[0].posX + tilesMoved][Characters[0].posY] < 100)
      drawTile((Characters[0].posX + tilesMoved)*spacing, refY, wMap[wRow][wCol][Characters[0].posX + tilesMoved][Characters[0].posY]);
    if (wMap[wRow][wCol][Characters[0].posX + tilesMoved + 1][Characters[0].posY] < 100)
      drawTile((Characters[0].posX + tilesMoved + 1)*spacing, refY, wMap[wRow][wCol][Characters[0].posX + tilesMoved + 1][Characters[0].posY]);
  }
  else if (Characters[0].cDirection == 3) {
    int refX = (Characters[0].posX + 1)*spacing;
    int refY = (Characters[0].posY + tilesMoved)*spacing + pixelsMoved;
    tft.drawFastVLine(refX - 7, refY + 2, 11, 0xC440);
    tft.drawFastHLine(refX - 9, refY + 13, 5, 0xA513);
    tft.drawFastHLine(refX - 8, refY + 14, 3, 0xA513);
    tft.drawPixel(refX - 7, refY + 15, 0xA513);
    tft.drawFastHLine(refX - 8, refY + 1, 3, 0xD000);
    tft.drawPixel(refX - 9, refY, 0xD000);
    tft.drawPixel(refX - 7, refY, 0xD000);
    if (wMap[wRow][wCol][Characters[0].posX][Characters[0].posY + tilesMoved] < 100)
      drawTile(Characters[0].posX*spacing, (Characters[0].posY + tilesMoved)*spacing, wMap[wRow][wCol][Characters[0].posX][Characters[0].posY + tilesMoved]);
    if (wMap[wRow][wCol][Characters[0].posX][Characters[0].posY + tilesMoved + 1] < 100)
      drawTile(Characters[0].posX*spacing, (Characters[0].posY + tilesMoved + 1)*spacing, wMap[wRow][wCol][Characters[0].posX][Characters[0].posY + tilesMoved + 1]);
  }
}

void drawBreakable(int i) {
  int x = Characters[i].posX*spacing;
  int y = Characters[i].posY*spacing;
  drawTile(Characters[i].posX*spacing, Characters[i].posY*spacing, Characters[i].cTile);
  if (Characters[i].type == 7) {
    // Draw a switch
    // Outer white layer
    tft.drawFastHLine(x + 7, y, 2, 0xA513);
    tft.drawFastHLine(x + 6, y + 1, 4, 0xA513);
    tft.drawFastHLine(x + 6, y + 14, 4, 0xA513);
    tft.drawFastHLine(x + 7, y + 15, 2, 0xA513);
    tft.drawFastVLine(x, y + 7, 2, 0xA513);
    tft.drawFastVLine(x + 1, y + 6, 4, 0xA513);
    tft.drawFastVLine(x + 14, y + 6, 4, 0xA513);
    tft.drawFastVLine(x + 15, y + 7, 2, 0xA513);
    // Outer red layer
    tft.drawLine(x + 7, y + 2, x + 2, y + 7, 0xD000);
    tft.drawLine(x + 8, y + 2, x + 13, y + 7, 0xD000);
    tft.drawLine(x + 7, y + 13, x + 2, y + 8, 0xD000);
    tft.drawLine(x + 8, y + 13, x + 13, y + 8, 0xD000);
    tft.drawLine(x + 6, y + 2, x + 2, y + 6, 0xD000);
    tft.drawLine(x + 9, y + 2, x + 13, y + 6, 0xD000);
    tft.drawLine(x + 6, y + 13, x + 2, y + 9, 0xD000);
    tft.drawLine(x + 9, y + 13, x + 13, y + 9, 0xD000);
    // Inner white layer
    tft.drawLine(x + 7, y + 3, x + 3, y + 7, 0xA513);
    tft.drawLine(x + 8, y + 3, x + 12, y + 7, 0xA513);
    tft.drawLine(x + 7, y + 12, x + 3, y + 8, 0xA513);
    tft.drawLine(x + 8, y + 12, x + 12, y + 8, 0xA513);
    // Middle red layer
    tft.drawLine(x + 7, y + 4, x + 4, y + 7, 0xD000);
    tft.drawLine(x + 8, y + 4, x + 11, y + 7, 0xD000);
    tft.drawLine(x + 7, y + 11, x + 4, y + 8, 0xD000);
    tft.drawLine(x + 8, y + 11, x + 11, y + 8, 0xD000);
    tft.drawLine(x + 7, y + 5, x + 5, y + 7, 0xD000);
    tft.drawLine(x + 8, y + 5, x + 10, y + 7, 0xD000);
    tft.drawLine(x + 7, y + 10, x + 5, y + 8, 0xD000);
    tft.drawLine(x + 8, y + 10, x + 10, y + 8, 0xD000);
    // Yellow layer
    tft.drawFastHLine(x + 7, y + 6, 2, 0xED46);
    tft.drawFastHLine(x + 7, y + 9, 2, 0xED46);
    tft.drawFastVLine(x + 6, y + 7, 2, 0xED46);
    tft.drawFastVLine(x + 9, y + 7, 2, 0xED46);
    // Inner red layer
    tft.fillRect(x + 7,y + 7, 2, 2, 0xD000);
  }
  else if (Characters[i].type == 6) {
    // Draw a pot
    // Outline
    tft.drawLine(x + 5, y + 2, x + 2, y + 5, ILI9340_BLACK);
    tft.drawLine(x + 10, y + 2, x + 13, y + 5, ILI9340_BLACK);
    tft.drawFastVLine(x + 2, y + 6, 4, ILI9340_BLACK);
    tft.drawFastVLine(x + 13, y + 6, 4, ILI9340_BLACK);
    tft.drawLine(x + 5, y + 13, x + 2, y + 10, ILI9340_BLACK);
    tft.drawLine(x + 10, y + 13, x + 13, y + 10, ILI9340_BLACK);
    tft.drawFastHLine(x + 6, y + 13, 4, ILI9340_BLACK);
    tft.drawFastVLine(x + 4, y + 4, 2, ILI9340_BLACK);
    tft.drawFastVLine(x + 11, y + 4, 2, ILI9340_BLACK);
    tft.drawLine(x + 4, y + 5, x + 6, y + 8, ILI9340_BLACK);
    tft.drawLine(x + 11, y + 5, x + 9, y + 8, ILI9340_BLACK);
    tft.drawFastHLine(x + 7, y + 8, 2, ILI9340_BLACK);
    // Ring
    tft.drawFastHLine(x + 6, y + 2, 4, 0xED46);
    tft.drawFastHLine(x + 6, y + 6, 4, 0xED46);
    tft.drawFastVLine(x + 5, y + 3, 3, 0xED46);
    tft.drawFastVLine(x + 10, y + 3, 3, 0xED46);
  }
}

void borderTile(int xPos,int yPos,int side) {
  int borderColour = 0xB463;
  if(side == 0) {
    //Top Border
    tft.drawFastVLine(xPos*spacing,yPos*spacing+5,4,borderColour);
    tft.drawFastVLine(xPos*spacing+15,yPos*spacing+5,4,borderColour);
    tft.drawFastVLine(xPos*spacing+1,yPos*spacing+4,2,borderColour);
    tft.drawFastVLine(xPos*spacing+14,yPos*spacing+4,2,borderColour);
    tft.drawFastVLine(xPos*spacing+2,yPos*spacing+3,2,borderColour);
    tft.drawFastVLine(xPos*spacing+13,yPos*spacing+3,2,borderColour);
    tft.drawPixel(xPos*spacing+3,yPos*spacing+3,borderColour);
    tft.drawPixel(xPos*spacing+12,yPos*spacing+3,borderColour);
    tft.drawFastVLine(xPos*spacing+4,yPos*spacing+3,2,borderColour);
    tft.drawFastVLine(xPos*spacing+11,yPos*spacing+3,2,borderColour);
    tft.drawFastVLine(xPos*spacing+5,yPos*spacing+4,3,borderColour);
    tft.drawFastVLine(xPos*spacing+10,yPos*spacing+4,3,borderColour);
    tft.drawFastVLine(xPos*spacing+6,yPos*spacing+4,2,borderColour);
    tft.drawFastVLine(xPos*spacing+9,yPos*spacing+4,2,borderColour);
    tft.drawFastVLine(xPos*spacing+7,yPos*spacing+3,2,borderColour);
    tft.drawFastVLine(xPos*spacing+8,yPos*spacing+3,2,borderColour);
    tft.fillRect(xPos*spacing+1,yPos*spacing+7,3,2,borderColour);
    tft.fillRect(xPos*spacing+12,yPos*spacing+7,3,2,borderColour);
    tft.drawFastVLine(xPos*spacing+4,yPos*spacing+6,2,borderColour);
    tft.drawFastVLine(xPos*spacing+11,yPos*spacing+6,2,borderColour);
    tft.fillRect(xPos*spacing,yPos*spacing+11,16,2,borderColour);
    tft.drawFastHLine(xPos*spacing+4,yPos*spacing+10,8,borderColour);
    tft.drawFastHLine(xPos*spacing+6,yPos*spacing+9,4,borderColour);
    tft.drawFastHLine(xPos*spacing+7,yPos*spacing+8,2,borderColour);
    yPos+=14;
    //Bottom Border
    tft.fillRect(xPos*spacing,yPos*spacing+3,16,2,borderColour);
    tft.drawFastHLine(xPos*spacing+4,yPos*spacing+5,8,borderColour);
    tft.drawFastHLine(xPos*spacing+6,yPos*spacing+6,4,borderColour);
    tft.drawFastHLine(xPos*spacing+7,yPos*spacing+7,2,borderColour);
    tft.drawFastVLine(xPos*spacing,yPos*spacing+7,4,borderColour);
    tft.drawFastVLine(xPos*spacing+15,yPos*spacing+7,4,borderColour);
    tft.drawFastVLine(xPos*spacing+1,yPos*spacing+10,2,borderColour);
    tft.drawFastVLine(xPos*spacing+14,yPos*spacing+10,2,borderColour);
    tft.drawFastVLine(xPos*spacing+2,yPos*spacing+11,2,borderColour);
    tft.drawFastVLine(xPos*spacing+13,yPos*spacing+11,2,borderColour);
    tft.drawPixel(xPos*spacing+3,yPos*spacing+12,borderColour);
    tft.drawPixel(xPos*spacing+12,yPos*spacing+12,borderColour);
    tft.drawFastVLine(xPos*spacing+4,yPos*spacing+11,2,borderColour);
    tft.drawFastVLine(xPos*spacing+11,yPos*spacing+11,2,borderColour);
    tft.drawFastVLine(xPos*spacing+5,yPos*spacing+9,3,borderColour);
    tft.drawFastVLine(xPos*spacing+10,yPos*spacing+9,3,borderColour);
    tft.drawFastVLine(xPos*spacing+6,yPos*spacing+10,2,borderColour);
    tft.drawFastVLine(xPos*spacing+9,yPos*spacing+10,2,borderColour);
    tft.drawFastVLine(xPos*spacing+7,yPos*spacing+11,2,borderColour);
    tft.drawFastVLine(xPos*spacing+8,yPos*spacing+11,2,borderColour);
    tft.fillRect(xPos*spacing+1,yPos*spacing+7,3,2,borderColour);
    tft.fillRect(xPos*spacing+12,yPos*spacing+7,3,2,borderColour);
    tft.drawFastVLine(xPos*spacing+4,yPos*spacing+8,2,borderColour);
    tft.drawFastVLine(xPos*spacing+11,yPos*spacing+8,2,borderColour);
  }
  else if(side == 1) {
    //Left Border
    tft.drawFastVLine(xPos*spacing+3,yPos*spacing+2,3,borderColour);
    tft.drawFastVLine(xPos*spacing+3,yPos*spacing+7,2,borderColour);
    tft.drawFastVLine(xPos*spacing+3,yPos*spacing+11,3,borderColour);
    tft.drawFastVLine(xPos*spacing+4,yPos*spacing+1,2,borderColour);
    tft.drawFastVLine(xPos*spacing+4,yPos*spacing+4,8,borderColour);
    tft.drawFastVLine(xPos*spacing+4,yPos*spacing+13,2,borderColour);
    tft.drawFastVLine(xPos*spacing+5,yPos*spacing,2,borderColour);
    tft.drawFastVLine(xPos*spacing+5,yPos*spacing+5,2,borderColour);
    tft.drawFastVLine(xPos*spacing+5,yPos*spacing+9,2,borderColour);
    tft.drawFastVLine(xPos*spacing+5,yPos*spacing+14,2,borderColour);
    tft.drawPixel(xPos*spacing+6,yPos*spacing,borderColour);
    tft.drawFastVLine(xPos*spacing+6,yPos*spacing+4,2,borderColour);
    tft.drawFastVLine(xPos*spacing+6,yPos*spacing+10,2,borderColour);
    tft.drawPixel(xPos*spacing+6,yPos*spacing+15,borderColour);
    tft.drawFastVLine(xPos*spacing+7,yPos*spacing,5,borderColour);
    tft.drawFastVLine(xPos*spacing+7,yPos*spacing+11,5,borderColour);
    tft.drawFastVLine(xPos*spacing+8,yPos*spacing,4,borderColour);
    tft.drawFastVLine(xPos*spacing+8,yPos*spacing+7,2,borderColour);
    tft.drawFastVLine(xPos*spacing+8,yPos*spacing+12,4,borderColour);
    tft.drawFastVLine(xPos*spacing+9,yPos*spacing+6,4,borderColour);
    tft.drawFastVLine(xPos*spacing+10,yPos*spacing+4,8,borderColour);
    tft.drawFastVLine(xPos*spacing+11,yPos*spacing,16,borderColour);
    tft.drawFastVLine(xPos*spacing+12,yPos*spacing,16,borderColour);
    xPos+=4;
    //Right Border
    tft.drawFastVLine(xPos*spacing+12,yPos*spacing+2,3,borderColour);
    tft.drawFastVLine(xPos*spacing+12,yPos*spacing+7,2,borderColour);
    tft.drawFastVLine(xPos*spacing+12,yPos*spacing+11,3,borderColour);
    tft.drawFastVLine(xPos*spacing+11,yPos*spacing+1,2,borderColour);
    tft.drawFastVLine(xPos*spacing+11,yPos*spacing+4,8,borderColour);
    tft.drawFastVLine(xPos*spacing+11,yPos*spacing+13,2,borderColour);
    tft.drawFastVLine(xPos*spacing+10,yPos*spacing,2,borderColour);
    tft.drawFastVLine(xPos*spacing+10,yPos*spacing+5,2,borderColour);
    tft.drawFastVLine(xPos*spacing+10,yPos*spacing+9,2,borderColour);
    tft.drawFastVLine(xPos*spacing+10,yPos*spacing+14,2,borderColour);
    tft.drawPixel(xPos*spacing+9,yPos*spacing,borderColour);
    tft.drawFastVLine(xPos*spacing+9,yPos*spacing+4,2,borderColour);
    tft.drawFastVLine(xPos*spacing+9,yPos*spacing+10,2,borderColour);
    tft.drawPixel(xPos*spacing+9,yPos*spacing+15,borderColour);
    tft.drawFastVLine(xPos*spacing+8,yPos*spacing,5,borderColour);
    tft.drawFastVLine(xPos*spacing+8,yPos*spacing+11,5,borderColour);
    tft.drawFastVLine(xPos*spacing+7,yPos*spacing,4,borderColour);
    tft.drawFastVLine(xPos*spacing+7,yPos*spacing+7,2,borderColour);
    tft.drawFastVLine(xPos*spacing+7,yPos*spacing+12,4,borderColour);
    tft.drawFastVLine(xPos*spacing+6,yPos*spacing+6,4,borderColour);
    tft.drawFastVLine(xPos*spacing+5,yPos*spacing+4,8,borderColour);
    tft.drawFastVLine(xPos*spacing+4,yPos*spacing,16,borderColour);
    tft.drawFastVLine(xPos*spacing+3,yPos*spacing,16,borderColour);
  }
  else {
    //Top Left Corner
    tft.drawFastVLine(15*spacing+3,0*spacing+3,2,borderColour);
    tft.drawFastVLine(15*spacing+3,0*spacing+7,2,borderColour);
    tft.drawFastVLine(15*spacing+3,0*spacing+12,2,borderColour);
    tft.drawFastVLine(15*spacing+4,0*spacing+3,7,borderColour);
    tft.drawFastVLine(15*spacing+4,0*spacing+11,4,borderColour);
    tft.drawFastVLine(15*spacing+5,0*spacing+4,3,borderColour);
    tft.drawFastVLine(15*spacing+5,0*spacing+9,3,borderColour);
    tft.drawFastVLine(15*spacing+5,0*spacing+14,2,borderColour);
    tft.drawFastVLine(15*spacing+6,0*spacing+4,2,borderColour);
    tft.drawFastVLine(15*spacing+6,0*spacing+14,2,borderColour);
    tft.drawFastVLine(15*spacing+7,0*spacing+3,2,borderColour);
    tft.drawFastVLine(15*spacing+7,0*spacing+7,9,borderColour);
    tft.drawFastVLine(15*spacing+8,0*spacing+3,2,borderColour);
    tft.drawFastVLine(15*spacing+8,0*spacing+7,9,borderColour);
    tft.drawFastVLine(15*spacing+9,0*spacing+4,2,borderColour);
    tft.drawFastVLine(15*spacing+9,0*spacing+7,2,borderColour);
    tft.drawPixel(15*spacing+10,0*spacing+5,borderColour);
    tft.drawFastVLine(15*spacing+10,0*spacing+7,2,borderColour);
    tft.drawFastVLine(15*spacing+11,0*spacing+4,2,borderColour);
    tft.drawFastVLine(15*spacing+11,0*spacing+7,2,borderColour);
    tft.drawFastVLine(15*spacing+11,0*spacing+11,5,borderColour);
    tft.drawFastVLine(15*spacing+12,0*spacing+3,2,borderColour);
    tft.drawFastVLine(15*spacing+12,0*spacing+7,2,borderColour);
    tft.drawFastVLine(15*spacing+12,0*spacing+11,5,borderColour);
    tft.drawFastVLine(15*spacing+13,0*spacing+3,2,borderColour);
    tft.drawFastVLine(15*spacing+13,0*spacing+7,2,borderColour);
    tft.drawFastVLine(15*spacing+13,0*spacing+11,2,borderColour);
    tft.drawFastVLine(15*spacing+14,0*spacing+4,5,borderColour);
    tft.drawFastVLine(15*spacing+14,0*spacing+11,2,borderColour);
    tft.drawFastVLine(15*spacing+15,0*spacing+5,4,borderColour);
    tft.drawFastVLine(15*spacing+15,0*spacing+11,2,borderColour);
    //Top Right Corner
    tft.drawFastVLine(19*spacing+12,0*spacing+3,2,borderColour);
    tft.drawFastVLine(19*spacing+12,0*spacing+7,2,borderColour);
    tft.drawFastVLine(19*spacing+12,0*spacing+12,2,borderColour);
    tft.drawFastVLine(19*spacing+11,0*spacing+3,7,borderColour);
    tft.drawFastVLine(19*spacing+11,0*spacing+11,4,borderColour);
    tft.drawFastVLine(19*spacing+10,0*spacing+4,3,borderColour);
    tft.drawFastVLine(19*spacing+10,0*spacing+9,3,borderColour);
    tft.drawFastVLine(19*spacing+10,0*spacing+14,2,borderColour);
    tft.drawFastVLine(19*spacing+9,0*spacing+4,2,borderColour);
    tft.drawFastVLine(19*spacing+9,0*spacing+14,2,borderColour);
    tft.drawFastVLine(19*spacing+8,0*spacing+3,2,borderColour);
    tft.drawFastVLine(19*spacing+8,0*spacing+7,9,borderColour);
    tft.drawFastVLine(19*spacing+7,0*spacing+3,2,borderColour);
    tft.drawFastVLine(19*spacing+7,0*spacing+7,9,borderColour);
    tft.drawFastVLine(19*spacing+6,0*spacing+4,2,borderColour);
    tft.drawFastVLine(19*spacing+6,0*spacing+7,2,borderColour);
    tft.drawPixel(19*spacing+5,0*spacing+5,borderColour);
    tft.drawFastVLine(19*spacing+5,0*spacing+7,2,borderColour);
    tft.drawFastVLine(19*spacing+4,0*spacing+4,2,borderColour);
    tft.drawFastVLine(19*spacing+4,0*spacing+7,2,borderColour);
    tft.drawFastVLine(19*spacing+4,0*spacing+11,5,borderColour);
    tft.drawFastVLine(19*spacing+3,0*spacing+3,2,borderColour);
    tft.drawFastVLine(19*spacing+3,0*spacing+7,2,borderColour);
    tft.drawFastVLine(19*spacing+3,0*spacing+11,5,borderColour);
    tft.drawFastVLine(19*spacing+2,0*spacing+3,2,borderColour);
    tft.drawFastVLine(19*spacing+2,0*spacing+7,2,borderColour);
    tft.drawFastVLine(19*spacing+2,0*spacing+11,2,borderColour);
    tft.drawFastVLine(19*spacing+1,0*spacing+4,5,borderColour);
    tft.drawFastVLine(19*spacing+1,0*spacing+11,2,borderColour);
    tft.drawFastVLine(19*spacing,0*spacing+5,4,borderColour);
    tft.drawFastVLine(19*spacing,0*spacing+11,2,borderColour);
    //Bottom Left Corner
    tft.drawFastVLine(15*spacing+3,14*spacing+11,2,borderColour);
    tft.drawFastVLine(15*spacing+3,14*spacing+7,2,borderColour);
    tft.drawFastVLine(15*spacing+3,14*spacing+2,2,borderColour);
    tft.drawFastVLine(15*spacing+4,14*spacing+6,7,borderColour);
    tft.drawFastVLine(15*spacing+4,14*spacing+1,4,borderColour);
    tft.drawFastVLine(15*spacing+5,14*spacing+9,3,borderColour);
    tft.drawFastVLine(15*spacing+5,14*spacing+4,3,borderColour);
    tft.drawFastVLine(15*spacing+5,14*spacing,2,borderColour);
    tft.drawFastVLine(15*spacing+6,14*spacing+10,2,borderColour);
    tft.drawFastVLine(15*spacing+6,14*spacing,2,borderColour);
    tft.drawFastVLine(15*spacing+7,14*spacing+11,2,borderColour);
    tft.drawFastVLine(15*spacing+7,14*spacing,9,borderColour);
    tft.drawFastVLine(15*spacing+8,14*spacing+11,2,borderColour);
    tft.drawFastVLine(15*spacing+8,14*spacing,9,borderColour);
    tft.drawFastVLine(15*spacing+9,14*spacing+10,2,borderColour);
    tft.drawFastVLine(15*spacing+9,14*spacing+7,2,borderColour);
    tft.drawPixel(15*spacing+10,14*spacing+10,borderColour);
    tft.drawFastVLine(15*spacing+10,14*spacing+7,2,borderColour);
    tft.drawFastVLine(15*spacing+11,14*spacing+10,2,borderColour);
    tft.drawFastVLine(15*spacing+11,14*spacing+7,2,borderColour);
    tft.drawFastVLine(15*spacing+11,14*spacing,5,borderColour);
    tft.drawFastVLine(15*spacing+12,14*spacing+11,2,borderColour);
    tft.drawFastVLine(15*spacing+12,14*spacing+7,2,borderColour);
    tft.drawFastVLine(15*spacing+12,14*spacing,5,borderColour);
    tft.drawFastVLine(15*spacing+13,14*spacing+11,2,borderColour);
    tft.drawFastVLine(15*spacing+13,14*spacing+7,2,borderColour);
    tft.drawFastVLine(15*spacing+13,14*spacing+3,2,borderColour);
    tft.drawFastVLine(15*spacing+14,14*spacing+7,5,borderColour);
    tft.drawFastVLine(15*spacing+14,14*spacing+3,2,borderColour);
    tft.drawFastVLine(15*spacing+15,14*spacing+7,4,borderColour);
    tft.drawFastVLine(15*spacing+15,14*spacing+3,2,borderColour);
    //Bottom Right Corner
    tft.drawFastVLine(19*spacing+12,14*spacing+11,2,borderColour);
    tft.drawFastVLine(19*spacing+12,14*spacing+7,2,borderColour);
    tft.drawFastVLine(19*spacing+12,14*spacing+2,2,borderColour);
    tft.drawFastVLine(19*spacing+11,14*spacing+6,7,borderColour);
    tft.drawFastVLine(19*spacing+11,14*spacing+1,4,borderColour);
    tft.drawFastVLine(19*spacing+10,14*spacing+9,3,borderColour);
    tft.drawFastVLine(19*spacing+10,14*spacing+4,3,borderColour);
    tft.drawFastVLine(19*spacing+10,14*spacing,2,borderColour);
    tft.drawFastVLine(19*spacing+9,14*spacing+10,2,borderColour);
    tft.drawFastVLine(19*spacing+9,14*spacing,2,borderColour);
    tft.drawFastVLine(19*spacing+8,14*spacing+11,2,borderColour);
    tft.drawFastVLine(19*spacing+8,14*spacing,9,borderColour);
    tft.drawFastVLine(19*spacing+7,14*spacing+11,2,borderColour);
    tft.drawFastVLine(19*spacing+7,14*spacing,9,borderColour);
    tft.drawFastVLine(19*spacing+6,14*spacing+10,2,borderColour);
    tft.drawFastVLine(19*spacing+6,14*spacing+7,2,borderColour);
    tft.drawPixel(19*spacing+5,14*spacing+10,borderColour);
    tft.drawFastVLine(19*spacing+5,14*spacing+7,2,borderColour);
    tft.drawFastVLine(19*spacing+4,14*spacing+10,2,borderColour);
    tft.drawFastVLine(19*spacing+4,14*spacing+7,2,borderColour);
    tft.drawFastVLine(19*spacing+4,14*spacing,5,borderColour);
    tft.drawFastVLine(19*spacing+3,14*spacing+11,2,borderColour);
    tft.drawFastVLine(19*spacing+3,14*spacing+7,2,borderColour);
    tft.drawFastVLine(19*spacing+3,14*spacing,5,borderColour);
    tft.drawFastVLine(19*spacing+2,14*spacing+11,2,borderColour);
    tft.drawFastVLine(19*spacing+2,14*spacing+7,2,borderColour);
    tft.drawFastVLine(19*spacing+2,14*spacing+3,2,borderColour);
    tft.drawFastVLine(19*spacing+1,14*spacing+7,5,borderColour);
    tft.drawFastVLine(19*spacing+1,14*spacing+3,2,borderColour);
    tft.drawFastVLine(19*spacing,14*spacing+7,4,borderColour);
    tft.drawFastVLine(19*spacing,14*spacing+3,2,borderColour);
  }
}

void drawTile(int xPos,int yPos,int tileType) {
  int lightGrey = 0x52A9;
  int darkGrey = 0x3A07;
  int red = 0xD041; 
  int hotPink = 0xC808;
  int orange = 0xFD25;
  int yellow = 0xFEC5;
  //Floor
  if(tileType == 0){
    tft.fillRect(xPos+1,yPos+1,2,15,0x8280);
    tft.fillRect(xPos+5,yPos,2,16,0x8280);
    tft.fillRect(xPos+9,yPos,2,16,0x8280);
    tft.fillRect(xPos+13,yPos,2,16,0x8280);
    tft.drawFastVLine(xPos,yPos,16,0x4140);
    tft.drawFastVLine(xPos+15,yPos,16,0x4140);
    tft.fillRect(xPos+3,yPos,2,16,0x4140);
    tft.fillRect(xPos+7,yPos,2,16,0x4140);
    tft.fillRect(xPos+11,yPos,2,16,0x4140);
    tft.drawFastHLine(xPos+1,yPos,2,0x4140);
    tft.drawFastHLine(xPos+1,yPos+9,2,0x4140);
    tft.drawFastHLine(xPos+5,yPos+4,2,0x4140);
    tft.drawFastHLine(xPos+5,yPos+13,2,0x4140);
    tft.drawFastHLine(xPos+9,yPos+7,2,0x4140);
    tft.drawFastHLine(xPos+9,yPos+15,2,0x4140);
    tft.drawFastHLine(xPos+13,yPos+2,2,0x4140);
    tft.drawFastHLine(xPos+13,yPos+12,2,0x4140);
  }
  //Wood Planks
  else if(tileType == 1){
    tft.fillRect(xPos+1,yPos+1,2,15,0xD460);
    tft.fillRect(xPos+5,yPos,2,16,0xD460);
    tft.fillRect(xPos+9,yPos,2,16,0xD460);
    tft.fillRect(xPos+13,yPos,2,16,0xD460);
    tft.drawFastVLine(xPos,yPos,16,0x9320);
    tft.drawFastVLine(xPos+15,yPos,16,0x9320);
    tft.fillRect(xPos+3,yPos,2,16,0x9320);
    tft.fillRect(xPos+7,yPos,2,16,0x9320);
    tft.fillRect(xPos+11,yPos,2,16,0x9320);
    tft.drawFastHLine(xPos+1,yPos,2,0x9320);
    tft.drawFastHLine(xPos+1,yPos+9,2,0x9320);
    tft.drawFastHLine(xPos+5,yPos+4,2,0x9320);
    tft.drawFastHLine(xPos+5,yPos+13,2,0x9320);
    tft.drawFastHLine(xPos+9,yPos+7,2,0x9320);
    tft.drawFastHLine(xPos+9,yPos+15,2,0x9320);
    tft.drawFastHLine(xPos+13,yPos+2,2,0x9320);
    tft.drawFastHLine(xPos+13,yPos+12,2,0x9320);
  }
  //Boss Floor
  else if(tileType == 2){
    int outC = 0x8A81;
    int inC = 0x3185;
    tft.fillRect(xPos,yPos,spacing,spacing,outC);//Outer bits
    tft.fillRect(xPos+1,yPos+1,4,4,inC);//Inner stones
    tft.fillRect(xPos+7,yPos+1,3,3,inC);
    tft.fillRect(xPos+12,yPos+1,3,3,inC);
    tft.fillRect(xPos+1,yPos+7,6,3,inC);
    tft.fillRect(xPos+9,yPos+6,6,3,inC);
    tft.fillRect(xPos+1,yPos+12,8,3,inC);
    tft.fillRect(xPos+11,yPos+11,4,4,inC);
  }
  //Acid Traps
  else if(tileType == 3){
    int colour = 0x31E0;
    tft.fillRect(xPos+1,yPos+1,2,15,0x8280);
    tft.fillRect(xPos+5,yPos,2,16,0x8280);
    tft.fillRect(xPos+9,yPos,2,16,0x8280);
    tft.fillRect(xPos+13,yPos,2,16,0x8280);
    tft.drawFastVLine(xPos,yPos,16,colour);
    tft.drawFastVLine(xPos+15,yPos,16,colour);
    tft.fillRect(xPos+3,yPos,2,16,colour);
    tft.fillRect(xPos+7,yPos,2,16,colour);
    tft.fillRect(xPos+11,yPos,2,16,colour);
    tft.drawFastHLine(xPos+1,yPos,2,colour);
    tft.drawFastHLine(xPos+1,yPos+9,2,colour);
    tft.drawFastHLine(xPos+5,yPos+4,2,colour);
    tft.drawFastHLine(xPos+5,yPos+13,2,colour);
    tft.drawFastHLine(xPos+9,yPos+7,2,colour);
    tft.drawFastHLine(xPos+9,yPos+15,2,colour);
    tft.drawFastHLine(xPos+13,yPos+2,2,colour);
    tft.drawFastHLine(xPos+13,yPos+12,2,colour);
  }
  //Boss lava
  else if(tileType == 4){
    int outC = 0x71E1;
    int inC = 0x39C6;
    tft.fillRect(xPos,yPos,spacing,spacing,outC);//Outer bits
    tft.fillRect(xPos+1,yPos+1,4,4,inC);//Inner stones
    tft.fillRect(xPos+7,yPos+1,3,3,inC);
    tft.fillRect(xPos+12,yPos+1,3,3,inC);
    tft.fillRect(xPos+1,yPos+7,6,3,inC);
    tft.fillRect(xPos+9,yPos+6,6,3,inC);
    tft.fillRect(xPos+1,yPos+12,8,3,inC);
    tft.fillRect(xPos+11,yPos+11,4,4,inC);
  }
  //Heart
  else if(tileType == 5){
    if(Characters[0].row == 1 && Characters[0].col ==1)
      drawTile(xPos,yPos,1);
    else
      drawTile(xPos,yPos,0);
    int heartGold = 0xED46;
    int heartRed = 0xD000;
    tft.drawFastVLine(xPos,yPos+4,3,heartGold);
    tft.drawFastVLine(xPos+1,yPos+3,5,heartGold);
    tft.drawFastVLine(xPos+2,yPos+2,7,heartGold);
    tft.drawFastVLine(xPos+3,yPos+2,8,heartGold);
    tft.drawFastVLine(xPos+4,yPos+2,9,heartGold);
    tft.drawFastVLine(xPos+5,yPos+3,9,heartGold);
    tft.drawFastVLine(xPos+6,yPos+4,9,heartGold);
    tft.drawFastVLine(xPos+7,yPos+5,9,heartGold);
    tft.drawFastVLine(xPos+8,yPos+5,9,heartGold);
    tft.drawFastVLine(xPos+9,yPos+4,9,heartGold);
    tft.drawFastVLine(xPos+10,yPos+3,9,heartGold);
    tft.drawFastVLine(xPos+11,yPos+2,9,heartGold);
    tft.drawFastVLine(xPos+12,yPos+2,8,heartGold);
    tft.drawFastVLine(xPos+13,yPos+2,7,heartGold);
    tft.drawFastVLine(xPos+14,yPos+3,5,heartGold);
    tft.drawFastVLine(xPos+15,yPos+4,3,heartGold);
    //Red
    tft.drawFastVLine(xPos+2,yPos+4,3,heartRed);
    tft.drawFastVLine(xPos+3,yPos+3,5,heartRed);
    tft.drawFastVLine(xPos+4,yPos+3,6,heartRed);
    tft.drawFastVLine(xPos+5,yPos+4,6,heartRed);
    tft.drawFastVLine(xPos+6,yPos+5,6,heartRed);
    tft.drawFastVLine(xPos+7,yPos+6,6,heartRed);
    tft.drawFastVLine(xPos+8,yPos+6,6,heartRed);
    tft.drawFastVLine(xPos+9,yPos+5,6,heartRed);
    tft.drawFastVLine(xPos+10,yPos+4,6,heartRed);
    tft.drawFastVLine(xPos+11,yPos+3,6,heartRed);
    tft.drawFastVLine(xPos+12,yPos+3,5,heartRed);
    tft.drawFastVLine(xPos+13,yPos+4,3,heartRed);
  }
  else if(tileType == 7) {
    drawTile(xPos,yPos,0);
    tft.drawLine(xPos,yPos+14,xPos+8,yPos+6,0x9DB7);
    tft.drawLine(xPos,yPos+15,xPos+15,yPos,0x9DB7);
    tft.drawLine(xPos+1,yPos+15,xPos+9,yPos+7,0x9DB7);
    tft.drawFastHLine(xPos+10,yPos,2,0x9DB7);
    tft.drawFastHLine(xPos+8,yPos+1,2,0x9DB7);
    tft.drawFastHLine(xPos+7,yPos+2,2,0x9DB7);
    tft.drawFastHLine(xPos+7,yPos+3,2,0x9DB7);
    tft.drawFastHLine(xPos+8,yPos+4,5,0x9DB7);
    tft.drawFastVLine(xPos+11,yPos+3,5,0x9DB7);
    tft.drawFastHLine(xPos+12,yPos+7,2,0x9DB7);
    tft.drawFastHLine(xPos+12,yPos+8,2,0x9DB7);
    tft.drawFastVLine(xPos+14,yPos+6,2,0x9DB7);
    tft.drawFastVLine(xPos+15,yPos+4,2,0x9DB7);
  }
  //Walls
  else if((tileType >= 10 && tileType <= 13)) {
    tft.fillRect(xPos,yPos,spacing,spacing,0x5AEB);//Darker
    tft.fillRect(xPos+1,yPos+1,4,4,0x8430);//Lighter
    tft.fillRect(xPos+7,yPos+1,3,3,0x8430);
    tft.fillRect(xPos+12,yPos+1,3,3,0x8430);
    tft.fillRect(xPos+1,yPos+7,6,3,0x8430);
    tft.fillRect(xPos+9,yPos+6,6,3,0x8430);
    tft.fillRect(xPos+1,yPos+12,8,3,0x8430);
    tft.fillRect(xPos+11,yPos+11,4,4,0x8430);
  }
  //Water
  else if(tileType ==14){
    int waterColour = 0x1412;
    tft.fillRect(xPos,yPos,spacing,spacing,0x02D4);
    tft.drawFastVLine(xPos+8,yPos,2,waterColour);
    tft.drawFastVLine(xPos+9,yPos+1,2,waterColour);
    tft.drawPixel(xPos+10,yPos+2,waterColour);
    tft.drawFastHLine(xPos+11,yPos+3,3,waterColour);
    tft.drawFastHLine(xPos+14,yPos+4,2,waterColour);
    tft.drawPixel(xPos+15,yPos+5,waterColour);
    tft.drawPixel(xPos+2,yPos+1,waterColour);
    tft.drawFastVLine(xPos+3,yPos,3,waterColour);
    tft.drawFastHLine(xPos+3,yPos+1,4,waterColour);
    tft.drawPixel(xPos+5,yPos+3,waterColour);
    tft.drawFastHLine(xPos+6,yPos+4,3,waterColour);
    tft.drawFastVLine(xPos+9,yPos+5,3,waterColour);
    tft.drawPixel(xPos+10,yPos+6,waterColour);
    tft.drawFastHLine(xPos+11,yPos+7,2,waterColour);
    tft.drawPixel(xPos+12,yPos+8,waterColour);
    tft.drawFastHLine(xPos+13,yPos+9,2,waterColour);
    tft.drawPixel(xPos+15,yPos+10,waterColour);
    tft.drawFastHLine(xPos,yPos+2,2,waterColour);
    tft.drawPixel(xPos,yPos+3,waterColour);
    tft.drawFastVLine(xPos,yPos+5,3,waterColour);
    tft.drawPixel(xPos+1,yPos+7,waterColour);
    tft.drawPixel(xPos+2,yPos+8,waterColour);
    tft.drawPixel(xPos+4,yPos+8,waterColour);
    tft.drawFastHLine(xPos+2,yPos+9,5,waterColour);
    tft.drawFastHLine(xPos+5,yPos+10,4,waterColour);
    tft.drawFastHLine(xPos+8,yPos+11,2,waterColour);
    tft.drawFastHLine(xPos+10,yPos+12,2,waterColour);
    tft.drawFastVLine(xPos+12,yPos+13,2,waterColour);
    tft.drawPixel(xPos+13,yPos+14,waterColour);
    tft.drawFastHLine(xPos+14,yPos+15,2,waterColour);
    tft.drawFastHLine(xPos,yPos+12,3,waterColour);
    tft.drawFastHLine(xPos+2,yPos+13,2,waterColour);
    tft.drawPixel(xPos+4,yPos+14,waterColour);
    tft.drawFastHLine(xPos+4,yPos+15,4,waterColour);
    tft.drawPixel(xPos+7,yPos+12,waterColour);
    tft.drawPixel(xPos+8,yPos+13,waterColour);
    tft.drawFastVLine(xPos+9,yPos+14,2,waterColour);
  }
  //Lava
  else if(tileType == 15 || tileType == 51 || tileType == 52){
    tft.fillRect(xPos,yPos,16,16,0xFD42);
    tft.fillRect(xPos,yPos,2,4,0xFB40);
    tft.fillRect(xPos+4,yPos,8,2,0xFB40);
    tft.fillRect(xPos+6,yPos+2,4,2,0xFB40);
    tft.fillRect(xPos+2,yPos+4,4,8,0xFB40);
    tft.fillRect(xPos,yPos+6,8,4,0xFB40);
    tft.fillRect(xPos+10,yPos+4,4,4,0xFB40);
    tft.fillRect(xPos+14,yPos,2,4,0xFB40);
    tft.fillRect(xPos+12,yPos+10,4,4,0xFB40);
    tft.fillRect(xPos+6,yPos+12,4,2,0xFB40);
    tft.fillRect(xPos+4,yPos+14,8,2,0xFB40);
  }
  //Boss Almost Lava
  else if(tileType == 16 || tileType == 50){
    int outC = 0xD3C1;
    int inC = 0x6120;
    tft.fillRect(xPos,yPos,spacing,spacing,outC);//Outer bits
    tft.fillRect(xPos+1,yPos+1,4,4,inC);//Inner stones
    tft.fillRect(xPos+7,yPos+1,3,3,inC);
    tft.fillRect(xPos+12,yPos+1,3,3,inC);
    tft.fillRect(xPos+1,yPos+7,6,3,inC);
    tft.fillRect(xPos+9,yPos+6,6,3,inC);
    tft.fillRect(xPos+1,yPos+12,8,3,inC);
    tft.fillRect(xPos+11,yPos+11,4,4,inC);
  }
  //Empty Heart
  else if(tileType == 30){
    tft.fillRect(xPos,yPos,spacing,spacing,0x0000);
  }
  //Quarter Heart
  else if(tileType == 31){
    int heartRed = 0xD000;
    tft.drawFastVLine(xPos+2,yPos+4,4,heartRed);
    tft.drawFastVLine(xPos+3,yPos+3,5,heartRed);
    tft.drawFastVLine(xPos+4,yPos+3,5,heartRed);
    tft.drawFastVLine(xPos+5,yPos+3,5,heartRed);
    tft.drawFastVLine(xPos+6,yPos+4,4,heartRed);
    tft.drawFastVLine(xPos+7,yPos+5,3,heartRed);
  }
  //Half Heart
  else if(tileType == 32){
    int heartRed = 0xD000;
    tft.drawFastVLine(xPos+2,yPos+4,4,heartRed);
    tft.drawFastVLine(xPos+3,yPos+3,6,heartRed);
    tft.drawFastVLine(xPos+4,yPos+3,7,heartRed);
    tft.drawFastVLine(xPos+5,yPos+3,8,heartRed);
    tft.drawFastVLine(xPos+6,yPos+4,8,heartRed);
    tft.drawFastVLine(xPos+7,yPos+5,8,heartRed);
  }
  //3 Quarters of a Heart
  else if(tileType == 33){
    int heartRed = 0xD000;
    tft.drawFastVLine(xPos+2,yPos+4,4,heartRed);
    tft.drawFastVLine(xPos+3,yPos+3,6,heartRed);
    tft.drawFastVLine(xPos+4,yPos+3,7,heartRed);
    tft.drawFastVLine(xPos+5,yPos+3,8,heartRed);
    tft.drawFastVLine(xPos+6,yPos+4,8,heartRed);
    tft.drawFastVLine(xPos+7,yPos+5,8,heartRed);
    tft.drawFastVLine(xPos+8,yPos+8,5,heartRed);
    tft.drawFastVLine(xPos+9,yPos+8,4,heartRed);
    tft.drawFastVLine(xPos+10,yPos+8,3,heartRed);
    tft.drawFastVLine(xPos+11,yPos+8,2,heartRed);
    tft.drawFastVLine(xPos+12,yPos+8,1,heartRed);
  }
  //Full Heart
  else if(tileType == 34){
    int heartRed = 0xD000;
    tft.drawFastVLine(xPos+2,yPos+4,4,heartRed);
    tft.drawFastVLine(xPos+3,yPos+3,6,heartRed);
    tft.drawFastVLine(xPos+4,yPos+3,7,heartRed);
    tft.drawFastVLine(xPos+5,yPos+3,8,heartRed);
    tft.drawFastVLine(xPos+6,yPos+4,8,heartRed);
    tft.drawFastVLine(xPos+7,yPos+5,8,heartRed);
    tft.drawFastVLine(xPos+8,yPos+5,8,heartRed);
    tft.drawFastVLine(xPos+9,yPos+4,8,heartRed);
    tft.drawFastVLine(xPos+10,yPos+3,8,heartRed);
    tft.drawFastVLine(xPos+11,yPos+3,7,heartRed);
    tft.drawFastVLine(xPos+12,yPos+3,6,heartRed);
    tft.drawFastVLine(xPos+13,yPos+4,4,heartRed);
  }
  else if(tileType == 60){
    drawTile(xPos,yPos,10);
    //Block 1/9
    tft.drawFastVLine(xPos+6,yPos+7,5,lightGrey);
    tft.drawFastVLine(xPos+7,yPos+5,3,lightGrey);
    tft.drawFastVLine(xPos+8,yPos+4,2,lightGrey);
    tft.drawFastVLine(xPos+9,yPos+3,2,lightGrey);
    tft.drawFastVLine(xPos+10,yPos+2,2,lightGrey);
    tft.drawFastVLine(xPos+11,yPos+1,2,lightGrey);
    tft.drawFastVLine(xPos+12,yPos,5,lightGrey);
    tft.drawFastVLine(xPos+13,yPos+4,3,lightGrey);
    tft.drawFastVLine(xPos+14,yPos+6,3,lightGrey);
    tft.drawFastVLine(xPos+15,yPos+8,2,lightGrey);
    tft.drawFastVLine(xPos+7,yPos+11,3,lightGrey);
    tft.drawFastVLine(xPos+8,yPos+13,2,lightGrey);
    tft.drawFastVLine(xPos+9,yPos+14,2,lightGrey);
    tft.drawFastHLine(xPos+10,yPos+15,2,lightGrey);
    //Dark Grey
    tft.drawFastVLine(xPos+7,yPos+8,3,darkGrey);
    tft.drawFastVLine(xPos+8,yPos+6,7,darkGrey);
    tft.drawFastVLine(xPos+9,yPos+5,9,darkGrey);
    tft.drawFastVLine(xPos+10,yPos+4,11,darkGrey);
    tft.drawFastVLine(xPos+11,yPos+3,12,darkGrey);
    tft.drawFastVLine(xPos+12,yPos+5,11,darkGrey);
    tft.drawFastVLine(xPos+13,yPos+7,9,darkGrey);
    tft.drawFastVLine(xPos+14,yPos+9,7,darkGrey);
    tft.drawFastVLine(xPos+15,yPos+10,6,darkGrey);
  }
  //Block 2/9
  else if(tileType == 61){
    drawTile(xPos,yPos,10);
    //Light Gray Crown
    tft.drawFastVLine(xPos+5,yPos+7,2,lightGrey);
    tft.drawFastVLine(xPos+6,yPos+5,6,lightGrey);
    tft.drawFastVLine(xPos+7,yPos+4,7,lightGrey);
    tft.drawFastVLine(xPos+8,yPos+4,7,lightGrey);
    tft.drawFastVLine(xPos+9,yPos+5,6,lightGrey);
    tft.drawFastVLine(xPos+10,yPos+7,2,lightGrey);
    //Horangens
    tft.drawFastHLine(xPos,yPos+9,2,lightGrey);
    tft.drawFastHLine(xPos+14,yPos+9,2,lightGrey);
    tft.drawFastHLine(xPos+1,yPos+10,14,lightGrey);
    tft.drawFastHLine(xPos+2,yPos+11,12,lightGrey);
    tft.drawFastHLine(xPos+3,yPos+13,10,lightGrey);
    tft.drawFastHLine(xPos+2,yPos+14,2,lightGrey);
    tft.drawFastHLine(xPos+12,yPos+14,2,lightGrey);
    tft.drawFastHLine(xPos,yPos+15,3,lightGrey);
    tft.drawFastHLine(xPos+13,yPos+15,3,lightGrey);
    tft.drawFastVLine(xPos+6,yPos+11,4,lightGrey);
    tft.drawFastVLine(xPos+9,yPos+11,4,lightGrey);
    //Dark Grey Crown and Horangens
    tft.fillRect(xPos+7,yPos+6,2,3,darkGrey);
    tft.drawFastVLine(xPos,yPos+10,5,darkGrey);
    tft.drawFastVLine(xPos+15,yPos+10,5,darkGrey);
    tft.drawFastVLine(xPos+1,yPos+11,4,darkGrey);
    tft.drawFastVLine(xPos+14,yPos+11,4,darkGrey);
    tft.drawFastVLine(xPos+2,yPos+12,2,darkGrey);
    tft.drawFastVLine(xPos+13,yPos+12,2,darkGrey);
    tft.drawFastHLine(xPos+3,yPos+12,3,darkGrey);
    tft.drawFastHLine(xPos+10,yPos+12,3,darkGrey);
    //Red
    tft.drawFastHLine(xPos+4,yPos+10,3,red);
    tft.drawFastHLine(xPos+9,yPos+10,3,red);
    tft.drawPixel(xPos+5,yPos+9,red);
    tft.drawPixel(xPos+10,yPos+9,red);
    //HotPink
    tft.fillRect(xPos+7,yPos+11,2,2,hotPink);
    //Yellow
    tft.fillRect(xPos+7,yPos+13,2,3,yellow);
    tft.drawFastHLine(xPos+5,yPos+15,6,yellow);
    //orangeange
    tft.drawFastHLine(xPos+4,yPos+14,2,orange);
    tft.drawFastHLine(xPos+10,yPos+14,2,orange);
    tft.drawFastHLine(xPos+3,yPos+15,2,orange);
    tft.drawFastHLine(xPos+11,yPos+15,2,orange);
  }
  //Block 3/9
  else if(tileType == 62){
    drawTile(xPos,yPos,10);
    //Light Grey
    tft.drawFastVLine(xPos+9,yPos+7,5,lightGrey);
    tft.drawFastVLine(xPos+8,yPos+5,3,lightGrey);
    tft.drawFastVLine(xPos+7,yPos+4,2,lightGrey);
    tft.drawFastVLine(xPos+6,yPos+3,2,lightGrey);
    tft.drawFastVLine(xPos+5,yPos+2,2,lightGrey);
    tft.drawFastVLine(xPos+4,yPos+1,2,lightGrey);
    tft.drawFastVLine(xPos+3,yPos,5,lightGrey);
    tft.drawFastVLine(xPos+2,yPos+4,3,lightGrey);
    tft.drawFastVLine(xPos+1,yPos+6,3,lightGrey);
    tft.drawFastVLine(xPos,yPos+8,2,lightGrey);
    tft.drawFastVLine(xPos+8,yPos+11,3,lightGrey);
    tft.drawFastVLine(xPos+7,yPos+13,2,lightGrey);
    tft.drawFastVLine(xPos+6,yPos+14,2,lightGrey);
    tft.drawFastHLine(xPos+4,yPos+15,2,lightGrey);
    //Dark Grey
    tft.drawFastVLine(xPos+8,yPos+8,3,darkGrey);
    tft.drawFastVLine(xPos+7,yPos+6,7,darkGrey);
    tft.drawFastVLine(xPos+6,yPos+5,9,darkGrey);
    tft.drawFastVLine(xPos+5,yPos+4,11,darkGrey);
    tft.drawFastVLine(xPos+4,yPos+3,12,darkGrey);
    tft.drawFastVLine(xPos+3,yPos+5,11,darkGrey);
    tft.drawFastVLine(xPos+2,yPos+7,9,darkGrey);
    tft.drawFastVLine(xPos+1,yPos+9,7,darkGrey);
    tft.drawFastVLine(xPos,yPos+10,6,darkGrey);
  }
  //Block 4/9
  else if(tileType == 63){
    drawTile(xPos,yPos,16);
    //Light Grey
    tft.drawFastHLine(xPos+11,yPos,5,lightGrey);
    tft.drawFastVLine(xPos+6,yPos+3,2,lightGrey);
    tft.drawFastVLine(xPos+5,yPos+4,2,lightGrey);
    tft.drawFastVLine(xPos+4,yPos+5,2,lightGrey);
    tft.drawFastVLine(xPos+3,yPos+6,3,lightGrey);
    tft.drawFastVLine(xPos+2,yPos+8,2,lightGrey);
    tft.drawFastHLine(xPos+1,yPos+10,2,lightGrey);
    tft.drawFastHLine(xPos+1,yPos+11,6,lightGrey);
    tft.drawFastHLine(xPos+6,yPos+12,7,lightGrey);
    tft.drawFastVLine(xPos+7,yPos+4,2,lightGrey);
    tft.drawFastVLine(xPos+8,yPos+5,2,lightGrey);
    tft.drawFastHLine(xPos+9,yPos+6,2,lightGrey);
    tft.drawFastHLine(xPos+10,yPos+7,3,lightGrey);
    tft.drawFastVLine(xPos+12,yPos+8,2,lightGrey);
    tft.drawFastVLine(xPos+13,yPos+9,3,lightGrey);
    tft.drawFastVLine(xPos+12,yPos+11,2,lightGrey);
    //Dark Grey
    tft.drawFastHLine(xPos+6,yPos+5,1,darkGrey);
    tft.drawFastHLine(xPos+5,yPos+6,3,darkGrey);
    tft.drawFastHLine(xPos+4,yPos+7,6,darkGrey);
    tft.drawFastHLine(xPos+4,yPos+8,8,darkGrey);
    tft.drawFastHLine(xPos+3,yPos+9,9,darkGrey);
    tft.drawFastHLine(xPos+3,yPos+10,10,darkGrey);
    tft.drawFastHLine(xPos+7,yPos+11,6,darkGrey);
    //orangeange
    tft.drawFastVLine(xPos+10,yPos,2,orange);
    tft.drawFastHLine(xPos+11,yPos+1,5,orange);
    tft.drawPixel(xPos+7,yPos+3,orange);
    tft.drawPixel(xPos+8,yPos+4,orange);
    tft.drawFastHLine(xPos+9,yPos+5,2,orange);
    tft.drawFastHLine(xPos+11,yPos+6,2,orange);
    tft.drawFastVLine(xPos+13,yPos+7,2,orange);
    tft.drawFastVLine(xPos+14,yPos+9,3,orange);
    tft.drawPixel(xPos+13,yPos+12,orange);
    tft.drawFastHLine(xPos,yPos+12,6,orange);
    tft.drawFastHLine(xPos+5,yPos+13,8,orange);
    tft.drawFastVLine(xPos,yPos+14,2,orange);
    //Yellow
    tft.drawFastHLine(xPos+8,yPos,2,yellow);
    tft.drawPixel(xPos+9,yPos+1,yellow);
    tft.drawFastHLine(xPos+10,yPos+2,6,yellow);
    tft.drawFastHLine(xPos+6,yPos+2,2,yellow);
    tft.drawPixel(xPos+7,yPos+1,yellow);
    tft.drawPixel(xPos+8,yPos+3,yellow);
    tft.drawPixel(xPos+9,yPos+4,yellow);
    tft.drawFastHLine(xPos+13,yPos+4,3,yellow);
    tft.drawFastHLine(xPos+14,yPos+5,2,yellow);
    tft.drawFastVLine(xPos+15,yPos+6,2,yellow);
    tft.drawFastHLine(xPos+2,yPos+13,3,yellow);
    tft.drawFastHLine(xPos+5,yPos+14,8,yellow);
    tft.drawFastHLine(xPos+13,yPos+13,2,yellow);
    tft.drawPixel(xPos+14,yPos+12,yellow);
    tft.drawPixel(xPos+15,yPos+15,yellow);
    tft.drawFastHLine(xPos+1,yPos+15,3,yellow);
    //Red
    tft.drawPixel(xPos+8,yPos+1,red);
    tft.drawFastHLine(xPos+8,yPos+2,2,red);
    tft.drawFastHLine(xPos+9,yPos+3,7,red);
    tft.drawFastHLine(xPos+10,yPos+4,3,red);
    tft.drawFastHLine(xPos+12,yPos+5,2,red);
    tft.drawFastHLine(xPos+13,yPos+6,2,red);
    tft.drawFastVLine(xPos+14,yPos+7,2,red);
    tft.drawFastVLine(xPos+15,yPos+8,7,red);
    tft.drawFastHLine(xPos,yPos+13,2,red);
    tft.drawFastHLine(xPos+1,yPos+14,4,red);
    tft.drawFastHLine(xPos+4,yPos+15,11,red);
    tft.drawFastHLine(xPos+13,yPos+14,3,red);
  }
  //Block 5/9
  else if(tileType == 64){
    drawTile(xPos,yPos,16);
    //Light Grey
    tft.drawPixel(xPos,yPos,lightGrey);
    tft.drawPixel(xPos+15,yPos,lightGrey);
    tft.drawFastHLine(xPos+6,yPos+5,4,lightGrey);
    tft.drawFastHLine(xPos+2,yPos+6,12,lightGrey);
    tft.drawFastHLine(xPos+2,yPos+7,12,lightGrey);
    tft.drawFastHLine(xPos+3,yPos+8,10,lightGrey);
    tft.drawFastHLine(xPos+5,yPos+9,6,lightGrey);
    tft.drawFastHLine(xPos+5,yPos+13,6,lightGrey);
    tft.drawFastHLine(xPos+4,yPos+14,8,lightGrey);
    tft.drawFastHLine(xPos+3,yPos+15,10,lightGrey);
    //Dark Grey
    tft.drawFastHLine(xPos+6,yPos+6,4,darkGrey);
    tft.drawFastHLine(xPos+4,yPos+7,8,darkGrey);
    tft.drawFastHLine(xPos+5,yPos+8,6,darkGrey);
    tft.drawFastHLine(xPos+7,yPos+9,2,darkGrey);
    tft.drawFastHLine(xPos+6,yPos+14,4,darkGrey);
    tft.drawFastHLine(xPos+5,yPos+15,6,darkGrey);
    //orangeange
    tft.drawFastHLine(xPos+1,yPos,4,orange);
    tft.drawFastHLine(xPos,yPos+1,3,orange);
    tft.drawFastHLine(xPos+11,yPos,4,orange);
    tft.drawFastHLine(xPos+13,yPos+1,3,orange);
    tft.drawFastHLine(xPos+6,yPos+4,4,orange);
    tft.drawFastHLine(xPos,yPos+5,6,orange);
    tft.drawFastHLine(xPos+10,yPos+5,6,orange);
    tft.drawFastHLine(xPos,yPos+6,2,orange);
    tft.drawFastHLine(xPos+14,yPos+6,2,orange);
    tft.drawPixel(xPos+1,yPos+7,orange);
    tft.drawPixel(xPos+14,yPos+7,orange);
    tft.drawFastHLine(xPos+1,yPos+8,2,orange);
    tft.drawFastHLine(xPos+13,yPos+8,2,orange);
    tft.drawFastHLine(xPos+2,yPos+9,3,orange);
    tft.drawFastHLine(xPos+11,yPos+9,3,orange);
    tft.drawFastHLine(xPos+5,yPos+10,6,orange);
    tft.drawFastHLine(xPos+4,yPos+12,8,orange);
    tft.drawFastHLine(xPos+3,yPos+13,2,orange);
    tft.drawFastHLine(xPos+11,yPos+13,2,orange);
    tft.drawFastHLine(xPos+2,yPos+14,2,orange);
    tft.drawFastHLine(xPos+12,yPos+14,2,orange);
    tft.drawPixel(xPos+2,yPos+15,orange);
    tft.drawPixel(xPos+13,yPos+15,orange);
    //Draw Yellow
    tft.drawFastHLine(xPos,yPos+2,4,yellow);
    tft.drawFastHLine(xPos+3,yPos+1,4,yellow);
    tft.drawFastHLine(xPos+5,yPos,2,yellow);
    tft.drawFastHLine(xPos+9,yPos,2,yellow);
    tft.drawFastHLine(xPos+9,yPos+1,4,yellow);
    tft.drawFastHLine(xPos+12,yPos+2,4,yellow);
    tft.drawFastHLine(xPos,yPos+4,6,yellow);
    tft.drawFastHLine(xPos+6,yPos+3,4,yellow);
    tft.drawFastHLine(xPos+10,yPos+4,6,yellow);
    tft.drawFastVLine(xPos,yPos+7,3,yellow);
    tft.drawFastVLine(xPos+1,yPos+9,2,yellow);
    tft.drawFastHLine(xPos+2,yPos+10,3,yellow);
    tft.drawFastHLine(xPos+11,yPos+10,3,yellow);
    tft.drawFastVLine(xPos+14,yPos+9,2,yellow);
    tft.drawFastVLine(xPos+15,yPos+7,3,yellow);
    tft.drawFastHLine(xPos+7,yPos+11,2,yellow);
    tft.drawFastHLine(xPos+2,yPos+12,2,yellow);
    tft.drawFastHLine(xPos+12,yPos+12,2,yellow);
    tft.drawFastHLine(xPos+1,yPos+13,2,yellow);
    tft.drawFastHLine(xPos+13,yPos+13,2,yellow);
    tft.drawPixel(xPos+1,yPos+14,yellow);
    tft.drawPixel(xPos+14,yPos+14,yellow);
    tft.drawFastHLine(xPos,yPos+15,2,yellow);
    tft.drawFastHLine(xPos+14,yPos+15,2,yellow);
    //Draw Red
    tft.drawFastHLine(xPos,yPos+3,6,red);
    tft.drawFastHLine(xPos+4,yPos+2,8,red);
    tft.drawFastHLine(xPos+10,yPos+3,6,red);
    tft.fillRect(xPos+7,yPos,2,2,red);
    tft.drawPixel(xPos,yPos+10,red);
    tft.drawPixel(xPos+15,yPos+10,red);
    tft.drawPixel(xPos+1,yPos+12,red);
    tft.drawPixel(xPos+14,yPos+12,red);
    tft.drawFastHLine(xPos+1,yPos+11,6,red);
    tft.drawFastHLine(xPos+9,yPos+11,6,red);
    tft.drawFastVLine(xPos,yPos+13,2,red);
    tft.drawFastVLine(xPos+15,yPos+13,2,red);
    //Draw Pink
    tft.drawFastVLine(xPos,yPos+11,2,hotPink);
    tft.drawFastVLine(xPos+15,yPos+11,2,hotPink);
  }
  //Block 6/9
  else if(tileType == 65){
    drawTile(xPos,yPos,16);
    //Light Grey
    tft.drawFastHLine(xPos,yPos,5,lightGrey);
    tft.drawFastVLine(xPos+9,yPos+3,2,lightGrey);
    tft.drawFastVLine(xPos+10,yPos+4,2,lightGrey);
    tft.drawFastVLine(xPos+11,yPos+5,2,lightGrey);
    tft.drawFastVLine(xPos+12,yPos+6,3,lightGrey);
    tft.drawFastVLine(xPos+13,yPos+8,2,lightGrey);
    tft.drawFastHLine(xPos+13,yPos+10,2,lightGrey);
    tft.drawFastHLine(xPos+9,yPos+11,6,lightGrey);
    tft.drawFastHLine(xPos+3,yPos+12,7,lightGrey);
    tft.drawFastVLine(xPos+8,yPos+4,2,lightGrey);
    tft.drawFastVLine(xPos+7,yPos+5,2,lightGrey);
    tft.drawFastHLine(xPos+5,yPos+6,2,lightGrey);
    tft.drawFastHLine(xPos+3,yPos+7,3,lightGrey);
    tft.drawFastVLine(xPos+3,yPos+8,2,lightGrey);
    tft.drawFastVLine(xPos+2,yPos+9,3,lightGrey);
    tft.drawFastVLine(xPos+3,yPos+11,2,lightGrey);
    //Dark Grey
    tft.drawFastHLine(xPos+9,yPos+5,1,darkGrey);
    tft.drawFastHLine(xPos+8,yPos+6,3,darkGrey);
    tft.drawFastHLine(xPos+6,yPos+7,6,darkGrey);
    tft.drawFastHLine(xPos+4,yPos+8,8,darkGrey);
    tft.drawFastHLine(xPos+4,yPos+9,9,darkGrey);
    tft.drawFastHLine(xPos+3,yPos+10,10,darkGrey);
    tft.drawFastHLine(xPos+4,yPos+11,6,darkGrey);
    //orangeange 
    tft.drawFastVLine(xPos+5,yPos,2,orange);
    tft.drawFastHLine(xPos,yPos+1,5,orange);
    tft.drawPixel(xPos+8,yPos+3,orange);
    tft.drawPixel(xPos+7,yPos+4,orange);
    tft.drawFastHLine(xPos+5,yPos+5,2,orange);
    tft.drawFastHLine(xPos+3,yPos+6,2,orange);
    tft.drawFastVLine(xPos+2,yPos+7,2,orange);
    tft.drawFastVLine(xPos+1,yPos+9,3,orange);
    tft.drawPixel(xPos+2,yPos+12,orange);
    tft.drawFastHLine(xPos+10,yPos+12,6,orange);
    tft.drawFastHLine(xPos+3,yPos+13,8,orange);
    tft.drawFastVLine(xPos+15,yPos+14,2,orange);
    //Yellow
    tft.drawFastHLine(xPos+6,yPos,2,yellow);
    tft.drawPixel(xPos+6,yPos+1,yellow);
    tft.drawFastHLine(xPos,yPos+2,6,yellow);
    tft.drawFastHLine(xPos+8,yPos+2,2,yellow);
    tft.drawPixel(xPos+8,yPos+1,yellow);
    tft.drawPixel(xPos+7,yPos+3,yellow);
    tft.drawPixel(xPos+6,yPos+4,yellow);
    tft.drawFastHLine(xPos,yPos+4,3,yellow);
    tft.drawFastHLine(xPos,yPos+5,2,yellow);
    tft.drawFastVLine(xPos,yPos+6,2,yellow);
    tft.drawFastHLine(xPos+11,yPos+13,3,yellow);
    tft.drawFastHLine(xPos+3,yPos+14,8,yellow);
    tft.drawFastHLine(xPos+1,yPos+13,2,yellow);
    tft.drawPixel(xPos+1,yPos+12,yellow);
    tft.drawPixel(xPos,yPos+15,yellow);
    tft.drawFastHLine(xPos+12,yPos+15,3,yellow);
    //Red
    tft.drawPixel(xPos+7,yPos+1,red);
    tft.drawFastHLine(xPos+6,yPos+2,2,red);
    tft.drawFastHLine(xPos,yPos+3,7,red);
    tft.drawFastHLine(xPos+3,yPos+4,3,red);
    tft.drawFastHLine(xPos+2,yPos+5,2,red);
    tft.drawFastHLine(xPos+1,yPos+6,2,red);
    tft.drawFastVLine(xPos+1,yPos+7,2,red);
    tft.drawFastVLine(xPos,yPos+8,7,red);
    tft.drawFastHLine(xPos+14,yPos+13,2,red);
    tft.drawFastHLine(xPos+11,yPos+14,4,red);
    tft.drawFastHLine(xPos+1,yPos+15,11,red);
    tft.drawFastHLine(xPos,yPos+14,3,red);
  }
  //Block 7/9
  else if(tileType == 66){
    drawTile(xPos,yPos,2);
    //Light Grey
    tft.drawPixel(xPos,yPos,lightGrey);
    tft.drawPixel(xPos+1,yPos+2,lightGrey);
    tft.drawFastHLine(xPos+2,yPos+3,6,lightGrey);
    tft.drawPixel(xPos+9,yPos+2,lightGrey);
    tft.drawFastVLine(xPos+10,yPos+2,3,lightGrey);
    tft.drawPixel(xPos+11,yPos+9,lightGrey);
    tft.drawFastHLine(xPos+10,yPos+10,2,lightGrey);
    tft.drawFastHLine(xPos+8,yPos+11,2,lightGrey);
    tft.drawPixel(xPos+7,yPos+12,lightGrey);
    tft.drawPixel(xPos+12,yPos+11,lightGrey);
    tft.drawPixel(xPos+14,yPos+12,lightGrey);
    //Dark Grey
    tft.drawFastHLine(xPos,yPos+1,3,darkGrey);
    tft.drawFastHLine(xPos+2,yPos+2,7,darkGrey);
    tft.drawFastVLine(xPos,yPos+2,2,darkGrey);
    tft.drawFastVLine(xPos+1,yPos+3,3,darkGrey);
    tft.drawFastVLine(xPos+2,yPos+4,3,darkGrey);
    tft.drawFastVLine(xPos+3,yPos+4,4,darkGrey);
    tft.drawFastVLine(xPos+4,yPos+4,1,darkGrey);
    tft.drawFastVLine(xPos+5,yPos+4,3,darkGrey);
    tft.drawFastVLine(xPos+6,yPos+4,4,darkGrey);
    tft.drawFastVLine(xPos+7,yPos+4,1,darkGrey);
    tft.drawFastVLine(xPos+8,yPos+3,4,darkGrey);
    tft.drawFastVLine(xPos+9,yPos+3,5,darkGrey);
    tft.drawFastVLine(xPos+10,yPos+5,1,darkGrey);
    tft.drawFastVLine(xPos+6,yPos+14,2,darkGrey);
    tft.drawFastVLine(xPos+7,yPos+13,3,darkGrey);
    tft.drawFastVLine(xPos+8,yPos+12,3,darkGrey);
    tft.drawFastVLine(xPos+9,yPos+12,2,darkGrey);
    tft.drawFastVLine(xPos+10,yPos+11,5,darkGrey);
    tft.drawFastVLine(xPos+11,yPos+11,4,darkGrey);
    tft.drawFastVLine(xPos+12,yPos+12,2,darkGrey);
    tft.drawFastVLine(xPos+13,yPos+12,4,darkGrey);
    tft.drawFastVLine(xPos+14,yPos+13,2,darkGrey);
    tft.drawFastVLine(xPos+15,yPos+13,1,darkGrey);
    //orangeange
    tft.drawFastHLine(xPos+1,yPos,3,orange);
    tft.drawFastHLine(xPos+3,yPos+1,9,orange);
    tft.drawFastVLine(xPos+11,yPos+1,3,orange);
    tft.drawFastVLine(xPos+15,yPos+3,2,orange);
    tft.drawFastHLine(xPos+12,yPos+10,2,orange);
    tft.drawFastHLine(xPos+13,yPos+11,3,orange);
    tft.drawPixel(xPos+15,yPos+12,orange);
    //Yellow
    tft.drawFastHLine(xPos+4,yPos,8,yellow);
    tft.drawFastVLine(xPos+12,yPos+1,3,yellow);
    tft.drawPixel(xPos+11,yPos+4,yellow);
    tft.drawFastVLine(xPos+15,yPos,3,yellow);
    tft.drawFastVLine(xPos+14,yPos+3,3,yellow);
    tft.drawPixel(xPos+15,yPos+5,yellow);
    tft.drawFastHLine(xPos+12,yPos+9,2,yellow);
    tft.drawFastHLine(xPos+14,yPos+10,2,yellow);
    //Red
    tft.drawPixel(xPos+12,yPos,red);
    tft.drawFastVLine(xPos+14,yPos,3,red);
    tft.drawFastVLine(xPos+13,yPos+3,3,red);
    tft.drawFastVLine(xPos+11,yPos+5,2,red);
    tft.drawFastVLine(xPos+12,yPos+6,3,red);
    tft.drawFastVLine(xPos+15,yPos+6,2,red);
    tft.drawPixel(xPos+14,yPos+6,red);
    tft.drawFastHLine(xPos+13,yPos+8,2,red);
    tft.drawFastHLine(xPos+14,yPos+9,2,red);
    tft.drawFastVLine(xPos+9,yPos+14,2,red);
    tft.drawFastVLine(xPos+12,yPos+14,2,red);
    tft.drawFastVLine(xPos+15,yPos+14,2,red);
    tft.drawPixel(xPos+8,yPos+15,red);
    tft.drawPixel(xPos+11,yPos+15,red);
    tft.drawPixel(xPos+14,yPos+15,red);
    //Pink
    tft.drawFastVLine(xPos+13,yPos,3,hotPink);
    tft.drawFastVLine(xPos+12,yPos+4,2,hotPink);
    tft.drawFastVLine(xPos+13,yPos+6,2,hotPink);
    tft.drawPixel(xPos+14,yPos+7,hotPink);
    tft.drawPixel(xPos+15,yPos+8,hotPink);
  }
  //Block 8/9
  else if(tileType == 67){
    drawTile(xPos,yPos,2);
    //Light Grey
    tft.drawFastHLine(xPos+2,yPos,2,lightGrey);
    tft.drawFastHLine(xPos+12,yPos,2,lightGrey);
    tft.drawFastHLine(xPos+1,yPos+1,2,lightGrey);
    tft.drawFastHLine(xPos+13,yPos+1,2,lightGrey);
    tft.drawPixel(xPos+1,yPos+2,lightGrey);
    tft.drawPixel(xPos+14,yPos+2,lightGrey);
    tft.drawFastHLine(xPos,yPos+3,2,lightGrey);
    tft.drawFastHLine(xPos+14,yPos+3,2,lightGrey);
    tft.drawFastHLine(xPos,yPos+4,5,lightGrey);
    tft.drawFastHLine(xPos+11,yPos+4,5,lightGrey);
    tft.drawFastHLine(xPos+4,yPos+5,2,lightGrey);
    tft.drawFastHLine(xPos+10,yPos+5,2,lightGrey);
    tft.drawPixel(xPos+6,yPos+6,lightGrey);
    tft.drawPixel(xPos+9,yPos+6,lightGrey);
    tft.drawFastHLine(xPos+7,yPos+7,2,lightGrey);
    tft.drawFastVLine(xPos+1,yPos+13,2,lightGrey);
    tft.drawFastVLine(xPos+14,yPos+13,2,lightGrey);
    //Dark Grey
    tft.drawFastHLine(xPos+4,yPos,8,darkGrey);
    tft.drawFastHLine(xPos+3,yPos+1,10,darkGrey);
    tft.drawFastHLine(xPos+2,yPos+2,12,darkGrey);
    tft.drawFastHLine(xPos+2,yPos+3,12,darkGrey);
    tft.drawFastHLine(xPos+5,yPos+4,6,darkGrey);
    tft.drawFastHLine(xPos+6,yPos+5,4,darkGrey);
    tft.drawFastHLine(xPos+7,yPos+6,2,darkGrey);
    tft.drawFastVLine(xPos,yPos+13,3,darkGrey);
    tft.drawFastVLine(xPos+15,yPos+13,3,darkGrey);
    //orangeange
    tft.drawFastVLine(xPos,yPos,3,orange);
    tft.drawPixel(xPos+1,yPos,orange);
    tft.drawFastVLine(xPos+15,yPos,3,orange);
    tft.drawPixel(xPos+14,yPos,orange);
    tft.drawFastHLine(xPos,yPos+5,4,orange);
    tft.drawFastHLine(xPos+12,yPos+5,4,orange);
    tft.drawFastHLine(xPos+4,yPos+6,2,orange);
    tft.drawFastHLine(xPos+10,yPos+6,2,orange);
    tft.drawPixel(xPos+6,yPos+7,orange);
    tft.drawPixel(xPos+9,yPos+7,orange);
    tft.drawFastHLine(xPos+7,yPos+8,2,orange);
    tft.drawFastHLine(xPos,yPos+12,16,orange);
    tft.drawFastHLine(xPos+6,yPos+13,4,orange);
    //Yellow
    tft.drawFastHLine(xPos,yPos+6,4,yellow);
    tft.drawFastHLine(xPos+12,yPos+6,4,yellow);
    tft.drawFastHLine(xPos+3,yPos+7,3,yellow);
    tft.drawFastHLine(xPos+10,yPos+7,3,yellow);
    tft.drawFastHLine(xPos+5,yPos+8,2,yellow);
    tft.drawFastHLine(xPos+9,yPos+8,2,yellow);
    tft.drawFastHLine(xPos+6,yPos+9,4,yellow);
    tft.drawPixel(xPos,yPos+10,yellow);
    tft.drawPixel(xPos+15,yPos+10,yellow);
    tft.drawFastHLine(xPos,yPos+11,16,yellow);
    tft.drawFastHLine(xPos+7,yPos+12,2,yellow);
    //Red
    tft.drawFastHLine(xPos,yPos+7,3,red);
    tft.drawFastHLine(xPos+13,yPos+7,3,red);
    tft.drawFastHLine(xPos+2,yPos+8,3,red);
    tft.drawFastHLine(xPos+11,yPos+8,3,red);
    tft.drawFastHLine(xPos+4,yPos+9,2,red);
    tft.drawFastHLine(xPos+10,yPos+9,2,red);
    tft.drawPixel(xPos,yPos+9,red);
    tft.drawPixel(xPos+15,yPos+9,red);
    tft.drawFastHLine(xPos+1,yPos+10,14,red);
    tft.drawFastHLine(xPos+7,yPos+11,2,red);
    //Pink
    tft.drawFastHLine(xPos,yPos+8,2,hotPink);
    tft.drawFastHLine(xPos+14,yPos+8,2,hotPink);
    tft.drawFastHLine(xPos+1,yPos+9,3,hotPink);
    tft.drawFastHLine(xPos+12,yPos+9,3,hotPink);
  }
  else if(tileType == 68){
    drawTile(xPos,yPos,2);
    //Light Grey
    tft.drawPixel(xPos+15,yPos,lightGrey);
    tft.drawPixel(xPos+14,yPos+2,lightGrey);
    tft.drawFastHLine(xPos+8,yPos+3,6,lightGrey);
    tft.drawPixel(xPos+6,yPos+2,lightGrey);
    tft.drawFastVLine(xPos+5,yPos+2,3,lightGrey);
    tft.drawPixel(xPos+4,yPos+9,lightGrey);
    tft.drawFastHLine(xPos+4,yPos+10,2,lightGrey);
    tft.drawFastHLine(xPos+6,yPos+11,2,lightGrey);
    tft.drawPixel(xPos+8,yPos+12,lightGrey);
    tft.drawPixel(xPos+3,yPos+11,lightGrey);
    tft.drawPixel(xPos+1,yPos+12,lightGrey);
    //Dark Grey
    tft.drawFastHLine(xPos+13,yPos+1,3,darkGrey);
    tft.drawFastHLine(xPos+7,yPos+2,7,darkGrey);
    tft.drawFastVLine(xPos+15,yPos+2,2,darkGrey);
    tft.drawFastVLine(xPos+14,yPos+3,3,darkGrey);
    tft.drawFastVLine(xPos+13,yPos+4,3,darkGrey);
    tft.drawFastVLine(xPos+12,yPos+4,4,darkGrey);
    tft.drawFastVLine(xPos+11,yPos+4,1,darkGrey);
    tft.drawFastVLine(xPos+10,yPos+4,3,darkGrey);
    tft.drawFastVLine(xPos+9,yPos+4,4,darkGrey);
    tft.drawFastVLine(xPos+8,yPos+4,1,darkGrey);
    tft.drawFastVLine(xPos+7,yPos+3,4,darkGrey);
    tft.drawFastVLine(xPos+6,yPos+3,5,darkGrey);
    tft.drawFastVLine(xPos+5,yPos+5,1,darkGrey);
    tft.drawFastVLine(xPos+9,yPos+14,2,darkGrey);
    tft.drawFastVLine(xPos+8,yPos+13,3,darkGrey);
    tft.drawFastVLine(xPos+7,yPos+12,3,darkGrey);
    tft.drawFastVLine(xPos+6,yPos+12,2,darkGrey);
    tft.drawFastVLine(xPos+5,yPos+11,5,darkGrey);
    tft.drawFastVLine(xPos+4,yPos+11,4,darkGrey);
    tft.drawFastVLine(xPos+3,yPos+12,2,darkGrey);
    tft.drawFastVLine(xPos+2,yPos+12,4,darkGrey);
    tft.drawFastVLine(xPos+1,yPos+13,2,darkGrey);
    tft.drawFastVLine(xPos,yPos+13,1,darkGrey);
    //orangeange
    tft.drawFastHLine(xPos+12,yPos,3,orange);
    tft.drawFastHLine(xPos+4,yPos+1,9,orange);
    tft.drawFastVLine(xPos+4,yPos+1,3,orange);
    tft.drawFastVLine(xPos,yPos+3,2,orange);
    tft.drawFastHLine(xPos+2,yPos+10,2,orange);
    tft.drawFastHLine(xPos,yPos+11,3,orange);
    tft.drawPixel(xPos,yPos+12,orange);
    //Yellow
    tft.drawFastHLine(xPos+4,yPos,8,yellow);
    tft.drawFastVLine(xPos+3,yPos+1,3,yellow);
    tft.drawPixel(xPos+4,yPos+4,yellow);
    tft.drawFastVLine(xPos,yPos,3,yellow);
    tft.drawFastVLine(xPos+1,yPos+3,3,yellow);
    tft.drawPixel(xPos,yPos+5,yellow);
    tft.drawFastHLine(xPos+2,yPos+9,2,yellow);
    tft.drawFastHLine(xPos,yPos+10,2,yellow);
    //Red
    tft.drawPixel(xPos+3,yPos,red);
    tft.drawFastVLine(xPos+1,yPos,3,red);
    tft.drawFastVLine(xPos+2,yPos+3,3,red);
    tft.drawFastVLine(xPos+4,yPos+5,2,red);
    tft.drawFastVLine(xPos+3,yPos+6,3,red);
    tft.drawFastVLine(xPos,yPos+6,2,red);
    tft.drawPixel(xPos+1,yPos+6,red);
    tft.drawFastHLine(xPos+1,yPos+8,2,red);
    tft.drawFastHLine(xPos,yPos+9,2,red);
    tft.drawFastVLine(xPos+6,yPos+14,2,red);
    tft.drawFastVLine(xPos+3,yPos+14,2,red);
    tft.drawFastVLine(xPos,yPos+14,2,red);
    tft.drawPixel(xPos+7,yPos+15,red);
    tft.drawPixel(xPos+4,yPos+15,red);
    tft.drawPixel(xPos+1,yPos+15,red);
    //Pink
    tft.drawFastVLine(xPos+2,yPos,3,hotPink);
    tft.drawFastVLine(xPos+3,yPos+4,2,hotPink);
    tft.drawFastVLine(xPos+2,yPos+6,2,hotPink);
    tft.drawPixel(xPos+1,yPos+7,hotPink);
    tft.drawPixel(xPos,yPos+8,hotPink);
  }
}

void drawEnemy(int i){
  int x = Characters[i].posX;
  int y = Characters[i].posY;
  int enemyColour;
  int helmetColour = 0xC300;
  if(Characters[i].type == 1 || Characters[i].type == 2){
    enemyColour = 0x3C00;
  }
  else if(Characters[i].type == 5){
    enemyColour = 0x6011;
  }
  else{
    enemyColour = 0xA020;
  }

  //Enemy Facing Left
  if(Characters[i].cDirection == 0){
    //Enemy Horns
    tft.drawFastVLine(x*spacing+2,y*spacing,3,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+3,y*spacing+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+4,y*spacing+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+13,y*spacing+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+14,y*spacing+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+15,y*spacing,3,ILI9340_WHITE);
    //Enemy Head
    tft.fillRect(x*spacing+6,y*spacing,6,7,enemyColour);
    tft.drawFastVLine(x*spacing+12,y*spacing+1,6,enemyColour);
    tft.drawFastVLine(x*spacing+13,y*spacing+2,4,enemyColour);
    tft.drawFastVLine(x*spacing+5,y*spacing+1,3,enemyColour);
    tft.drawFastVLine(x*spacing+4,y*spacing+2,2,enemyColour);
    tft.drawFastHLine(x*spacing+6,y*spacing+7,5,enemyColour);
    tft.drawPixel(x*spacing+5,y*spacing+6,enemyColour);
    //Enemy Body
    tft.fillRect(x*spacing+1,y*spacing+8,13,2,enemyColour);
    tft.fillRect(x*spacing+13,y*spacing+9,2,3,enemyColour);
    tft.fillRect(x*spacing+6,y*spacing+12,6,2,enemyColour);
    tft.fillRect(x*spacing+5,y*spacing+13,2,2,enemyColour);
    tft.fillRect(x*spacing+4,y*spacing+14,2,2,enemyColour);
    tft.fillRect(x*spacing+11,y*spacing+13,2,2,enemyColour);
    tft.fillRect(x*spacing+12,y*spacing+14,2,2,enemyColour);
    //Draw Eyes
    tft.fillRect(x*spacing+4,y*spacing+4,2,2,0xDDE7);
    tft.fillRect(x*spacing+8,y*spacing+4,2,2,0xDDE7);
    //Draw Belt
    tft.fillRect(x*spacing+6,y*spacing+10,6,2,0xCC61);
    tft.fillRect(x*spacing+7,y*spacing+10,2,2,0xCE58);
    //Draw Sword
    tft.fillRect(x*spacing+1,y*spacing+10,2,2,0x82C0);
    tft.drawFastVLine(x*spacing+1,y*spacing+4,4,0xB555);
    tft.drawFastVLine(x*spacing+2,y*spacing+5,3,0xB555);
    if(Characters[i].type == 2 || Characters[i].type == 4){
      //DrawSword
      tft.drawFastVLine(x*spacing,y*spacing,5,0xB555);
      //Draw Shield
      tft.fillRect(x*spacing+12,y*spacing+7,3,6,0xB555);
      tft.drawFastVLine(x*spacing+13,y*spacing+8,4,0x6B6D);
      //Draw Helmet
      tft.fillRect(x*spacing+6,y*spacing,5,8,helmetColour);
      tft.drawFastVLine(x*spacing+4,y*spacing+2,4,helmetColour);
      tft.drawFastVLine(x*spacing+5,y*spacing+1,6,helmetColour);
      tft.drawFastVLine(x*spacing+11,y*spacing,7,helmetColour);
      tft.drawFastVLine(x*spacing+12,y*spacing+1,6,helmetColour);
      tft.drawFastVLine(x*spacing+13,y*spacing+3,4,helmetColour);
      tft.fillRect(x*spacing+5,y*spacing+4,6,2,0x0000);
      tft.fillRect(x*spacing+7,y*spacing+6,2,2,0x0000);
    }
  }
  //Enemy Facing Up
  else if(Characters[i].cDirection == 1){
    //Enemy Horns
    tft.drawFastVLine(x*spacing+1,y*spacing,3,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+2,y*spacing+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+3,y*spacing+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+12,y*spacing+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+13,y*spacing+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+14,y*spacing,3,ILI9340_WHITE);
    //Enemy Head
    tft.fillRect(x*spacing+5,y*spacing,6,7,enemyColour);
    tft.drawFastVLine(x*spacing+4,y*spacing+1,6,enemyColour);
    tft.drawFastVLine(x*spacing+3,y*spacing+2,4,enemyColour);
    tft.drawFastVLine(x*spacing+11,y*spacing+1,6,enemyColour);
    tft.drawFastVLine(x*spacing+12,y*spacing+2,4,enemyColour);
    tft.drawFastHLine(x*spacing+6,y*spacing+7,4,enemyColour);
    //Draw Body
    tft.fillRect(x*spacing+3,y*spacing+8,10,2,enemyColour);
    tft.fillRect(x*spacing+1,y*spacing+9,2,2,enemyColour);
    tft.fillRect(x*spacing+12,y*spacing+9,2,2,enemyColour);
    tft.drawPixel(x*spacing+13,y*spacing+11,enemyColour);
    tft.fillRect(x*spacing+5,y*spacing+12,6,2,enemyColour);
    tft.fillRect(x*spacing+4,y*spacing+13,2,2,enemyColour);
    tft.fillRect(x*spacing+3,y*spacing+14,2,2,enemyColour);
    tft.fillRect(x*spacing+10,y*spacing+13,2,2,enemyColour);
    tft.fillRect(x*spacing+11,y*spacing+14,2,2,enemyColour);
    //Draw Belt
    tft.fillRect(x*spacing+5,y*spacing+10,6,2,0xCC61);
    //Draw sword
    tft.fillRect(x*spacing+1,y*spacing+11,2,2,0x82C0);
    tft.drawFastVLine(x*spacing+1,y*spacing+5,4,0xB555);
    tft.drawFastVLine(x*spacing+2,y*spacing+6,3,0xB555);
    if(Characters[i].type == 2 || Characters[i].type == 4){
      //DrawSword
      tft.drawFastVLine(x*spacing+1,y*spacing+3,2,0xB555);
      tft.drawFastVLine(x*spacing+2,y*spacing+4,2,0xB555);
      //Draw Shield
      tft.drawFastHLine(x*spacing+12,y*spacing+7,3,0xB555);
      tft.drawFastHLine(x*spacing+13,y*spacing+8,2,0xB555);
      tft.drawFastVLine(x*spacing+14,y*spacing+9,3,0xB555);
      tft.drawFastHLine(x*spacing+12,y*spacing+12,3,0xB555);
      tft.drawPixel(x*spacing+12,y*spacing+11,0xB555);
      //Draw Helmet
      tft.fillRect(x*spacing+6,y*spacing,4,8,helmetColour);
      tft.drawFastVLine(x*spacing+3,y*spacing+2,4,helmetColour);
      tft.drawFastVLine(x*spacing+4,y*spacing+1,6,helmetColour);
      tft.drawFastVLine(x*spacing+5,y*spacing,7,helmetColour);
      tft.drawFastVLine(x*spacing+10,y*spacing,7,helmetColour);
      tft.drawFastVLine(x*spacing+11,y*spacing+1,6,helmetColour);
      tft.drawFastVLine(x*spacing+12,y*spacing+2,4,helmetColour);
    }
  }
  //Enemy Facing Right
  else if(Characters[i].cDirection == 2){
    //Enemy Horns
    tft.drawFastVLine(x*spacing,y*spacing,3,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+1,y*spacing+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+2,y*spacing+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+11,y*spacing+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+12,y*spacing+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+13,y*spacing,3,ILI9340_WHITE);
    //Enemy Head
    tft.fillRect(x*spacing+4,y*spacing,6,7,enemyColour);
    tft.drawFastVLine(x*spacing+3,y*spacing+1,6,enemyColour);
    tft.drawFastVLine(x*spacing+2,y*spacing+2,4,enemyColour);
    tft.drawFastVLine(x*spacing+10,y*spacing+1,3,enemyColour);
    tft.drawFastVLine(x*spacing+11,y*spacing+2,2,enemyColour);
    tft.drawFastHLine(x*spacing+5,y*spacing+7,5,enemyColour);
    tft.drawPixel(x*spacing+10,y*spacing+6,enemyColour);
    //Draw Body
    tft.fillRect(x*spacing+2,y*spacing+8,13,2,enemyColour);
    tft.fillRect(x*spacing+1,y*spacing+9,2,3,enemyColour);
    tft.fillRect(x*spacing+4,y*spacing+12,6,2,enemyColour);
    tft.fillRect(x*spacing+3,y*spacing+13,2,2,enemyColour);
    tft.fillRect(x*spacing+2,y*spacing+14,2,2,enemyColour);
    tft.fillRect(x*spacing+9,y*spacing+13,2,2,enemyColour);
    tft.fillRect(x*spacing+10,y*spacing+14,2,2,enemyColour);
    //Draw Eyes
    tft.fillRect(x*spacing+6,y*spacing+4,2,2,0xDDE7);
    tft.fillRect(x*spacing+10,y*spacing+4,2,2,0xDDE7);
    //Draw Belt
    tft.fillRect(x*spacing+4,y*spacing+10,6,2,0xCC61);
    tft.fillRect(x*spacing+7,y*spacing+10,2,2,0xCE58);
    //Draw sword
    tft.fillRect(x*spacing+13,y*spacing+10,2,2,0x82C0);
    tft.drawFastVLine(x*spacing+13,y*spacing+5,3,0xB555);
    tft.drawFastVLine(x*spacing+14,y*spacing+4,4,0xB555);
    if(Characters[i].type == 2 || Characters[i].type == 4){
      //DrawSword
      tft.drawFastVLine(x*spacing+15,y*spacing,6,0xB555);
      //Draw Shield
      tft.fillRect(x*spacing+1,y*spacing+7,3,6,0xB555);
      tft.drawFastVLine(x*spacing+2,y*spacing+8,4,0x6B6D);
      //Draw Helmet
      tft.fillRect(x*spacing+5,y*spacing,5,8,helmetColour);
      tft.drawFastVLine(x*spacing+2,y*spacing+2,4,helmetColour);
      tft.drawFastVLine(x*spacing+3,y*spacing+1,6,helmetColour);
      tft.drawFastVLine(x*spacing+4,y*spacing,7,helmetColour);
      tft.drawFastVLine(x*spacing+10,y*spacing+1,6,helmetColour);
      tft.drawFastVLine(x*spacing+11,y*spacing+2,4,helmetColour);
      tft.fillRect(x*spacing+5,y*spacing+4,6,2,0x0000);
      tft.fillRect(x*spacing+7,y*spacing+6,2,2,0x0000);
    }
  }
  //Enemy Facing Down
  else{
      //Enemy Horns
    tft.drawFastVLine(x*spacing+1,y*spacing,3,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+2,y*spacing+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+3,y*spacing+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+12,y*spacing+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+13,y*spacing+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*spacing+14,y*spacing,3,ILI9340_WHITE);
    //Enemy Head
    tft.fillRect(x*spacing+5,y*spacing,6,7,enemyColour);
    tft.drawFastVLine(x*spacing+4,y*spacing+1,6,enemyColour);
    tft.drawFastVLine(x*spacing+3,y*spacing+2,4,enemyColour);
    tft.drawFastVLine(x*spacing+11,y*spacing+1,6,enemyColour);
    tft.drawFastVLine(x*spacing+12,y*spacing+2,4,enemyColour);
    tft.drawFastHLine(x*spacing+6,y*spacing+7,4,enemyColour);
    //Draw Eyes
    tft.fillRect(x*spacing+5,y*spacing+4,2,2,0xDDE7);
    tft.fillRect(x*spacing+9,y*spacing+4,2,2,0xDDE7);
    //Draw Body
    tft.fillRect(x*spacing+3,y*spacing+8,10,2,enemyColour);
    tft.fillRect(x*spacing+1,y*spacing+9,2,2,enemyColour);
    tft.fillRect(x*spacing+12,y*spacing+9,2,2,enemyColour);
    tft.drawPixel(x*spacing+13,y*spacing+11,enemyColour);
    tft.fillRect(x*spacing+5,y*spacing+12,6,2,enemyColour);
    tft.fillRect(x*spacing+4,y*spacing+13,2,2,enemyColour);
    tft.fillRect(x*spacing+3,y*spacing+14,2,2,enemyColour);
    tft.fillRect(x*spacing+10,y*spacing+13,2,2,enemyColour);
    tft.fillRect(x*spacing+11,y*spacing+14,2,2,enemyColour);
    //Draw Belt
    tft.fillRect(x*spacing+5,y*spacing+10,6,2,0xCC61);
    tft.fillRect(x*spacing+7,y*spacing+10,2,2,0xCE58);
    //Draw sword
    tft.fillRect(x*spacing+13,y*spacing+11,2,2,0x82C0);
    tft.drawFastVLine(x*spacing+13,y*spacing+6,3,0xB555);
    tft.drawFastVLine(x*spacing+14,y*spacing+5,4,0xB555);
    if(Characters[i].type == 2 || Characters[i].type == 4){
      //DrawSword
      tft.drawFastVLine(x*spacing+13,y*spacing+1,5,0xB555);
      tft.drawFastVLine(x*spacing+14,y*spacing+1,4,0xB555);
      //Draw Shield
      tft.fillRect(x*spacing+1,y*spacing+7,3,6,0xB555);
      tft.drawFastVLine(x*spacing+2,y*spacing+8,4,0x6B6D);
      //Draw Helmet
      tft.fillRect(x*spacing+6,y*spacing,4,8,helmetColour);
      tft.drawFastVLine(x*spacing+3,y*spacing+2,4,helmetColour);
      tft.drawFastVLine(x*spacing+4,y*spacing+1,6,helmetColour);
      tft.drawFastVLine(x*spacing+5,y*spacing,7,helmetColour);
      tft.drawFastVLine(x*spacing+10,y*spacing,7,helmetColour);
      tft.drawFastVLine(x*spacing+11,y*spacing+1,6,helmetColour);
      tft.drawFastVLine(x*spacing+12,y*spacing+2,4,helmetColour);
      tft.fillRect(x*spacing+5,y*spacing+4,6,2,0x0000);
      tft.fillRect(x*spacing+7,y*spacing+6,2,2,0x0000);
    }
  }
}

void drawHero(int x,int y) {
  drawTile(x*spacing,y*spacing,Characters[0].cTile);
  //Hero facing left
  if(Characters[0].cDirection == 0) {
    //Draw Hat
    tft.drawFastHLine(x*spacing+5,y*spacing,11,0x1CB0);
    tft.drawFastHLine(x*spacing+4,y*spacing+1,11,0x1CB0);
    tft.drawFastHLine(x*spacing+3,y*spacing+2,11,0x1CB0);
    tft.drawFastHLine(x*spacing+10,y*spacing+3,4,0x1CB0);
    tft.drawFastHLine(x*spacing+11,y*spacing+4,3,0x1CB0);
    tft.drawFastHLine(x*spacing+12,y*spacing+5,2,0x1CB0);
    //Draw Eyes
    tft.fillRect(x*spacing+4,y*spacing+4,2,2,0x0000);
    tft.fillRect(x*spacing+8,y*spacing+4,2,2,0x0000);
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
    //Draw Boots
    tft.drawFastHLine(x*spacing+4,y*spacing+14,2,0xA3C0);
    tft.drawFastHLine(x*spacing+3,y*spacing+15,3,0xA3C0);
    tft.drawFastHLine(x*spacing+12,y*spacing+14,2,0xA3C0);
    tft.drawFastHLine(x*spacing+11,y*spacing+15,3,0xA3C0);
    if(weapon == 0){
      //Draw Sword
      tft.drawFastHLine(x*spacing,y*spacing+7,2,0xA000);
      tft.fillRect(x*spacing,y*spacing+10,2,2,0xA000);
      tft.fillRect(x*spacing,y*spacing+2,2,7,0xDEDB);
    }
    else if (weapon == 1) {
      //Draw Spear
      tft.drawFastVLine(x*spacing+1,y*spacing,2,0x9DB7);
      tft.drawFastVLine(x*spacing+2,y*spacing+1,5,0x9DB7);
      tft.drawFastVLine(x*spacing+1,y*spacing+5,2,0x9DB7);
      tft.drawFastVLine(x*spacing,y*spacing+3,5,0x9DB7);
      tft.drawFastVLine(x*spacing,y*spacing+10,4,0x9DB7);
    }
    else if(weapon == 2){
      //Draw Bow
      tft.fillRect(x*spacing,y*spacing+6,2,2,0xC440);
      tft.fillRect(x*spacing,y*spacing+10,2,2,0xC440);
      tft.drawFastHLine(x*spacing,y*spacing+5,2,0xA513);
      tft.drawFastHLine(x*spacing+1,y*spacing+4,2,0xA513);
      tft.drawFastHLine(x*spacing+2,y*spacing+3,2,0xA513);
      tft.drawFastHLine(x*spacing,y*spacing+12,2,0xA513);
      tft.drawFastHLine(x*spacing+1,y*spacing+13,2,0xA513);
      tft.drawFastHLine(x*spacing+2,y*spacing+14,2,0xA513);
      tft.drawFastVLine(x*spacing+3,y*spacing+4,10,0xFFFF);
    }
  }
  //Hero facing up
  else if(Characters[0].cDirection == 1) {
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
    //Draw Boots
    tft.fillRect(x*spacing+4,y*spacing+14,2,2,0xA3C0);
    tft.fillRect(x*spacing+10,y*spacing+14,2,2,0xA3C0);
    if(weapon == 0){
      //Draw Sword
      tft.drawFastHLine(x*spacing+1,y*spacing+8,2,0xA000);
      tft.fillRect(x*spacing+1,y*spacing+11,2,2,0xA000);
      tft.fillRect(x*spacing+1,y*spacing+1,2,7,0xDEDB);
    }
    else if (weapon == 1) {
      //Draw Spear
      tft.drawFastVLine(x*spacing+1,y*spacing+1,2,0x9DB7);
      tft.drawPixel(x*spacing+3,y*spacing+1,0x9DB7);
      tft.drawFastVLine(x*spacing,y*spacing+2,5,0x9DB7);
      tft.drawFastVLine(x*spacing+1,y*spacing+6,2,0x9DB7);
      tft.drawFastVLine(x*spacing+2,y*spacing+4,5,0x9DB7);
      tft.drawFastVLine(x*spacing+3,y*spacing+6,2,0x9DB7);
      tft.drawFastVLine(x*spacing+2,y*spacing+11,4,0x9DB7);
    }
    else if(weapon == 2) {
      //Draw Bow
      tft.drawFastVLine(x*spacing+1,y*spacing+7,2,0xC440);
      tft.drawFastVLine(x*spacing+1,y*spacing+11,2,0xC440);
      tft.drawFastHLine(x*spacing+1,y*spacing+4,2,0xA513);
      tft.drawFastVLine(x*spacing+1,y*spacing+5,2,0xA513);
      tft.drawFastVLine(x*spacing+1,y*spacing+13,2,0xA513);
      tft.drawFastHLine(x*spacing+1,y*spacing+15,2,0xA513);
      tft.drawFastVLine(x*spacing+2,y*spacing+5,4,0xFFFF);
      tft.drawFastVLine(x*spacing+2,y*spacing+11,4,0xFFFF);
    }
  }
  //Hero facing right
  else if(Characters[0].cDirection == 2) {
    //Draw Hat
    tft.drawFastHLine(x*spacing,y*spacing,11,0x1CB0);
    tft.drawFastHLine(x*spacing+1,y*spacing+1,11,0x1CB0);
    tft.drawFastHLine(x*spacing+2,y*spacing+2,11,0x1CB0);
    tft.drawFastHLine(x*spacing+2,y*spacing+3,4,0x1CB0);
    tft.drawFastHLine(x*spacing+2,y*spacing+4,3,0x1CB0);
    tft.drawFastHLine(x*spacing+2,y*spacing+5,2,0x1CB0);
    //Draw eyes
    tft.fillRect(x*spacing+6,y*spacing+4,2,2,0x0000);
    tft.fillRect(x*spacing+10,y*spacing+4,2,2,0x0000);
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
    //Draw Boots
    tft.drawFastHLine(x*spacing+2,y*spacing+14,2,0xA3C0);
    tft.drawFastHLine(x*spacing+2,y*spacing+15,3,0xA3C0);
    tft.drawFastHLine(x*spacing+10,y*spacing+14,2,0xA3C0);
    tft.drawFastHLine(x*spacing+10,y*spacing+15,3,0xA3C0); 
    if(weapon == 0){
      //Draw Sword
      tft.drawFastHLine(x*spacing+14,y*spacing+7,2,0xA000);
      tft.fillRect(x*spacing+14,y*spacing+10,2,2,0xA000);
      tft.fillRect(x*spacing+14,y*spacing+2,2,7,0xDEDB);
    }
    else if (weapon == 1) {
      //Draw Spear
      tft.drawFastVLine(x*spacing+14,y*spacing,2,0x9DB7);
      tft.drawFastVLine(x*spacing+13,y*spacing+1,5,0x9DB7);
      tft.drawFastVLine(x*spacing+14,y*spacing+5,2,0x9DB7);
      tft.drawFastVLine(x*spacing+15,y*spacing+3,5,0x9DB7);
      tft.drawFastVLine(x*spacing+15,y*spacing+10,4,0x9DB7);
    }
    else if(weapon == 2){
      //Draw Bow
      tft.fillRect(x*spacing+14,y*spacing+6,2,2,0xC440);
      tft.fillRect(x*spacing+14,y*spacing+10,2,2,0xC440);
      tft.drawFastHLine(x*spacing+14,y*spacing+5,2,0xA513);
      tft.drawFastHLine(x*spacing+13,y*spacing+4,2,0xA513);
      tft.drawFastHLine(x*spacing+12,y*spacing+3,2,0xA513);
      tft.drawFastHLine(x*spacing+14,y*spacing+12,2,0xA513);
      tft.drawFastHLine(x*spacing+13,y*spacing+13,2,0xA513);
      tft.drawFastHLine(x*spacing+12,y*spacing+14,2,0xA513);
      tft.drawFastVLine(x*spacing+12,y*spacing+4,10,0xFFFF);
    }
  }
  //Hero facing down
  else {
    //Draw Hat
    tft.drawFastHLine(x*spacing+5,y*spacing,6,0x1CB0);
    tft.drawFastHLine(x*spacing+4,y*spacing+1,8,0x1CB0);
    tft.drawFastHLine(x*spacing+5,y*spacing+2,6,0x1CB0);
    tft.fillRect(x*spacing+3,y*spacing+2,2,2,0x1CB0);
    tft.fillRect(x*spacing+11,y*spacing+2,2,2,0x1CB0);
    tft.drawFastVLine(x*spacing+3,y*spacing+4,2,0x1CB0);
    tft.drawFastVLine(x*spacing+12,y*spacing+4,2,0x1CB0);
    //Draw eyes
    tft.fillRect(x*spacing+5,y*spacing+4,2,2,0x0000);
    tft.fillRect(x*spacing+9,y*spacing+4,2,2,0x0000);
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
    //Draw Boots
    tft.fillRect(x*spacing+4,y*spacing+14,2,2,0xA3C0);
    tft.fillRect(x*spacing+10,y*spacing+14,2,2,0xA3C0);
    if(weapon == 0) {
      //Draw Sword
      tft.drawFastHLine(x*spacing+13,y*spacing+8,2,0xA000);
      tft.fillRect(x*spacing+13,y*spacing+11,2,2,0xA000);
      tft.fillRect(x*spacing+13,y*spacing+1,2,7,0xDEDB);
    }
    else if (weapon == 1) {
      //Draw Spear
      tft.drawFastVLine(x*spacing+12,y*spacing+1,2,0x9DB7);
      tft.drawFastVLine(x*spacing+14,y*spacing+1,2,0x9DB7);
      tft.drawFastVLine(x*spacing+11,y*spacing+2,5,0x9DB7);
      tft.drawFastVLine(x*spacing+15,y*spacing+2,5,0x9DB7);
      tft.drawFastVLine(x*spacing+12,y*spacing+6,2,0x9DB7);
      tft.drawFastVLine(x*spacing+14,y*spacing+6,2,0x9DB7);
      tft.drawFastVLine(x*spacing+13,y*spacing+4,5,0x9DB7);
      tft.drawFastVLine(x*spacing+13,y*spacing+11,4,0x9DB7);
    }
    else if(weapon == 2) {
      //Draw Bow
      tft.fillRect(x*spacing+13,y*spacing+7,2,2,0xC440);
      tft.fillRect(x*spacing+13,y*spacing+11,2,2,0xC440);
      tft.fillRect(x*spacing+13,y*spacing+4,2,2,0xA513);
      tft.fillRect(x*spacing+13,y*spacing+13,2,2,0xA513);
    }
  }
}
