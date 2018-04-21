#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"
//#include <SPI.h>
//#include <SD.h>
//#include <Adafruit_VS1053.h>

#define _sclk 52
#define _miso 50
#define _mosi 51
#define _cs 41
#define _dc 40
#define _rst 45

//Sound code
//#define BREAKOUT_CS 10
//#define BREAKOUT_DCS 8
//#define SHIELD_CS 7
//#define SHIELD_DCS 6
//#define CARDCS 4
//#define DREQ 3

//Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_CS,SHIELD_DCS,DREQ,CARDCS);//set up for the shield

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
  int lastMove;
  void properties(int _type, int _posX, int _posY, int _cDirection, int _row, int _col, int _health, bool _cStatus, int _cTile, int _lastMove) {
    type = _type;
    posX = _posX;
    posY = _posY;
    cDirection = _cDirection;
    row = _row;
    col = _col;
    health = _health;
    cStatus = _cStatus;
    cTile = _cTile;
    lastMove = _lastMove;
  }
};

class ROOM
{
  public:
  int enemies;
  int switches;
  void properties(int _enemies, int _switches) {
    enemies = _enemies;
    switches = _switches;
  }
};

int wMap[3][3][15][15] = {{{{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}, //L Room 3
                            {10,14,14,14,14,14,14,14,14,14,14,14,14,14,10},
                            {10,14,14,14,14,1,1,1,1,1,1,14,14,14,10},
                            {10,14,14,1,1,1,1,1,1,1,1,1,14,14,10},
                            {10,14,1,1,1,1,1,1,1,1,1,1,14,14,10},
                            {10,14,1,1,1,1,1,1,1,1,1,1,14,14,10},
                            {10,14,1,1,1,1,1,1,1,1,1,1,1,14,10},
                            {10,14,1,1,1,1,1,1,1,1,1,1,1,1,1},
                            {10,14,1,1,1,1,1,1,1,1,1,1,1,14,10},
                            {10,14,1,1,1,1,1,1,1,1,1,1,14,14,10},
                            {10,14,1,1,1,1,1,1,1,1,1,1,14,14,10},
                            {10,14,14,1,1,1,1,1,1,1,1,1,14,14,10},
                            {10,14,14,14,14,1,1,1,1,1,1,14,14,14,10},
                            {10,14,14,14,14,14,14,14,14,14,14,14,14,14,10},
                            {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}},
                         
                           {{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}, //T Room 3
                            {10,16,16,16,16,16,16,16,16,16,16,16,16,16,10},
                            {10,16,2,2,2,2,2,2,2,2,2,2,2,16,10},
                            {10,16,2,2,2,2,2,2,2,2,2,2,2,16,10},
                            {10,16,2,2,2,16,16,2,2,16,16,2,2,16,10},
                            {10,16,2,2,2,16,16,2,2,16,16,2,2,16,10},
                            {60,63,66,2,2,2,2,2,2,2,2,2,2,2,10},
                            {61,64,67,2,2,2,2,2,2,2,2,2,2,2,10},
                            {62,65,68,2,2,2,2,2,2,2,2,2,2,2,10},
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
                            {10,14,14,14,14,14,10,0,0,10,1,14,14,0,10},
                            {1,1,14,14,14,14,0,0,0,10,7,14,14,0,0},
                            {10,14,14,14,14,14,10,0,0,10,1,14,14,0,10},
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
                            {2,2,1,1,15,15,15,15,15,15,15,15,1,0,10},
                            {2,2,1,1,15,15,15,15,15,15,15,15,1,0,0},
                            {2,2,1,1,15,15,15,15,15,15,15,15,1,0,10},
                            {10,10,5,5,15,15,15,15,15,15,15,15,15,10,10},
                            {10,15,15,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,15,15,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,15,15,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,15,15,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}},

                           {{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}, //D Room 2
                            {10,0,0,12,12,0,0,7,0,0,12,12,0,0,10},
                            {10,0,0,0,12,12,0,0,0,12,12,0,0,0,10},
                            {10,0,0,0,0,12,12,0,12,12,0,0,0,0,10},
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
                            {10,0,0,0,0,10,14,14,14,10,0,0,0,0,10},
                            {10,0,0,0,0,0,14,14,14,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,10,0,0,0,0,0,0,10},
                            {10,0,0,0,10,0,0,0,0,0,10,0,0,0,10},
                            {10,14,0,0,0,0,0,0,0,0,0,0,0,14,10},
                            {10,14,14,0,0,0,0,0,0,0,0,0,14,14,10},
                            {10,14,14,14,0,0,0,0,0,0,0,14,14,14,10},
                            {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}},

                           {{10,10,10,10,10,10,10,0,10,10,10,10,10,10,10}, //T Room 1
                            {10,10,10,10,10,0,0,0,0,0,10,10,10,10,10},
                            {10,10,10,0,0,0,0,0,0,0,0,0,10,10,10},
                            {10,10,0,0,0,0,0,0,0,0,0,0,0,10,10},
                            {10,10,0,0,0,0,0,0,0,0,0,0,0,10,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,10,0,0,0,0,0,0,0,0,0,0,0,10,10},
                            {10,10,0,0,0,0,0,0,0,0,0,0,0,10,10},
                            {10,10,10,0,0,0,0,0,0,0,0,0,10,10,10},
                            {10,10,10,10,10,0,0,0,0,0,10,10,10,10,10},
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
const int SPACING = 16;
//Reads input directions from the Dpad
bool dPad[4];
//Initialize array of character objects
CHARACTER Characters[38];
// Initialize array of room objects
ROOM Rooms[3][3];

//Boolean statments to make sure button presses are only registered once
bool moveState;
bool lastMoveState = 1;
bool attackState;
bool lastAttackState = 1;
bool weaponState;
bool lastWeaponState = 1;

// Current room's row
int wRow = 2;
// Current room's column
int wCol = 1;
// Currently equipped weapon - 0 for sword, 1 for trident, 2 for bow
int weapon = 0;
// Booleans to set whether the hero has non-default weapons
bool spear = false;
bool bow = false;
// Maximum hero health - may be increased by heart containers
int maxHealth = 16;

// Total number of enemies
const int ENEMYCOUNT = 21;
// Total number of breakables
const int BREAKABLECOUNT = 11;

// Base boss health
int bossHP = 30;
// Boolean to initiate boss battle
bool bossRoom = false;
// Default ate at which tiles turn to lava during boss battle (1/bossLavaRate tiles will change each turn)
int bossLavaRate = 6;

void setup() {

  /*
  Serial.begin(9600);
  if(!musicPlayer.begin()) {
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while(1);
  }
  Serial.println(F("VS1053 found"));
  */
  
  randomSeed(analogRead(0));
  
  // Character properties: type,posX,posY,cDirection,row,col,health,cStatus,cTile
  // Character 0 is the Hero
  Characters[0].properties(0,7,14,1,2,1,24,1,10,0);
  // Characters of types 1 through 5 are enemies
  // Top left enemies
  Characters[1].properties(2,7,4,0,0,0,7,1,1,0);
  Characters[2].properties(1,4,8,0,0,0,4,1,1,0);
  Characters[3].properties(1,10,8,0,0,0,4,1,1,0);
  // Top right enemies
  Characters[4].properties(4,3,4,0,0,2,7,1,0,0);
  Characters[5].properties(3,11,3,0,0,2,4,1,0,0);
  Characters[6].properties(3,2,8,0,0,2,4,1,0,0);
  Characters[7].properties(4,12,8,0,0,2,7,1,0,0);
  // Middle left enemies
  Characters[8].properties(1,2,2,0,1,0,4,1,0,0);
  Characters[9].properties(2,12,2,0,1,0,7,1,0,0);
  // Middle right enemies
  Characters[10].properties(3,5,3,0,1,2,4,1,0,0);
  Characters[11].properties(4,12,4,0,1,2,7,1,0,0);
  Characters[12].properties(3,4,10,0,1,2,4,1,0,0);
  // Bottom left enemies
  Characters[13].properties(1,5,2,0,2,0,4,1,0,0);
  Characters[14].properties(1,9,2,0,2,0,4,1,0,0);
  Characters[15].properties(1,3,7,0,2,0,4,1,0,0);
  Characters[16].properties(1,7,10,0,2,0,4,1,0,0);
  // Bottom centre enemies
  Characters[17].properties(5,11,3,1,2,1,4,1,0,0);
  Characters[18].properties(5,3,8,3,2,1,4,1,0,0);
  // Bottom right enemies
  Characters[19].properties(3,5,3,0,2,2,4,1,0,0);
  Characters[20].properties(3,12,5,0,2,2,4,1,0,0);
  Characters[21].properties(3,9,12,0,2,2,4,1,0,0);
  
  // Characters of type 6 are pots
  // Bottom left pots
  Characters[22].properties(8,0,0,0,0,0,0,0,10,0);
  Characters[23].properties(6,1,6,0,2,0,1,1,0,0);
  Characters[24].properties(6,1,8,0,2,0,1,1,0,0);
  Characters[25].properties(6,6,13,0,2,0,1,1,0,0);
  Characters[26].properties(6,8,13,0,2,0,1,1,0,0);
  
  // Characters of type 7 are switches
  // Top left switches
  Characters[27].properties(7,1,3,0,0,0,1,1,14,0);
  Characters[28].properties(7,13,3,0,0,0,1,1,14,0);
  Characters[29].properties(7,3,13,0,0,0,1,1,14,0);
  Characters[30].properties(7,11,13,0,0,0,1,1,14,0);
  // Top right switch
  Characters[31].properties(7,7,6,0,0,2,1,1,0,0);
  // Middle left switch
  Characters[32].properties(7,7,8,0,1,0,1,1,0,0);
  
  // Hey!  Who put these here?
  Characters[33].properties(6,13,11,0,2,1,1,1,0,0);
  Characters[34].properties(6,13,12,0,2,1,1,1,0,0);
  Characters[35].properties(6,13,13,0,2,1,1,1,0,0);
  Characters[36].properties(6,12,13,0,2,1,1,1,0,0);
  Characters[37].properties(6,12,14,0,2,1,1,1,0,0);
  
  // Room properties: enemies, switches
  // Top Rooms
  Rooms[0][0].properties(3,4);
  Rooms[0][1].properties(1,0);
  Rooms[0][2].properties(4,1);
  // Middle Rooms
  Rooms[1][0].properties(2,1);
  Rooms[1][1].properties(0,0);
  Rooms[1][2].properties(3,0);
  // Bottom Rooms
  Rooms[2][0].properties(4,0);
  Rooms[2][1].properties(4,0);
  Rooms[2][2].properties(3,0);
  
  // Initialize pins for d-pad and buttons
  for(int i = 22;i<29;i++){
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
  
  // Initialize the screen
  tft.begin();
  
  // Set the screen to landscape and clear it
  tft.setRotation(3);
  tft.fillScreen(ILI9340_BLACK);
  
  // initializeMap();
  drawMap();
  hearts();
  placeBreakables();
  heroMove();
  enemiesMove();
  gameBorder();
}

void loop() {
  // Check current state of buttons
  moveState = digitalRead(26);
  attackState = digitalRead(27);
  weaponState = digitalRead(28);
  
  // Check if user wants to change direction
  if(Characters[0].health > 0)
    faceDirection();
    
  // If the weapon button is pressed, switch to next available weapon
  if ((weaponState && !lastWeaponState)&& Characters[0].health>0) {
    weapon++;
    if (weapon == 1 && !spear)
      weapon++;
    if (weapon == 2 && !bow)
      weapon = 0;
    if(weapon == 3)
      weapon = 0;
    drawHero(Characters[0].posX,Characters[0].posY);
  }
  lastWeaponState = weaponState;
  
  // If the move button is pressed, take a turn
  if((moveState && !lastMoveState) && Characters[0].health>0) {
    heroMove();
    enemiesMove();
  }
  lastMoveState = moveState;
  
  // If the attack button is pressed, take a turn
  if((attackState && !lastAttackState) && Characters[0].health>0){
    heroAttack();
    enemiesMove();
  }
  lastAttackState = attackState;
}

//Place the character after a map change
void placeChar() {
  //Remove Hero from current spot
  drawTile(Characters[0].posX*SPACING,Characters[0].posY*SPACING,Characters[0].cTile);
  wMap[wRow][wCol][Characters[0].posX][Characters[0].posY] = Characters[0].cTile;
  
  if(Characters[0].cDirection == 3 && wCol == 1 && wRow == 2) {
    Characters[0].posX = 7;
    Characters[0].posY = 13;
    Characters[0].cDirection = 1;
    tft.fillScreen(ILI9340_BLACK);
    tft.setCursor(60,60);
    tft.setTextSize(2);
    tft.setTextColor(0xFFFF);
    tft.println("Created By");
    tft.setTextColor(0x096F);
    tft.setTextSize(4);
    tft.setCursor(92,80);
    tft.println("Liam");
    tft.setTextColor(0xFFFF);
    tft.setTextSize(2);
    tft.setCursor(144,114);
    tft.println("&");
    tft.setTextColor(0xD100);
    tft.setTextSize(4);
    tft.setCursor(107,130);
    tft.println("Torin");
    delay(10000);
  }
  else {
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
  }
   drawMap();
   gameBorder();
   hearts();
   placeBreakables();
   
   //Place the Hero on the map
   wMap[wRow][wCol][Characters[0].posX][Characters[0].posY] = 42;
   //Draw the hero
   drawHero(Characters[0].posX,Characters[0].posY);

   //If all the switches have been press, build the brige to the boss
   if (wRow == 1 && wCol == 1 && Rooms[0][0].switches == 0 && Rooms[0][2].switches == 0) {
     for (int i = 4; i < 12; i++) {
      for (int j = 6; j < 9; j++) {
        wMap[wRow][wCol][j][i] = 1;
        drawTile(j*SPACING, i*SPACING, 1);
      }
    }
  }
}


void heroMove() {
  int newPos[2] = {};
  // Check the direction the hero is trying to move
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

  // Check if the Hero is moving to a valid square
  if(wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] < 10 || (wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] >= 50 && wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] <= 59)) {
    drawTile(Characters[0].posX*SPACING,Characters[0].posY*SPACING,Characters[0].cTile);
    wMap[wRow][wCol][Characters[0].posX][Characters[0].posY] = Characters[0].cTile;
    
    //If the hero is walking onto a heart, restore hero health
    if(wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] == 5) {
      Characters[0].health+=2;
      hearts();
    }

    // If the hero is moving onto an item
    else if (wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] == 7) {
      // If in room [1,0], the hero is picking up the trident
      if(wCol == 0 && wRow == 1)
        spear = true;
      // If in room [1,2], the hero is picking up the bow
      if(wCol == 2 && wRow == 1)
        bow = true;
      // If in room [0,0] or room [0,2], the hero is picking up a heart container
      if((wCol == 0 || wCol == 2) && wRow == 0) {
        maxHealth += 4; // Increase maximum hero health by 4
        Characters[0].health = maxHealth;
        hearts();
      }
    }

    // If the hero is moving onto acid
    else {
      Characters[0].cTile = wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]];
      if(wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] == 3) {
        Characters[0].health-=1;
        hearts();
      }
    }

    // Move the hero to the new position
    wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] = 42;
    Characters[0].posX = Characters[0].posX + newPos[0];
    Characters[0].posY = Characters[0].posY + newPos[1];
    drawHero(Characters[0].posX,Characters[0].posY);
    if((Characters[0].row == 0 && Characters[0].col == 1) && bossRoom == 1) {
      bossMechanics(0);
    }
  }

  // If the hero is moving into a door
  if(Characters[0].posX <= 0 || Characters[0].posX >= 14 || Characters[0].posY <= 0 || Characters[0].posY >= 14){
    placeChar();
  }

  // If the hero has moved far into the boss room (crossed boss activation threshold)
  if ((Characters[0].row == 0 && Characters[0].col == 1) && bossRoom == false && Characters[0].posY <= 10) {
    bossRoom = true;
  }
}

void heroAttack(){
  int attacks[2] = {};

  // Check the direction the hero is attacking in
  if(Characters[0].cDirection == 0)
    attacks[0] = -1;
  else if(Characters[0].cDirection == 1)
    attacks[1] = -1;
  else if(Characters[0].cDirection == 2)
    attacks[0] = 1;
  else
    attacks[1] = 1;

  // If the hero is using the sword
  if (weapon == 0) {
    // If the hero is attacking in the boss room, boss mechanics take and action
    if((Characters[0].row == 0 && Characters[0].col == 1) && bossRoom == 1) {
      bossMechanics(0);
    }
    // Check if the hero is attacking a damageable object
    if(wMap[wRow][wCol][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]] >= 100){
      int currentEnemy = wMap[wRow][wCol][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]]-100;
      // Decrease the damageable object's health by 3
      Characters[currentEnemy].health -= 3;
      // Check if the object has been killed/destroyed
      if(Characters[currentEnemy].health <= 0) {
        Characters[currentEnemy].cStatus = 0;
        // Check what type of object was killed/destroyed
        // If it was an enemy, decrement that room's 'enemies' property and check if an action should be taken
        if (Characters[currentEnemy].type < 6) {
          Rooms[Characters[currentEnemy].row][Characters[currentEnemy].col].enemies--;
          checkEnemies();
        }
        // If it was a switch, decrement that room's 'switches' property and check if an action should be taken
        if (Characters[currentEnemy].type == 7) {
          Rooms[Characters[currentEnemy].row][Characters[currentEnemy].col].switches--;
          checkSwitches();
        }
        // If the killed/destroyed object was an enemy or a pot, chance for a heart to drop
        if(random(8) == 0 && Characters[currentEnemy].type < 7){
          Characters[currentEnemy].cTile = 5;
        }
        // Remove object from the map
        wMap[wRow][wCol][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]] = Characters[currentEnemy].cTile;
        drawTile(Characters[currentEnemy].posX*SPACING,Characters[currentEnemy].posY*SPACING,Characters[currentEnemy].cTile);
      }
    }
    // Check if the boss is being attacked
    else if(wMap[wRow][wCol][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]] >= 60 && wMap[wRow][wCol][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]] <= 68) {
      // Decrease the boss's health by 3
      bossHP -= 3;
      // If the boss has been enraged, set bossLavaRate to 2 and call bossEnrage
      if (bossHP <= 12) {
        bossLavaRate = 2;
        bossEnrage();
      }
      // Check if the boss has been killed
      if(bossHP <= 0) {
        // Remove all lava from the room
        magmaClear();
        // Set hero health to 0 - this prevents the player from continuing to move after the game ends
        Characters[0].health = 0;
        // Print win message
        tft.setCursor(58,116);
        tft.setTextSize(3);
        tft.setTextColor(0xFFFF);
        tft.print("YOU WON");
      }
      // If the boss is still alive, activate the mechanic specific to the current weapon
      if (bossHP > 0) {
        bossMechanics(1);
      }
    }
  }
  
  else if (weapon == 1) {
    // If the hero is attacking in the boss room, boss mechanics take and action
    if((Characters[0].row == 0 && Characters[0].col == 1) && bossRoom == 1) {
      bossMechanics(0);
    }
    // Check if the hero is attacking a damageable object
    if(wMap[wRow][wCol][Characters[0].posX + 2*attacks[0]][Characters[0].posY + 2*attacks[1]] >= 100){
      int currentEnemy = wMap[wRow][wCol][Characters[0].posX + 2*attacks[0]][Characters[0].posY + 2*attacks[1]]-100;
      // Decrease the damageable object's health by 2
      Characters[currentEnemy].health -= 2;
      // Check if the object has been killed/destroyed
      if(Characters[currentEnemy].health <= 0) {
        Characters[currentEnemy].cStatus = 0;
        // Check what type of object was killed/destroyed
        // If it was an enemy, decrement that room's 'enemies' property and check if an action should be taken
        if (Characters[currentEnemy].type < 6) {
          Rooms[Characters[currentEnemy].row][Characters[currentEnemy].col].enemies--;
          checkEnemies();
        }
        // If it was a switch, decrement that room's 'switches' property and check if an action should be taken
        if (Characters[currentEnemy].type == 7) {
          Rooms[Characters[currentEnemy].row][Characters[currentEnemy].col].switches--;
          checkSwitches();
        }
        // If the killed/destroyed object was an enemy or a pot, chance for a heart to drop
        if(random(8) == 0 && Characters[currentEnemy].type < 7){
          Characters[currentEnemy].cTile = 5;
        }
        // Remove object from the map
        wMap[wRow][wCol][Characters[0].posX + 2*attacks[0]][Characters[0].posY + 2*attacks[1]] = Characters[currentEnemy].cTile;
        drawTile(Characters[currentEnemy].posX*SPACING,Characters[currentEnemy].posY*SPACING,Characters[currentEnemy].cTile);
      }
    }
    // Check if the boss is being attacked
    else if(wMap[wRow][wCol][Characters[0].posX + 2*attacks[0]][Characters[0].posY + 2*attacks[1]] >= 60 && wMap[wRow][wCol][Characters[0].posX + 2*attacks[0]][Characters[0].posY + 2*attacks[1]] <= 68) {
      // Decrease the boss's health by 2
      bossHP -= 2;
      // If the boss has been enraged, set bossLavaRate to 2 and call bossEnrage
      if (bossHP <= 12) {
        bossLavaRate = 2;
        bossEnrage();
      }
      // Check if the boss has been killed
      if(bossHP <= 0){
        // Remove all lava from the room
        magmaClear();
        // Set hero health to 0 - this prevents the player from continuing to move after the game ends
        Characters[0].health = 0;
        // Print win message
        tft.setCursor(58,116);
        tft.setTextSize(3);
        tft.setTextColor(0xFFFF);
        tft.print("YOU WON");
      }
      // If the boss is still alive, activate the mechanic specific to the current weapon
      if (bossHP > 0) {
        bossMechanics(2);
      }
    }
  }
  else if (weapon == 2) {
    // If the hero is attacking in the boss room, boss mechanics take and action
    if((Characters[0].row == 0 && Characters[0].col == 1) && bossRoom == 1) {
      bossMechanics(0);
    }
    // Integer variable to track number of tiles the arrow has moved
    int i = 0;
    // Boolean to break the loop if an enemy has been hit
    bool enemyHit = false;
    // Loop while the arrow is able to move onto the next tile and no enemy, switch or pot has been hit
    while(!enemyHit && Characters[0].posX + i*attacks[0] < 14 && Characters[0].posY + i*attacks[1] > 0 && (wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] < 10 || wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] == 14 || wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] == 15 || (wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] >= 50 && wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] <= 59) || wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] == 42)) {
      // Increment i so that the arrow does not immediately hit the hero
      i++;
      // If an enemy, switch or pot has been hit
      if (wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] >= 100) {
        // Set enemyHit to true, so that the loop will terminate after this cycle (i.e., the arrow will not penetrate damageable objects)
        enemyHit = true;
        // Determine position of the object being attacked within the Characters[] array
        int currentEnemy = wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]]-100;
        // Decrement the enemy's health
        Characters[currentEnemy].health--;
        // Check if the object has been killed/destroyed
        if(Characters[currentEnemy].health <= 0) {
          Characters[currentEnemy].cStatus = 0;
          // Check what type of object was killed/destroyed
          // If it was an enemy, decrement that room's 'enemies' property and check if an action should be taken
          if (Characters[currentEnemy].type < 6) {
            Rooms[Characters[currentEnemy].row][Characters[currentEnemy].col].enemies--;
            checkEnemies();
          }
          // If it was a switch, decrement that room's 'switches' property and check if an action should be taken
          if (Characters[currentEnemy].type == 7) {
            Rooms[Characters[currentEnemy].row][Characters[currentEnemy].col].switches--;
            checkSwitches();
          }
          // If the killed/destroyed object was an enemy or a pot, chance for a heart to drop
          if(random(8) == 0 && Characters[currentEnemy].type < 7){
            Characters[currentEnemy].cTile = 5;
          }
          // Remove object from the map
          wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] = Characters[currentEnemy].cTile;
          drawTile(Characters[currentEnemy].posX*SPACING,Characters[currentEnemy].posY*SPACING,Characters[currentEnemy].cTile);
        }
      }
      // // Check if the boss is being attacked
      else if(wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] >= 60 && wMap[wRow][wCol][Characters[0].posX + i*attacks[0]][Characters[0].posY + i*attacks[1]] <= 68) {
        // Decrement the boss's health
        bossHP--;
        // If bossRoom is false, set it to true - this prevents the player from exploiting the bow's range in the boss battle
        if ((Characters[0].row == 0 && Characters[0].col == 1) && bossRoom == false) { // Is this necessary?
          bossRoom = true;
        }
        // If the boss has been enraged, set bossLavaRate to 2 and call bossEnrage
        if (bossHP <= 12) {
          bossLavaRate = 2;
          bossEnrage();
        }
        // Check if the boss has been killed
        if(bossHP <= 0){
          // Remove all lava from the room
          magmaClear();
          // Set hero health to 0 - this prevents the player from continuing to move after the game ends
          Characters[0].health = 0;
          // Print win message
          tft.setCursor(58,116);
          tft.setTextSize(3);
          tft.setTextColor(0xFFFF);
          tft.print("YOU WON");
        }
        // If the boss is still alive, activate the mechanic specific to the current weapon
        if(bossHP > 0){
          bossMechanics(3);
        }
      }
      // If nothing has been hit, draw arrow animation
      else if (wMap[wRow][wCol][Characters[0].posX + (i+1)*attacks[0]][Characters[0].posY + (i+1)*attacks[1]] < 10 || wMap[wRow][wCol][Characters[0].posX + (i+1)*attacks[0]][Characters[0].posY + (i+1)*attacks[1]] == 14 || wMap[wRow][wCol][Characters[0].posX + (i+1)*attacks[0]][Characters[0].posY + (i+1)*attacks[1]] == 15 || (wMap[wRow][wCol][Characters[0].posX + (i+1)*attacks[0]][Characters[0].posY + (i+1)*attacks[1]] >= 50 && wMap[wRow][wCol][Characters[0].posX + (i+1)*attacks[0]][Characters[0].posY + (i+1)*attacks[1]] <= 59)){
        // Call drawArrow every two pixels for one tile
        for (int j = 0; j < 16; j++) {
          drawArrow(i, j);
          j++;
        }
      }
    }
  }
}

// If the boss has been enraged, change all the inner walls of the boss room to lava
void bossEnrage() {
  for (int i = 0; i<15; i++) {
    for (int j = 0; j<15; j++) {
      if (wMap[wRow][wCol][i][j] == 16) {
        wMap[wRow][wCol][i][j] = 51;
        drawTile(i*SPACING,j*SPACING,wMap[wRow][wCol][i][j]);
      }
    }
  }
}

// Clear lava from boss room
void magmaClear() {
  for(int i = 0;i<15;i++) {
    for(int j = 0;j<15;j++) {
      if(wMap[wRow][wCol][i][j] == 50 || wMap[wRow][wCol][i][j] == 51 || wMap[wRow][wCol][i][j] == 52) {
        wMap[wRow][wCol][i][j] = 2;
        drawTile(i*SPACING,j*SPACING,wMap[wRow][wCol][i][j]);
      }
      if(wMap[wRow][wCol][i][j] >= 63 && wMap[wRow][wCol][i][j] <= 68) {
        wMap[wRow][wCol][i][j] = 15;
        drawTile(i*SPACING,j*SPACING,wMap[wRow][wCol][i][j]);
      }
      if(wMap[wRow][wCol][i][j] >= 60 && wMap[wRow][wCol][i][j] <= 62) {
        wMap[wRow][wCol][i][j] = 10;
        drawTile(i*SPACING,j*SPACING,wMap[wRow][wCol][i][j]);
      }
    }
  }
  Characters[0].cTile = 2;
  drawHero(Characters[0].posX,Characters[0].posY);
}

// Draw the side inventory border
void gameBorder() {
  int borderColour = 0x096F;
  tft.fillRect(15*SPACING,0*SPACING,SPACING*5,SPACING,borderColour);
  tft.fillRect(15*SPACING,1*SPACING,SPACING,SPACING*13,borderColour);
  tft.fillRect(15*SPACING,14*SPACING,SPACING*5,SPACING,borderColour);
  tft.fillRect(19*SPACING,1*SPACING,SPACING,SPACING*13,borderColour);
  for(int i = 1;i<4;i++){
    borderTile(15+i,0,0);
  }
  for(int i = 0;i<13;i++) {
    borderTile(15,1+i,1);
  }
  borderTile(0,0,2);
}

// Draw the hero's current health as Characters[0].health/4 hearts
void hearts() {
  // If the character's health exceeds maxHealth, reduce health to maxHealth
    if(Characters[0].health > maxHealth)
    Characters[0].health = maxHealth;

  if(Characters[0].health <= 0)
    Characters[0].health = 1;
  
  //Clear the old hearts
  tft.fillRect(16*SPACING,1*SPACING,48,32,0x0000);
  int fullHearts = Characters[0].health/4;
  int partHeart = Characters[0].health%4;
  int i,j = 1,row = 0;
  //Display the full hearts
  for(i = 16;i<fullHearts+16;i++){
    drawTile((i-row)*SPACING,j*SPACING,34);
    if(i == 16+2){
      j++;
      row = 3;
    }
  }
  //Display the part heart
  switch(partHeart){
    case 1: drawTile((i-row)*SPACING,j*SPACING,31);
            break;
    case 2: drawTile((i-row)*SPACING,j*SPACING,32);
            break;
    case 3: drawTile((i-row)*SPACING,j*SPACING,33);
            break;
  }
}

// Place breakables (hearts and switches) in the current room
void placeBreakables() {
  // For all breakables
  for (int i = ENEMYCOUNT + 1; i < ENEMYCOUNT + BREAKABLECOUNT + 1; i++) {
    // Check if the breakable has not been destroyed and is in the current room (world row and world column)
    if (Characters[i].row == wRow && Characters[i].col == wCol && Characters[i].cStatus == 1) {
      // Place the breakable on the world map
      wMap[wRow][wCol][Characters[i].posX][Characters[i].posY] = i + 100;
      // Draw the breakable onto the screen
      drawBreakable(i);
    }
  }
  // What?!  How did this get here?
  for (int i = 33; i < 38; i++) {
    if (Characters[i].row == wRow && Characters[i].col == wCol && Characters[i].cStatus == 1) {
      wMap[wRow][wCol][Characters[i].posX][Characters[i].posY] = i + 100;
      drawTile(Characters[i].posX*SPACING, Characters[i].posY*SPACING, 10);
    }
  }
}

// Call each enemy in the room to move
void enemiesMove(){
  // For all enemies
  for(int i = 1; i < ENEMYCOUNT + 1; i++){
    // Call the enemy to move if it is alive and in the current room (world row and world column)
    if(Characters[i].cStatus == 1 && (Characters[i].col == Characters[0].col && Characters[i].row == Characters[0].row))
      enemyMove(i);
  }
}

// Function controlling enemy movement
void enemyMove(int i) {
  // Integers to track where the enemy 'wants' to move - initialized at current enemy position
  int row = Characters[i].posX; // Currently, row and column are misassigned to X and Y - oops!  This is something to be fixed.
  int column = Characters[i].posY;
  // If the enemy is more than six tiles away from the hero, generate a random move (up, down, left, right or none)
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
  // If the enemy is exactly one tile away from the here, attack
  else if(charDistance(Characters[i].posX, Characters[i].posY) == 1) {
    // Set enemy direction based on relative position of hero
    if(Characters[0].posX < Characters[i].posX)
      Characters[i].cDirection = 0;
    else if(Characters[0].posY < Characters[i].posY)
      Characters[i].cDirection = 1;
    else if(Characters[0].posX > Characters[i].posX)
      Characters[i].cDirection = 2;
    else
      Characters[i].cDirection = 3;
    // Draw the enemy facing the selected direction
    drawEnemy(i);
    // If the enemy is of type 2 or 4 (helmeted, stronger enemies), decrease hero health by two
    if(Characters[i].type == 2 || Characters[i].type == 4)
      Characters[0].health-=2;
    // Else, decrement hero health
    else
      Characters[0].health--;
    // Draw hero health on the side inventory
    hearts();
    // If the hero has been killed
    if(Characters[0].health <= 0) {
      // Remove the hero from the map
      drawTile(Characters[0].posX*SPACING, Characters[0].posY*SPACING,Characters[i].cTile);
      wMap[wRow][wCol][Characters[0].posX][Characters[0].posY] = Characters[0].cTile;
      // Draw loss message
      tft.setCursor(41,110);
      tft.setTextSize(3);
      tft.setTextColor(0xF800);
      tft.print("GAME OVER");
    }
  }
  // If the enemy is <= 6 and > 1 tile from the hero, try to move toward the hero
  else {
    // Float to track most ideal move - initialized to 100 to ensure the enemy moves if it can
    float minDist = 100;
    // Integer to ensure the enemy moves
    int loops = 0;
    // Integer to store the currently selected move
    int currentMove;
    while (loops < 2) {
      // If moving LEFT is a valid move, it lowers distance from the hero below minDist, and the enemy was either not in that position last turn OR no move was selected the first time through the loop
      if (sqrt(pow(((Characters[i].posX - 1) - Characters[0].posX),2) + pow(Characters[i].posY - Characters[0].posY,2)) < minDist && (Characters[i].lastMove != 1 || loops > 0) && (wMap[wRow][wCol][Characters[i].posX - 1][Characters[i].posY] <= 9 || (wMap[wRow][wCol][Characters[i].posX - 1][Characters[i].posY] >= 50 && wMap[wRow][wCol][Characters[i].posX - 1][Characters[i].posY] <= 59))) {
        // Set minDist to what the distance from the hero would be if this move were made
        minDist = sqrt(pow(((row - 1) - Characters[0].posX),2) + pow(column - Characters[0].posY,2));
        // Set column to enemy's current Y position
        column = Characters[i].posY;
        // Set row to enemy's current X position minus 1
        row = Characters[i].posX - 1;
        // Set currentMove - this prevents the enemy from returning to its current position next turn, unless it is the only possible move
        currentMove = 2;
      }
      // If moving RIGHT is a valid move, it lowers distance from the hero below minDist, and the enemy was either not in that position last turn OR no move was selected the first time through the loop
      if (sqrt(pow(((Characters[i].posX + 1) - Characters[0].posX),2) + pow(Characters[i].posY - Characters[0].posY,2)) < minDist && (Characters[i].lastMove != 2 || loops > 0) && (wMap[wRow][wCol][Characters[i].posX + 1][Characters[i].posY] <= 9 || (wMap[wRow][wCol][Characters[i].posX + 1][Characters[i].posY] >= 50 && wMap[wRow][wCol][Characters[i].posX + 1][Characters[i].posY] <= 59))) {
        // Set minDist to what the distance from the hero would be if this move were made
        minDist = sqrt(pow(((row + 1) - Characters[0].posX),2) + pow(column - Characters[0].posY,2));
        // Set column to enemy's current Y position
        column = Characters[i].posY;
        // Set row to enemy's current X position plus 1
        row = Characters[i].posX + 1;
        currentMove = 1;
      }
      // If moving UP is a valid move, it lowers distance from the hero below minDist, and the enemy was either not in that position last turn OR no move was selected the first time through the loop
      if (sqrt(pow((Characters[i].posX - Characters[0].posX),2) + pow((Characters[i].posY - 1) - Characters[0].posY,2)) < minDist && (Characters[i].lastMove != 3 || loops > 0) && (wMap[wRow][wCol][Characters[i].posX][Characters[i].posY - 1] <= 9 || (wMap[wRow][wCol][Characters[i].posX][Characters[i].posY - 1] >= 50 && wMap[wRow][wCol][Characters[i].posX][Characters[i].posY - 1] <= 59))) {
        // Set minDist to what the distance from the hero would be if this move were made
        minDist = sqrt(pow((row - Characters[0].posX),2) + pow((column - 1) - Characters[0].posY,2));
        // Set row to enemy's current X
        row = Characters[i].posX;
        // Set row to enemy's current Y position minus 1
        column = Characters[i].posY - 1;
        currentMove = 4;
      }
      // If moving DOWN is a valid move, it lowers distance from the hero below minDist, and the enemy was either not in that position last turn OR no move was selected the first time through the loop
      if (sqrt(pow((Characters[i].posX - Characters[0].posX),2) + pow((Characters[i].posY + 1) - Characters[0].posY,2)) < minDist && (Characters[i].lastMove != 4 || loops > 0) && (wMap[wRow][wCol][Characters[i].posX][Characters[i].posY + 1] <= 9 || (wMap[wRow][wCol][Characters[i].posX][Characters[i].posY + 1] >= 50 && wMap[wRow][wCol][Characters[i].posX][Characters[i].posY + 1] <= 59))) {
        // Set minDist to what the distance from the hero would be if this move were made
        minDist = sqrt(pow((row - Characters[0].posX),2) + pow((column + 1) - Characters[0].posY,2));
        // Set row to enemy's current X
        row = Characters[i].posX;
        // Set row to enemy's current Y position plus 1
        column = Characters[i].posY + 1;
        currentMove = 3;
      }
      // If no move has been selected, increment loops - this lets the loop run one more time, now allowing movement onto the enemy's previously occupied tile
      if (row == Characters[i].posX && column == Characters[i].posY)
        loops++;
      //  If a move has been selected, increase loops by two - this breaks the loop
      else
        loops = 2;
    }
    // Set the enemy's 'lastMove' property to currentMove
    Characters[i].lastMove = currentMove;
  }
  // Remove the enemy from their current position
  drawTile(Characters[i].posX*SPACING, Characters[i].posY*SPACING,Characters[i].cTile);
  wMap[wRow][wCol][Characters[i].posX][Characters[i].posY] = Characters[i].cTile;
  // Place the enemy in their new position
  Characters[i].cTile = wMap[wRow][wCol][row][column];
  wMap[wRow][wCol][row][column] = 100+i;
  // Determine the enemy's direction based on how their position changed
  if(Characters[i].posX > row)
    Characters[i].cDirection = 0; 
  else if(Characters[i].posX < row)
    Characters[i].cDirection = 2;
  else if(Characters[i].posY > column)
    Characters[i].cDirection = 1;
  else if(Characters[i].posY < column)
    Characters[i].cDirection = 3;
  // Set the enemy's new 'posX' and 'posY' properties
  Characters[i].posX = row;
  Characters[i].posY = column;
  // Draw the enemy in their new position
  drawEnemy(i);
}

// Torin you better comment this (? so that this shows up on ctrl+F)
void bossMechanics(int mechanicNum) {
  int rate = 0;
  if(mechanicNum == 0 || mechanicNum == 3){
    if(mechanicNum == 0) {
      rate = bossLavaRate;
    }
    else {
      rate = 2;
    }
    if(random(rate) == 0 && Characters[0].cTile == 2) {
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
        if(wMap[wRow][wCol][i][j] == 2  && random(rate) == 0  ) {
          wMap[wRow][wCol][i][j] = 50;
          drawTile(i*SPACING,j*SPACING,wMap[wRow][wCol][i][j]);
        }
        else if(wMap[wRow][wCol][i][j] == 50) {
          wMap[wRow][wCol][i][j] = 51;
          drawTile(i*SPACING,j*SPACING,wMap[wRow][wCol][i][j]);
        }
        else if(wMap[wRow][wCol][i][j] == 51) {
          wMap[wRow][wCol][i][j] = 52;
          drawTile(i*SPACING,j*SPACING,wMap[wRow][wCol][i][j]);
        }
        else if(wMap[wRow][wCol][i][j] == 52) {
          wMap[wRow][wCol][i][j] = 2;
          drawTile(i*SPACING,j*SPACING,wMap[wRow][wCol][i][j]);
        }
      }
    }
  }
  else if(mechanicNum == 1) {
    for(int i = 2;i<4;i++) {
      if(wMap[wRow][wCol][5][i] != 42) {
        if(wMap[wRow][wCol][5][i] == 2) {
          wMap[wRow][wCol][5][i] = 50;
          drawTile(5*SPACING,i*SPACING,wMap[wRow][wCol][5][i]);
        }
        else if(wMap[wRow][wCol][5][i] < 52) {
          wMap[wRow][wCol][5][i]++;
          drawTile(5*SPACING,i*SPACING,wMap[wRow][wCol][5][i]);
        }
        else if(wMap[wRow][wCol][5][i] == 52) {
          wMap[wRow][wCol][5][i] = 2;
          drawTile(5*SPACING,i*SPACING,wMap[wRow][wCol][5][i]);
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
          drawTile(9*SPACING,i*SPACING,wMap[wRow][wCol][9][i]);
        }
        else if(wMap[wRow][wCol][9][i] < 52) {
          wMap[wRow][wCol][9][i]++;
          drawTile(9*SPACING,i*SPACING,wMap[wRow][wCol][9][i]);
        }
        else if(wMap[wRow][wCol][9][i] == 52) {
          wMap[wRow][wCol][9][i] = 2;
          drawTile(9*SPACING,i*SPACING,wMap[wRow][wCol][9][i]);
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
          drawTile(i*SPACING,3*SPACING,wMap[wRow][wCol][i][3]);
        }
        else if(wMap[wRow][wCol][i][3] < 52) {
          wMap[wRow][wCol][i][3]++;
          drawTile(i*SPACING,3*SPACING,wMap[wRow][wCol][i][3]);
        }
        else if(wMap[wRow][wCol][i][3] == 52) {
          wMap[wRow][wCol][i][3] = 2;
          drawTile(i*SPACING,3*SPACING,wMap[wRow][wCol][i][3]);
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
  else if(mechanicNum == 2) {
    for(int i = 2;i<5;i++) {
      if(wMap[wRow][wCol][4][i] != 42) {
        if(wMap[wRow][wCol][4][i] == 2) {
          wMap[wRow][wCol][4][i] = 50;
          drawTile(4*SPACING,i*SPACING,wMap[wRow][wCol][4][i]);
        }
        else if(wMap[wRow][wCol][4][i] < 52) {
          wMap[wRow][wCol][4][i]++;
          drawTile(4*SPACING,i*SPACING,wMap[wRow][wCol][4][i]);
        }
        else if(wMap[wRow][wCol][4][i] == 52) {
          wMap[wRow][wCol][4][i] = 2;
          drawTile(4*SPACING,i*SPACING,wMap[wRow][wCol][4][i]);
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
      if(wMap[wRow][wCol][10][i] != 42) {
        if(wMap[wRow][wCol][10][i] == 2) {
          wMap[wRow][wCol][10][i] = 50;
          drawTile(10*SPACING,i*SPACING,wMap[wRow][wCol][10][i]);
        }
        else if(wMap[wRow][wCol][10][i] < 52) {
          wMap[wRow][wCol][10][i]++;
          drawTile(10*SPACING,i*SPACING,wMap[wRow][wCol][10][i]);
        }
        else if(wMap[wRow][wCol][10][i] == 52) {
          wMap[wRow][wCol][10][i] = 2;
          drawTile(10*SPACING,i*SPACING,wMap[wRow][wCol][10][i]);
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
    
    for(int i = 5;i<10;i++) {
      if(wMap[wRow][wCol][i][4] != 42) {
        if(wMap[wRow][wCol][i][4] == 2) {
          wMap[wRow][wCol][i][4] = 50;
          drawTile(i*SPACING,4*SPACING,wMap[wRow][wCol][i][4]);
        }
        else if(wMap[wRow][wCol][i][4] < 52) {
          wMap[wRow][wCol][i][4]++;
          drawTile(i*SPACING,4*SPACING,wMap[wRow][wCol][i][4]);
        }
        else if(wMap[wRow][wCol][i][4] == 52) {
          wMap[wRow][wCol][i][4] = 2;
          drawTile(i*SPACING,4*SPACING,wMap[wRow][wCol][i][4]);
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

// Function to perform actions based on the number of enemies in a room
void checkEnemies() {
  // If there are no enemies in the current room
  if (Rooms[wRow][wCol].enemies == 0 ) {
    // If in room [2,0]
    if (wRow == 2 && wCol == 0) {
      // Remove wall from topmost centre tile, allowing the hero to move to room [1,0]
      wMap[wRow][wCol][7][0] = 0;
      drawTile(7*SPACING, 0, 0);
      // Remove wall from rightmost middle tile, allowing the hero to move to room [2,1]
      wMap[wRow][wCol][14][7] = 0;
      drawTile(14*SPACING, 7*SPACING, 0);
    }
    // If in room [1,2]
    if (wRow == 1 && wCol == 2) {
      // Remove wall from room tile [4,7], allowing the player to access the bow
      wMap[wRow][wCol][4][7] = 0;
      drawTile(4*SPACING, 7*SPACING, 0);
    }
  }
}

// Function to perform actions based on the number of switches in a room
void checkSwitches() {
  // If there are no switches in the current room
  if (Rooms[wRow][wCol].switches == 0) {
    // If in room [0,0]
    if (wRow == 0 && wCol == 0) {
      // Place heart container
      wMap[wRow][wCol][7][3] = 7;
      drawTile(7*SPACING, 3*SPACING, 7);
    }
    // If in room [0,2]
    else if (wRow == 0 && wCol == 2) {
      // Place heart container
      wMap[wRow][wCol][7][2] = 7;
      drawTile(7*SPACING, 2*SPACING, 7);
    }
    // If in room [1,0]
    else if (wRow == 1 && wCol == 0) {
      // Place a bridge from the lower doorway to the platform holding the trident
      wMap[wRow][wCol][7][11] = 1;
      drawTile(7*SPACING, 11*SPACING, 1);
      wMap[wRow][wCol][7][12] = 1;
      drawTile(7*SPACING, 12*SPACING, 1);
      // Place a bridge from the centre 'island' to the upper doorway
      wMap[wRow][wCol][7][2] = 1;
      drawTile(7*SPACING, 2*SPACING, 1);
      wMap[wRow][wCol][7][3] = 1;
      drawTile(7*SPACING, 3*SPACING, 1);
      wMap[wRow][wCol][7][4] = 1;
      drawTile(7*SPACING, 4*SPACING, 1);
      wMap[wRow][wCol][7][5] = 1;
      drawTile(7*SPACING, 5*SPACING, 1);
    }
  }
}

// Function to calculate distance between something and the hero
float charDistance(int X,int Y) {
   return sqrt(pow((Characters[0].posX - X), 2) + pow((Characters[0].posY - Y), 2));
}

// Function to set the hero's direction
void faceDirection() {
  // Check each if each limit switch is pressed
  for(int i = 0; i < 4; i++) {
    dPad[i] = digitalRead(22+i);
    // If the current limit switch is pressed
    if(!dPad[i] && Characters[0].cDirection !=i) {
      // Set the hero's 'cDirection' property
      Characters[0].cDirection = i;
      // Draw the hero facing the selected direction
      drawHero(Characters[0].posX,Characters[0].posY); // Couldn't this be put outside the for loop?
    }
  }
}

// Function to remove a tile from the display
void deleteChunk(int x,int y) {
  // Draw a black rectangle over the selected tile
  tft.fillRect(x*SPACING,y*SPACING,16,16,ILI9340_BLACK);
}

// Function to draw the current room
void drawMap() {
  // Fill the entire screen with black
  tft.fillScreen(ILI9340_BLACK);
  // Draw all tiles in the current room
  for(int i = 0; i < 15 ; i++) {
    for(int j = 0; j < 15; j++) {
      drawTile(i*SPACING, j*SPACING, wMap[wRow][wCol][i][j]);
    }
  }
}

// Function to draw the arrow?
void drawArrow(int tilesMoved, int pixelsMoved) {
  // If the character is facing LEFT
  if (Characters[0].cDirection == 0) {
    int refX = (Characters[0].posX + 1 - tilesMoved)*SPACING - pixelsMoved - 1;
    int refY = Characters[0].posY*SPACING;
    tft.drawFastHLine(refX - 12, refY + 7, 11, 0xC440);
    tft.drawFastVLine(refX - 13, refY + 5, 5, 0xA513);
    tft.drawFastVLine(refX - 14, refY + 6, 3, 0xA513);
    tft.drawPixel(refX - 15, refY + 7, 0xA513);
    tft.drawFastVLine(refX - 1, refY + 6, 3, 0xD000);
    tft.drawPixel(refX, refY + 5, 0xD000);
    tft.drawPixel(refX, refY + 7, 0xD000);
    tft.drawPixel(refX, refY + 9, 0xD000);
    if (wMap[wRow][wCol][Characters[0].posX - tilesMoved][Characters[0].posY] < 100)
       drawTile((Characters[0].posX - tilesMoved)*SPACING, refY, wMap[wRow][wCol][Characters[0].posX - tilesMoved][Characters[0].posY]);
    if (wMap[wRow][wCol][Characters[0].posX - tilesMoved - 1][Characters[0].posY] < 100)
       drawTile((Characters[0].posX - tilesMoved - 1)*SPACING, refY, wMap[wRow][wCol][Characters[0].posX - tilesMoved - 1][Characters[0].posY]);
  }
  // If the character is facing UP
  else if (Characters[0].cDirection == 1) {
    int refX = (Characters[0].posX + 1)*SPACING;
    int refY = (Characters[0].posY - tilesMoved)*SPACING - pixelsMoved;
    if ((Characters[0].posY - tilesMoved - 1) > 0) {
      tft.drawFastVLine(refX - 7, refY + 3, 11, 0xC440);
      tft.drawFastHLine(refX - 9, refY + 2, 5, 0xA513);
      tft.drawFastHLine(refX - 8, refY + 1, 3, 0xA513);
      tft.drawPixel(refX - 7, refY, 0xA513);
      tft.drawFastHLine(refX - 8, refY + 14, 3, 0xD000);
      tft.drawPixel(refX - 9, refY + 15, 0xD000);
      tft.drawPixel(refX - 7, refY + 15, 0xD000);
      tft.drawPixel(refX - 5, refY + 15, 0xD000);
    }
    if (wMap[wRow][wCol][Characters[0].posX][Characters[0].posY - tilesMoved] < 100)
       drawTile(Characters[0].posX*SPACING, (Characters[0].posY - tilesMoved)*SPACING, wMap[wRow][wCol][Characters[0].posX][Characters[0].posY - tilesMoved]);
    if (wMap[wRow][wCol][Characters[0].posX][Characters[0].posY - tilesMoved - 1] < 100 && (Characters[0].posY - tilesMoved - 1) > 0)
       drawTile(Characters[0].posX*SPACING, (Characters[0].posY - tilesMoved - 1)*SPACING, wMap[wRow][wCol][Characters[0].posX][Characters[0].posY - tilesMoved - 1]);
  }
  // If the character is facing RIGHT
  else if (Characters[0].cDirection == 2) {
    int refX = (Characters[0].posX + 1 + tilesMoved)*SPACING + pixelsMoved;
    int refY = Characters[0].posY*SPACING;
    if ((Characters[0].posX + tilesMoved + 1) < 14) {
      tft.drawFastHLine(refX - 13, refY + 7, 11, 0xC440);
      tft.drawFastVLine(refX - 2, refY + 5, 5, 0xA513);
      tft.drawFastVLine(refX - 1, refY + 6, 3, 0xA513);
      tft.drawPixel(refX, refY + 7, 0xA513);
      tft.drawFastVLine(refX - 14, refY + 6, 3, 0xD000);
      tft.drawPixel(refX - 15, refY + 5, 0xD000);
      tft.drawPixel(refX - 15, refY + 7, 0xD000);
      tft.drawPixel(refX - 15, refY + 9, 0xD000);
    }
    if (wMap[wRow][wCol][Characters[0].posX + tilesMoved][Characters[0].posY] < 100)
       drawTile((Characters[0].posX + tilesMoved)*SPACING, refY, wMap[wRow][wCol][Characters[0].posX + tilesMoved][Characters[0].posY]);
    if (wMap[wRow][wCol][Characters[0].posX + tilesMoved + 1][Characters[0].posY] < 100 && (Characters[0].posX + tilesMoved + 1) < 14)
       drawTile((Characters[0].posX + tilesMoved + 1)*SPACING, refY, wMap[wRow][wCol][Characters[0].posX + tilesMoved + 1][Characters[0].posY]);
  }
  // If the character is facing DOWN
  else if (Characters[0].cDirection == 3) {
    int refX = (Characters[0].posX + 1)*SPACING;
    int refY = (Characters[0].posY + tilesMoved)*SPACING + pixelsMoved;
    tft.drawFastVLine(refX - 7, refY + 2, 11, 0xC440);
    tft.drawFastHLine(refX - 9, refY + 13, 5, 0xA513);
    tft.drawFastHLine(refX - 8, refY + 14, 3, 0xA513);
    tft.drawPixel(refX - 7, refY + 15, 0xA513);
    tft.drawFastHLine(refX - 8, refY + 1, 3, 0xD000);
    tft.drawPixel(refX - 9, refY, 0xD000);
    tft.drawPixel(refX - 7, refY, 0xD000);
    if (wMap[wRow][wCol][Characters[0].posX][Characters[0].posY + tilesMoved] < 100)
       drawTile(Characters[0].posX*SPACING, (Characters[0].posY + tilesMoved)*SPACING, wMap[wRow][wCol][Characters[0].posX][Characters[0].posY + tilesMoved]);
    if (wMap[wRow][wCol][Characters[0].posX][Characters[0].posY + tilesMoved + 1] < 100)
       drawTile(Characters[0].posX*SPACING, (Characters[0].posY + tilesMoved + 1)*SPACING, wMap[wRow][wCol][Characters[0].posX][Characters[0].posY + tilesMoved + 1]);
  }
}

// Function to draw switches and pots
void drawBreakable(int i) {
  // Integers for X and Y coordinates
  int x = Characters[i].posX*SPACING;
  int y = Characters[i].posY*SPACING;
  // Draw the selected object's cTile
  drawTile(Characters[i].posX*SPACING, Characters[i].posY*SPACING, Characters[i].cTile);
  // If the object is a switch, draw a switch
  if (Characters[i].type == 7) {
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
  // If the object is a pot, draw a pot
  else if (Characters[i].type == 6) {
    // Body fill (brown)
    tft.fillRoundRect(x + 3, y + 3, 10, 10, 3, 0x8AC6);
    // Outline (black)
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
    // Ring (yellow and black)
    tft.drawFastHLine(x + 6, y + 2, 4, 0xED46);
    tft.drawFastHLine(x + 6, y + 6, 4, 0xED46);
    tft.drawFastVLine(x + 5, y + 3, 3, 0xED46);
    tft.drawFastVLine(x + 10, y + 3, 3, 0xED46);
    tft.fillRoundRect(x + 6, y + 3, 4, 3, 2, ILI9340_BLACK);
  }
}

// Function to draw border of side inventory?
void borderTile(int xPos,int yPos,int side) {
  int borderColour = 0xB463;
  if(side == 0) {
    //Top Border
    tft.drawFastVLine(xPos*SPACING,yPos*SPACING+5,4,borderColour);
    tft.drawFastVLine(xPos*SPACING+15,yPos*SPACING+5,4,borderColour);
    tft.drawFastVLine(xPos*SPACING+1,yPos*SPACING+4,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+14,yPos*SPACING+4,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+2,yPos*SPACING+3,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+13,yPos*SPACING+3,2,borderColour);
    tft.drawPixel(xPos*SPACING+3,yPos*SPACING+3,borderColour);
    tft.drawPixel(xPos*SPACING+12,yPos*SPACING+3,borderColour);
    tft.drawFastVLine(xPos*SPACING+4,yPos*SPACING+3,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+11,yPos*SPACING+3,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+5,yPos*SPACING+4,3,borderColour);
    tft.drawFastVLine(xPos*SPACING+10,yPos*SPACING+4,3,borderColour);
    tft.drawFastVLine(xPos*SPACING+6,yPos*SPACING+4,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+9,yPos*SPACING+4,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+7,yPos*SPACING+3,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+8,yPos*SPACING+3,2,borderColour);
    tft.fillRect(xPos*SPACING+1,yPos*SPACING+7,3,2,borderColour);
    tft.fillRect(xPos*SPACING+12,yPos*SPACING+7,3,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+4,yPos*SPACING+6,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+11,yPos*SPACING+6,2,borderColour);
    tft.fillRect(xPos*SPACING,yPos*SPACING+11,16,2,borderColour);
    tft.drawFastHLine(xPos*SPACING+4,yPos*SPACING+10,8,borderColour);
    tft.drawFastHLine(xPos*SPACING+6,yPos*SPACING+9,4,borderColour);
    tft.drawFastHLine(xPos*SPACING+7,yPos*SPACING+8,2,borderColour);
    yPos+=14;
    //Bottom Border
    tft.fillRect(xPos*SPACING,yPos*SPACING+3,16,2,borderColour);
    tft.drawFastHLine(xPos*SPACING+4,yPos*SPACING+5,8,borderColour);
    tft.drawFastHLine(xPos*SPACING+6,yPos*SPACING+6,4,borderColour);
    tft.drawFastHLine(xPos*SPACING+7,yPos*SPACING+7,2,borderColour);
    tft.drawFastVLine(xPos*SPACING,yPos*SPACING+7,4,borderColour);
    tft.drawFastVLine(xPos*SPACING+15,yPos*SPACING+7,4,borderColour);
    tft.drawFastVLine(xPos*SPACING+1,yPos*SPACING+10,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+14,yPos*SPACING+10,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+2,yPos*SPACING+11,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+13,yPos*SPACING+11,2,borderColour);
    tft.drawPixel(xPos*SPACING+3,yPos*SPACING+12,borderColour);
    tft.drawPixel(xPos*SPACING+12,yPos*SPACING+12,borderColour);
    tft.drawFastVLine(xPos*SPACING+4,yPos*SPACING+11,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+11,yPos*SPACING+11,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+5,yPos*SPACING+9,3,borderColour);
    tft.drawFastVLine(xPos*SPACING+10,yPos*SPACING+9,3,borderColour);
    tft.drawFastVLine(xPos*SPACING+6,yPos*SPACING+10,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+9,yPos*SPACING+10,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+7,yPos*SPACING+11,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+8,yPos*SPACING+11,2,borderColour);
    tft.fillRect(xPos*SPACING+1,yPos*SPACING+7,3,2,borderColour);
    tft.fillRect(xPos*SPACING+12,yPos*SPACING+7,3,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+4,yPos*SPACING+8,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+11,yPos*SPACING+8,2,borderColour);
  }
  else if(side == 1) {
    //Left Border
    tft.drawFastVLine(xPos*SPACING+3,yPos*SPACING+2,3,borderColour);
    tft.drawFastVLine(xPos*SPACING+3,yPos*SPACING+7,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+3,yPos*SPACING+11,3,borderColour);
    tft.drawFastVLine(xPos*SPACING+4,yPos*SPACING+1,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+4,yPos*SPACING+4,8,borderColour);
    tft.drawFastVLine(xPos*SPACING+4,yPos*SPACING+13,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+5,yPos*SPACING,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+5,yPos*SPACING+5,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+5,yPos*SPACING+9,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+5,yPos*SPACING+14,2,borderColour);
    tft.drawPixel(xPos*SPACING+6,yPos*SPACING,borderColour);
    tft.drawFastVLine(xPos*SPACING+6,yPos*SPACING+4,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+6,yPos*SPACING+10,2,borderColour);
    tft.drawPixel(xPos*SPACING+6,yPos*SPACING+15,borderColour);
    tft.drawFastVLine(xPos*SPACING+7,yPos*SPACING,5,borderColour);
    tft.drawFastVLine(xPos*SPACING+7,yPos*SPACING+11,5,borderColour);
    tft.drawFastVLine(xPos*SPACING+8,yPos*SPACING,4,borderColour);
    tft.drawFastVLine(xPos*SPACING+8,yPos*SPACING+7,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+8,yPos*SPACING+12,4,borderColour);
    tft.drawFastVLine(xPos*SPACING+9,yPos*SPACING+6,4,borderColour);
    tft.drawFastVLine(xPos*SPACING+10,yPos*SPACING+4,8,borderColour);
    tft.drawFastVLine(xPos*SPACING+11,yPos*SPACING,16,borderColour);
    tft.drawFastVLine(xPos*SPACING+12,yPos*SPACING,16,borderColour);
    xPos+=4;
    //Right Border
    tft.drawFastVLine(xPos*SPACING+12,yPos*SPACING+2,3,borderColour);
    tft.drawFastVLine(xPos*SPACING+12,yPos*SPACING+7,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+12,yPos*SPACING+11,3,borderColour);
    tft.drawFastVLine(xPos*SPACING+11,yPos*SPACING+1,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+11,yPos*SPACING+4,8,borderColour);
    tft.drawFastVLine(xPos*SPACING+11,yPos*SPACING+13,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+10,yPos*SPACING,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+10,yPos*SPACING+5,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+10,yPos*SPACING+9,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+10,yPos*SPACING+14,2,borderColour);
    tft.drawPixel(xPos*SPACING+9,yPos*SPACING,borderColour);
    tft.drawFastVLine(xPos*SPACING+9,yPos*SPACING+4,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+9,yPos*SPACING+10,2,borderColour);
    tft.drawPixel(xPos*SPACING+9,yPos*SPACING+15,borderColour);
    tft.drawFastVLine(xPos*SPACING+8,yPos*SPACING,5,borderColour);
    tft.drawFastVLine(xPos*SPACING+8,yPos*SPACING+11,5,borderColour);
    tft.drawFastVLine(xPos*SPACING+7,yPos*SPACING,4,borderColour);
    tft.drawFastVLine(xPos*SPACING+7,yPos*SPACING+7,2,borderColour);
    tft.drawFastVLine(xPos*SPACING+7,yPos*SPACING+12,4,borderColour);
    tft.drawFastVLine(xPos*SPACING+6,yPos*SPACING+6,4,borderColour);
    tft.drawFastVLine(xPos*SPACING+5,yPos*SPACING+4,8,borderColour);
    tft.drawFastVLine(xPos*SPACING+4,yPos*SPACING,16,borderColour);
    tft.drawFastVLine(xPos*SPACING+3,yPos*SPACING,16,borderColour);
  }
  else {
    //Top Left Corner
    tft.drawFastVLine(15*SPACING+3,0*SPACING+3,2,borderColour);
    tft.drawFastVLine(15*SPACING+3,0*SPACING+7,2,borderColour);
    tft.drawFastVLine(15*SPACING+3,0*SPACING+12,2,borderColour);
    tft.drawFastVLine(15*SPACING+4,0*SPACING+3,7,borderColour);
    tft.drawFastVLine(15*SPACING+4,0*SPACING+11,4,borderColour);
    tft.drawFastVLine(15*SPACING+5,0*SPACING+4,3,borderColour);
    tft.drawFastVLine(15*SPACING+5,0*SPACING+9,3,borderColour);
    tft.drawFastVLine(15*SPACING+5,0*SPACING+14,2,borderColour);
    tft.drawFastVLine(15*SPACING+6,0*SPACING+4,2,borderColour);
    tft.drawFastVLine(15*SPACING+6,0*SPACING+14,2,borderColour);
    tft.drawFastVLine(15*SPACING+7,0*SPACING+3,2,borderColour);
    tft.drawFastVLine(15*SPACING+7,0*SPACING+7,9,borderColour);
    tft.drawFastVLine(15*SPACING+8,0*SPACING+3,2,borderColour);
    tft.drawFastVLine(15*SPACING+8,0*SPACING+7,9,borderColour);
    tft.drawFastVLine(15*SPACING+9,0*SPACING+4,2,borderColour);
    tft.drawFastVLine(15*SPACING+9,0*SPACING+7,2,borderColour);
    tft.drawPixel(15*SPACING+10,0*SPACING+5,borderColour);
    tft.drawFastVLine(15*SPACING+10,0*SPACING+7,2,borderColour);
    tft.drawFastVLine(15*SPACING+11,0*SPACING+4,2,borderColour);
    tft.drawFastVLine(15*SPACING+11,0*SPACING+7,2,borderColour);
    tft.drawFastVLine(15*SPACING+11,0*SPACING+11,5,borderColour);
    tft.drawFastVLine(15*SPACING+12,0*SPACING+3,2,borderColour);
    tft.drawFastVLine(15*SPACING+12,0*SPACING+7,2,borderColour);
    tft.drawFastVLine(15*SPACING+12,0*SPACING+11,5,borderColour);
    tft.drawFastVLine(15*SPACING+13,0*SPACING+3,2,borderColour);
    tft.drawFastVLine(15*SPACING+13,0*SPACING+7,2,borderColour);
    tft.drawFastVLine(15*SPACING+13,0*SPACING+11,2,borderColour);
    tft.drawFastVLine(15*SPACING+14,0*SPACING+4,5,borderColour);
    tft.drawFastVLine(15*SPACING+14,0*SPACING+11,2,borderColour);
    tft.drawFastVLine(15*SPACING+15,0*SPACING+5,4,borderColour);
    tft.drawFastVLine(15*SPACING+15,0*SPACING+11,2,borderColour);
    //Top Right Corner
    tft.drawFastVLine(19*SPACING+12,0*SPACING+3,2,borderColour);
    tft.drawFastVLine(19*SPACING+12,0*SPACING+7,2,borderColour);
    tft.drawFastVLine(19*SPACING+12,0*SPACING+12,2,borderColour);
    tft.drawFastVLine(19*SPACING+11,0*SPACING+3,7,borderColour);
    tft.drawFastVLine(19*SPACING+11,0*SPACING+11,4,borderColour);
    tft.drawFastVLine(19*SPACING+10,0*SPACING+4,3,borderColour);
    tft.drawFastVLine(19*SPACING+10,0*SPACING+9,3,borderColour);
    tft.drawFastVLine(19*SPACING+10,0*SPACING+14,2,borderColour);
    tft.drawFastVLine(19*SPACING+9,0*SPACING+4,2,borderColour);
    tft.drawFastVLine(19*SPACING+9,0*SPACING+14,2,borderColour);
    tft.drawFastVLine(19*SPACING+8,0*SPACING+3,2,borderColour);
    tft.drawFastVLine(19*SPACING+8,0*SPACING+7,9,borderColour);
    tft.drawFastVLine(19*SPACING+7,0*SPACING+3,2,borderColour);
    tft.drawFastVLine(19*SPACING+7,0*SPACING+7,9,borderColour);
    tft.drawFastVLine(19*SPACING+6,0*SPACING+4,2,borderColour);
    tft.drawFastVLine(19*SPACING+6,0*SPACING+7,2,borderColour);
    tft.drawPixel(19*SPACING+5,0*SPACING+5,borderColour);
    tft.drawFastVLine(19*SPACING+5,0*SPACING+7,2,borderColour);
    tft.drawFastVLine(19*SPACING+4,0*SPACING+4,2,borderColour);
    tft.drawFastVLine(19*SPACING+4,0*SPACING+7,2,borderColour);
    tft.drawFastVLine(19*SPACING+4,0*SPACING+11,5,borderColour);
    tft.drawFastVLine(19*SPACING+3,0*SPACING+3,2,borderColour);
    tft.drawFastVLine(19*SPACING+3,0*SPACING+7,2,borderColour);
    tft.drawFastVLine(19*SPACING+3,0*SPACING+11,5,borderColour);
    tft.drawFastVLine(19*SPACING+2,0*SPACING+3,2,borderColour);
    tft.drawFastVLine(19*SPACING+2,0*SPACING+7,2,borderColour);
    tft.drawFastVLine(19*SPACING+2,0*SPACING+11,2,borderColour);
    tft.drawFastVLine(19*SPACING+1,0*SPACING+4,5,borderColour);
    tft.drawFastVLine(19*SPACING+1,0*SPACING+11,2,borderColour);
    tft.drawFastVLine(19*SPACING,0*SPACING+5,4,borderColour);
    tft.drawFastVLine(19*SPACING,0*SPACING+11,2,borderColour);
    //Bottom Left Corner
    tft.drawFastVLine(15*SPACING+3,14*SPACING+11,2,borderColour);
    tft.drawFastVLine(15*SPACING+3,14*SPACING+7,2,borderColour);
    tft.drawFastVLine(15*SPACING+3,14*SPACING+2,2,borderColour);
    tft.drawFastVLine(15*SPACING+4,14*SPACING+6,7,borderColour);
    tft.drawFastVLine(15*SPACING+4,14*SPACING+1,4,borderColour);
    tft.drawFastVLine(15*SPACING+5,14*SPACING+9,3,borderColour);
    tft.drawFastVLine(15*SPACING+5,14*SPACING+4,3,borderColour);
    tft.drawFastVLine(15*SPACING+5,14*SPACING,2,borderColour);
    tft.drawFastVLine(15*SPACING+6,14*SPACING+10,2,borderColour);
    tft.drawFastVLine(15*SPACING+6,14*SPACING,2,borderColour);
    tft.drawFastVLine(15*SPACING+7,14*SPACING+11,2,borderColour);
    tft.drawFastVLine(15*SPACING+7,14*SPACING,9,borderColour);
    tft.drawFastVLine(15*SPACING+8,14*SPACING+11,2,borderColour);
    tft.drawFastVLine(15*SPACING+8,14*SPACING,9,borderColour);
    tft.drawFastVLine(15*SPACING+9,14*SPACING+10,2,borderColour);
    tft.drawFastVLine(15*SPACING+9,14*SPACING+7,2,borderColour);
    tft.drawPixel(15*SPACING+10,14*SPACING+10,borderColour);
    tft.drawFastVLine(15*SPACING+10,14*SPACING+7,2,borderColour);
    tft.drawFastVLine(15*SPACING+11,14*SPACING+10,2,borderColour);
    tft.drawFastVLine(15*SPACING+11,14*SPACING+7,2,borderColour);
    tft.drawFastVLine(15*SPACING+11,14*SPACING,5,borderColour);
    tft.drawFastVLine(15*SPACING+12,14*SPACING+11,2,borderColour);
    tft.drawFastVLine(15*SPACING+12,14*SPACING+7,2,borderColour);
    tft.drawFastVLine(15*SPACING+12,14*SPACING,5,borderColour);
    tft.drawFastVLine(15*SPACING+13,14*SPACING+11,2,borderColour);
    tft.drawFastVLine(15*SPACING+13,14*SPACING+7,2,borderColour);
    tft.drawFastVLine(15*SPACING+13,14*SPACING+3,2,borderColour);
    tft.drawFastVLine(15*SPACING+14,14*SPACING+7,5,borderColour);
    tft.drawFastVLine(15*SPACING+14,14*SPACING+3,2,borderColour);
    tft.drawFastVLine(15*SPACING+15,14*SPACING+7,4,borderColour);
    tft.drawFastVLine(15*SPACING+15,14*SPACING+3,2,borderColour);
    //Bottom Right Corner
    tft.drawFastVLine(19*SPACING+12,14*SPACING+11,2,borderColour);
    tft.drawFastVLine(19*SPACING+12,14*SPACING+7,2,borderColour);
    tft.drawFastVLine(19*SPACING+12,14*SPACING+2,2,borderColour);
    tft.drawFastVLine(19*SPACING+11,14*SPACING+6,7,borderColour);
    tft.drawFastVLine(19*SPACING+11,14*SPACING+1,4,borderColour);
    tft.drawFastVLine(19*SPACING+10,14*SPACING+9,3,borderColour);
    tft.drawFastVLine(19*SPACING+10,14*SPACING+4,3,borderColour);
    tft.drawFastVLine(19*SPACING+10,14*SPACING,2,borderColour);
    tft.drawFastVLine(19*SPACING+9,14*SPACING+10,2,borderColour);
    tft.drawFastVLine(19*SPACING+9,14*SPACING,2,borderColour);
    tft.drawFastVLine(19*SPACING+8,14*SPACING+11,2,borderColour);
    tft.drawFastVLine(19*SPACING+8,14*SPACING,9,borderColour);
    tft.drawFastVLine(19*SPACING+7,14*SPACING+11,2,borderColour);
    tft.drawFastVLine(19*SPACING+7,14*SPACING,9,borderColour);
    tft.drawFastVLine(19*SPACING+6,14*SPACING+10,2,borderColour);
    tft.drawFastVLine(19*SPACING+6,14*SPACING+7,2,borderColour);
    tft.drawPixel(19*SPACING+5,14*SPACING+10,borderColour);
    tft.drawFastVLine(19*SPACING+5,14*SPACING+7,2,borderColour);
    tft.drawFastVLine(19*SPACING+4,14*SPACING+10,2,borderColour);
    tft.drawFastVLine(19*SPACING+4,14*SPACING+7,2,borderColour);
    tft.drawFastVLine(19*SPACING+4,14*SPACING,5,borderColour);
    tft.drawFastVLine(19*SPACING+3,14*SPACING+11,2,borderColour);
    tft.drawFastVLine(19*SPACING+3,14*SPACING+7,2,borderColour);
    tft.drawFastVLine(19*SPACING+3,14*SPACING,5,borderColour);
    tft.drawFastVLine(19*SPACING+2,14*SPACING+11,2,borderColour);
    tft.drawFastVLine(19*SPACING+2,14*SPACING+7,2,borderColour);
    tft.drawFastVLine(19*SPACING+2,14*SPACING+3,2,borderColour);
    tft.drawFastVLine(19*SPACING+1,14*SPACING+7,5,borderColour);
    tft.drawFastVLine(19*SPACING+1,14*SPACING+3,2,borderColour);
    tft.drawFastVLine(19*SPACING,14*SPACING+7,4,borderColour);
    tft.drawFastVLine(19*SPACING,14*SPACING+3,2,borderColour);
  }
}

// Function to draw many of the tile types in the game?
void drawTile(int xPos,int yPos,int tileType) {
  int lightGrey = 0x52A9;
  int darkGrey = 0x3A07;
  int red = 0xD041; 
  int hotPink = 0xC808;
  int orange = 0xFD25;
  int yellow = 0xFEC5;
  // Floor
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
  // Wood Planks
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
  // Boss Floor
  else if(tileType == 2){
    int outC = 0x8A81;
    int inC = 0x3185;
    tft.fillRect(xPos,yPos,SPACING,SPACING,outC);//Outer bits
    tft.fillRect(xPos+1,yPos+1,4,4,inC);//Inner stones
    tft.fillRect(xPos+7,yPos+1,3,3,inC);
    tft.fillRect(xPos+12,yPos+1,3,3,inC);
    tft.fillRect(xPos+1,yPos+7,6,3,inC);
    tft.fillRect(xPos+9,yPos+6,6,3,inC);
    tft.fillRect(xPos+1,yPos+12,8,3,inC);
    tft.fillRect(xPos+11,yPos+11,4,4,inC);
  }
  // Acid Traps
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
  // Boss lava
  else if(tileType == 4){
    int outC = 0x71E1;
    int inC = 0x39C6;
    tft.fillRect(xPos,yPos,SPACING,SPACING,outC);//Outer bits
    tft.fillRect(xPos+1,yPos+1,4,4,inC);//Inner stones
    tft.fillRect(xPos+7,yPos+1,3,3,inC);
    tft.fillRect(xPos+12,yPos+1,3,3,inC);
    tft.fillRect(xPos+1,yPos+7,6,3,inC);
    tft.fillRect(xPos+9,yPos+6,6,3,inC);
    tft.fillRect(xPos+1,yPos+12,8,3,inC);
    tft.fillRect(xPos+11,yPos+11,4,4,inC);
  }
  // Heart
  else if(tileType == 5){
    if(Characters[0].row == 1 && Characters[0].col ==1)
      drawTile(xPos,yPos,1);
    else
      drawTile(xPos,yPos,0);
    int heartGold = 0xED46;
    int heartRed = 0xD000;
    // Gold
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
    // Red
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
  else if (tileType == 7) {
    // ?
    if (wCol == 0 && wRow == 1) {
      drawTile(xPos,yPos,1);
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
    // ?
    if (wCol == 2 && wRow == 1) {
      drawTile(xPos, yPos, 0);
      tft.drawLine(xPos + 1, yPos + 11, xPos + 11, yPos + 1, 0xFFFF);
      tft.drawLine(xPos + 6, yPos + 14,xPos + 14, yPos + 6, 0xC440);
      tft.drawLine(xPos + 7, yPos + 14,xPos + 14, yPos + 7, 0xC440);
      tft.drawLine(xPos, yPos + 12, xPos + 3, yPos + 14, 0xA513);
      tft.drawLine(xPos + 12, yPos,xPos + 14, yPos + 3, 0xA513);
      tft.drawFastHLine(xPos + 4, yPos + 14, 2, 0xA513);
      tft.drawFastVLine(xPos + 14, yPos + 4, 2, 0xA513);
    }
    if((wCol == 0 || wCol == 2) && wRow == 0) {
      int heartGold = 0x8E38; // Actually silver
      int heartRed = 0xD000;
      // Silver
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
      // Red
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
  }
  // Walls
  else if((tileType >= 10 && tileType <= 13)) { // We only need 10 now, I think?
    tft.fillRect(xPos,yPos,SPACING,SPACING,0x5AEB);//Darker
    tft.fillRect(xPos+1,yPos+1,4,4,0x8430);//Lighter
    tft.fillRect(xPos+7,yPos+1,3,3,0x8430);
    tft.fillRect(xPos+12,yPos+1,3,3,0x8430);
    tft.fillRect(xPos+1,yPos+7,6,3,0x8430);
    tft.fillRect(xPos+9,yPos+6,6,3,0x8430);
    tft.fillRect(xPos+1,yPos+12,8,3,0x8430);
    tft.fillRect(xPos+11,yPos+11,4,4,0x8430);
  }
  // Water
  else if(tileType ==14){
    int waterColour = 0x1412;
    tft.fillRect(xPos,yPos,SPACING,SPACING,0x02D4); //?
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
  // Lava
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
  // Boss Almost Lava
  else if(tileType == 16 || tileType == 50){
    int outC = 0xD3C1;
    int inC = 0x6120;
    tft.fillRect(xPos,yPos,SPACING,SPACING,outC);//Outer bits
    tft.fillRect(xPos+1,yPos+1,4,4,inC);//Inner stones
    tft.fillRect(xPos+7,yPos+1,3,3,inC);
    tft.fillRect(xPos+12,yPos+1,3,3,inC);
    tft.fillRect(xPos+1,yPos+7,6,3,inC);
    tft.fillRect(xPos+9,yPos+6,6,3,inC);
    tft.fillRect(xPos+1,yPos+12,8,3,inC);
    tft.fillRect(xPos+11,yPos+11,4,4,inC);
  }
  // Empty Heart
  else if(tileType == 30){
    tft.fillRect(xPos,yPos,SPACING,SPACING,0x0000);
  }
  // One-Quarter Heart
  else if(tileType == 31){
    int heartRed = 0xD000;
    tft.drawFastVLine(xPos+2,yPos+4,4,heartRed);
    tft.drawFastVLine(xPos+3,yPos+3,5,heartRed);
    tft.drawFastVLine(xPos+4,yPos+3,5,heartRed);
    tft.drawFastVLine(xPos+5,yPos+3,5,heartRed);
    tft.drawFastVLine(xPos+6,yPos+4,4,heartRed);
    tft.drawFastVLine(xPos+7,yPos+5,3,heartRed);
  }
  // Half Heart
  else if(tileType == 32){
    int heartRed = 0xD000;
    tft.drawFastVLine(xPos+2,yPos+4,4,heartRed);
    tft.drawFastVLine(xPos+3,yPos+3,6,heartRed);
    tft.drawFastVLine(xPos+4,yPos+3,7,heartRed);
    tft.drawFastVLine(xPos+5,yPos+3,8,heartRed);
    tft.drawFastVLine(xPos+6,yPos+4,8,heartRed);
    tft.drawFastVLine(xPos+7,yPos+5,8,heartRed);
  }
  // Three-Quarter Heart
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
  // Full Heart
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
  // ?
  else if(tileType == 60){
    drawTile(xPos,yPos,10);
    // Block 1/9
    // Light Grey
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
    // Dark Grey
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
  // Block 2/9
  else if(tileType == 61){
    drawTile(xPos,yPos,10);
    // Light Grey Crown
    tft.drawFastVLine(xPos+5,yPos+7,2,lightGrey);
    tft.drawFastVLine(xPos+6,yPos+5,6,lightGrey);
    tft.drawFastVLine(xPos+7,yPos+4,7,lightGrey);
    tft.drawFastVLine(xPos+8,yPos+4,7,lightGrey);
    tft.drawFastVLine(xPos+9,yPos+5,6,lightGrey);
    tft.drawFastVLine(xPos+10,yPos+7,2,lightGrey);
    // Horns
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
    // Dark Grey Crown and Horns
    tft.fillRect(xPos+7,yPos+6,2,3,darkGrey);
    tft.drawFastVLine(xPos,yPos+10,5,darkGrey);
    tft.drawFastVLine(xPos+15,yPos+10,5,darkGrey);
    tft.drawFastVLine(xPos+1,yPos+11,4,darkGrey);
    tft.drawFastVLine(xPos+14,yPos+11,4,darkGrey);
    tft.drawFastVLine(xPos+2,yPos+12,2,darkGrey);
    tft.drawFastVLine(xPos+13,yPos+12,2,darkGrey);
    tft.drawFastHLine(xPos+3,yPos+12,3,darkGrey);
    tft.drawFastHLine(xPos+10,yPos+12,3,darkGrey);
    // Red
    tft.drawFastHLine(xPos+4,yPos+10,3,red);
    tft.drawFastHLine(xPos+9,yPos+10,3,red);
    tft.drawPixel(xPos+5,yPos+9,red);
    tft.drawPixel(xPos+10,yPos+9,red);
    // Hot Pink
    tft.fillRect(xPos+7,yPos+11,2,2,hotPink);
    // Yellow
    tft.fillRect(xPos+7,yPos+13,2,3,yellow);
    tft.drawFastHLine(xPos+5,yPos+15,6,yellow);
    // Orange
    tft.drawFastHLine(xPos+4,yPos+14,2,orange);
    tft.drawFastHLine(xPos+10,yPos+14,2,orange);
    tft.drawFastHLine(xPos+3,yPos+15,2,orange);
    tft.drawFastHLine(xPos+11,yPos+15,2,orange);
  }
  // Block 3/9
  else if(tileType == 62){
    drawTile(xPos,yPos,10);
    // Light Grey
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
    // Dark Grey
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
  // Block 4/9
  else if(tileType == 63){
    drawTile(xPos,yPos,16);
    // Light Grey
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
    // Dark Grey
    tft.drawFastHLine(xPos+6,yPos+5,1,darkGrey);
    tft.drawFastHLine(xPos+5,yPos+6,3,darkGrey);
    tft.drawFastHLine(xPos+4,yPos+7,6,darkGrey);
    tft.drawFastHLine(xPos+4,yPos+8,8,darkGrey);
    tft.drawFastHLine(xPos+3,yPos+9,9,darkGrey);
    tft.drawFastHLine(xPos+3,yPos+10,10,darkGrey);
    tft.drawFastHLine(xPos+7,yPos+11,6,darkGrey);
    // Orange
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
    // Yellow
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
    // Red
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
  // Block 5/9
  else if(tileType == 64){
    drawTile(xPos,yPos,16);
    // Light Grey
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
    // Dark Grey
    tft.drawFastHLine(xPos+6,yPos+6,4,darkGrey);
    tft.drawFastHLine(xPos+4,yPos+7,8,darkGrey);
    tft.drawFastHLine(xPos+5,yPos+8,6,darkGrey);
    tft.drawFastHLine(xPos+7,yPos+9,2,darkGrey);
    tft.drawFastHLine(xPos+6,yPos+14,4,darkGrey);
    tft.drawFastHLine(xPos+5,yPos+15,6,darkGrey);
    // Orange
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
    // Yellow
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
    // Red
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
    // Hot Pink
    tft.drawFastVLine(xPos,yPos+11,2,hotPink);
    tft.drawFastVLine(xPos+15,yPos+11,2,hotPink);
  }
  // Block 6/9
  else if(tileType == 65){
    drawTile(xPos,yPos,16);
    // Light Grey
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
    // Dark Grey
    tft.drawFastHLine(xPos+9,yPos+5,1,darkGrey);
    tft.drawFastHLine(xPos+8,yPos+6,3,darkGrey);
    tft.drawFastHLine(xPos+6,yPos+7,6,darkGrey);
    tft.drawFastHLine(xPos+4,yPos+8,8,darkGrey);
    tft.drawFastHLine(xPos+4,yPos+9,9,darkGrey);
    tft.drawFastHLine(xPos+3,yPos+10,10,darkGrey);
    tft.drawFastHLine(xPos+4,yPos+11,6,darkGrey);
    // Orange
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
    // Yellow
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
    // Red
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
  // Block 7/9
  else if(tileType == 66){
    drawTile(xPos,yPos,2);
    // Light Grey
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
    // Dark Grey
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
    // Orange
    tft.drawFastHLine(xPos+1,yPos,3,orange);
    tft.drawFastHLine(xPos+3,yPos+1,9,orange);
    tft.drawFastVLine(xPos+11,yPos+1,3,orange);
    tft.drawFastVLine(xPos+15,yPos+3,2,orange);
    tft.drawFastHLine(xPos+12,yPos+10,2,orange);
    tft.drawFastHLine(xPos+13,yPos+11,3,orange);
    tft.drawPixel(xPos+15,yPos+12,orange);
    // Yellow
    tft.drawFastHLine(xPos+4,yPos,8,yellow);
    tft.drawFastVLine(xPos+12,yPos+1,3,yellow);
    tft.drawPixel(xPos+11,yPos+4,yellow);
    tft.drawFastVLine(xPos+15,yPos,3,yellow);
    tft.drawFastVLine(xPos+14,yPos+3,3,yellow);
    tft.drawPixel(xPos+15,yPos+5,yellow);
    tft.drawFastHLine(xPos+12,yPos+9,2,yellow);
    tft.drawFastHLine(xPos+14,yPos+10,2,yellow);
    // Red
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
    // Hot Pink
    tft.drawFastVLine(xPos+13,yPos,3,hotPink);
    tft.drawFastVLine(xPos+12,yPos+4,2,hotPink);
    tft.drawFastVLine(xPos+13,yPos+6,2,hotPink);
    tft.drawPixel(xPos+14,yPos+7,hotPink);
    tft.drawPixel(xPos+15,yPos+8,hotPink);
  }
  // Block 8/9
  else if(tileType == 67){
    drawTile(xPos,yPos,2);
    // Light Grey
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
    // Dark Grey
    tft.drawFastHLine(xPos+4,yPos,8,darkGrey);
    tft.drawFastHLine(xPos+3,yPos+1,10,darkGrey);
    tft.drawFastHLine(xPos+2,yPos+2,12,darkGrey);
    tft.drawFastHLine(xPos+2,yPos+3,12,darkGrey);
    tft.drawFastHLine(xPos+5,yPos+4,6,darkGrey);
    tft.drawFastHLine(xPos+6,yPos+5,4,darkGrey);
    tft.drawFastHLine(xPos+7,yPos+6,2,darkGrey);
    tft.drawFastVLine(xPos,yPos+13,3,darkGrey);
    tft.drawFastVLine(xPos+15,yPos+13,3,darkGrey);
    // Orange
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
    // Yellow
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
    // Red
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
    // Hot Pink
    tft.drawFastHLine(xPos,yPos+8,2,hotPink);
    tft.drawFastHLine(xPos+14,yPos+8,2,hotPink);
    tft.drawFastHLine(xPos+1,yPos+9,3,hotPink);
    tft.drawFastHLine(xPos+12,yPos+9,3,hotPink);
  }
  else if(tileType == 68){
    drawTile(xPos,yPos,2);
    // Light Grey
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
    // Dark Grey
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
    // Orange
    tft.drawFastHLine(xPos+12,yPos,3,orange);
    tft.drawFastHLine(xPos+4,yPos+1,9,orange);
    tft.drawFastVLine(xPos+4,yPos+1,3,orange);
    tft.drawFastVLine(xPos,yPos+3,2,orange);
    tft.drawFastHLine(xPos+2,yPos+10,2,orange);
    tft.drawFastHLine(xPos,yPos+11,3,orange);
    tft.drawPixel(xPos,yPos+12,orange);
    // Yellow
    tft.drawFastHLine(xPos+4,yPos,8,yellow);
    tft.drawFastVLine(xPos+3,yPos+1,3,yellow);
    tft.drawPixel(xPos+4,yPos+4,yellow);
    tft.drawFastVLine(xPos,yPos,3,yellow);
    tft.drawFastVLine(xPos+1,yPos+3,3,yellow);
    tft.drawPixel(xPos,yPos+5,yellow);
    tft.drawFastHLine(xPos+2,yPos+9,2,yellow);
    tft.drawFastHLine(xPos,yPos+10,2,yellow);
    // Red
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
    // Hot Pink
    tft.drawFastVLine(xPos+2,yPos,3,hotPink);
    tft.drawFastVLine(xPos+3,yPos+4,2,hotPink);
    tft.drawFastVLine(xPos+2,yPos+6,2,hotPink);
    tft.drawPixel(xPos+1,yPos+7,hotPink);
    tft.drawPixel(xPos,yPos+8,hotPink);
  }
}

// Function to draw enemies?
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

  // Enemy Facing Left
  if(Characters[i].cDirection == 0){
    // Enemy Horns
    tft.drawFastVLine(x*SPACING+2,y*SPACING,3,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+3,y*SPACING+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+4,y*SPACING+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+13,y*SPACING+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+14,y*SPACING+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+15,y*SPACING,3,ILI9340_WHITE);
    // Enemy Head
    tft.fillRect(x*SPACING+6,y*SPACING,6,7,enemyColour);
    tft.drawFastVLine(x*SPACING+12,y*SPACING+1,6,enemyColour);
    tft.drawFastVLine(x*SPACING+13,y*SPACING+2,4,enemyColour);
    tft.drawFastVLine(x*SPACING+5,y*SPACING+1,3,enemyColour);
    tft.drawFastVLine(x*SPACING+4,y*SPACING+2,2,enemyColour);
    tft.drawFastHLine(x*SPACING+6,y*SPACING+7,5,enemyColour);
    tft.drawPixel(x*SPACING+5,y*SPACING+6,enemyColour);
    // Enemy Body
    tft.fillRect(x*SPACING+1,y*SPACING+8,13,2,enemyColour);
    tft.fillRect(x*SPACING+13,y*SPACING+9,2,3,enemyColour);
    tft.fillRect(x*SPACING+6,y*SPACING+12,6,2,enemyColour);
    tft.fillRect(x*SPACING+5,y*SPACING+13,2,2,enemyColour);
    tft.fillRect(x*SPACING+4,y*SPACING+14,2,2,enemyColour);
    tft.fillRect(x*SPACING+11,y*SPACING+13,2,2,enemyColour);
    tft.fillRect(x*SPACING+12,y*SPACING+14,2,2,enemyColour);
    // Draw Eyes
    tft.fillRect(x*SPACING+4,y*SPACING+4,2,2,0xDDE7);
    tft.fillRect(x*SPACING+8,y*SPACING+4,2,2,0xDDE7);
    // Draw Belt
    tft.fillRect(x*SPACING+6,y*SPACING+10,6,2,0xCC61);
    tft.fillRect(x*SPACING+7,y*SPACING+10,2,2,0xCE58);
    // Draw Sword
    tft.fillRect(x*SPACING+1,y*SPACING+10,2,2,0x82C0);
    tft.drawFastVLine(x*SPACING+1,y*SPACING+4,4,0xB555);
    tft.drawFastVLine(x*SPACING+2,y*SPACING+5,3,0xB555);
    if(Characters[i].type == 2 || Characters[i].type == 4){
      // Draw Sword Extension
      tft.drawFastVLine(x*SPACING,y*SPACING,5,0xB555);
      // Draw Shield
      tft.fillRect(x*SPACING+12,y*SPACING+7,3,6,0xB555);
      tft.drawFastVLine(x*SPACING+13,y*SPACING+8,4,0x6B6D);
      // Draw Helmet
      tft.fillRect(x*SPACING+6,y*SPACING,5,8,helmetColour);
      tft.drawFastVLine(x*SPACING+4,y*SPACING+2,4,helmetColour);
      tft.drawFastVLine(x*SPACING+5,y*SPACING+1,6,helmetColour);
      tft.drawFastVLine(x*SPACING+11,y*SPACING,7,helmetColour);
      tft.drawFastVLine(x*SPACING+12,y*SPACING+1,6,helmetColour);
      tft.drawFastVLine(x*SPACING+13,y*SPACING+3,4,helmetColour);
      tft.fillRect(x*SPACING+5,y*SPACING+4,6,2,0x0000);
      tft.fillRect(x*SPACING+7,y*SPACING+6,2,2,0x0000);
    }
  }
  // Enemy Facing Up
  else if(Characters[i].cDirection == 1){
    // Enemy Horns
    tft.drawFastVLine(x*SPACING+1,y*SPACING,3,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+2,y*SPACING+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+3,y*SPACING+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+12,y*SPACING+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+13,y*SPACING+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+14,y*SPACING,3,ILI9340_WHITE);
    // Enemy Head
    tft.fillRect(x*SPACING+5,y*SPACING,6,7,enemyColour);
    tft.drawFastVLine(x*SPACING+4,y*SPACING+1,6,enemyColour);
    tft.drawFastVLine(x*SPACING+3,y*SPACING+2,4,enemyColour);
    tft.drawFastVLine(x*SPACING+11,y*SPACING+1,6,enemyColour);
    tft.drawFastVLine(x*SPACING+12,y*SPACING+2,4,enemyColour);
    tft.drawFastHLine(x*SPACING+6,y*SPACING+7,4,enemyColour);
    // Enemy Body
    tft.fillRect(x*SPACING+3,y*SPACING+8,10,2,enemyColour);
    tft.fillRect(x*SPACING+1,y*SPACING+9,2,2,enemyColour);
    tft.fillRect(x*SPACING+12,y*SPACING+9,2,2,enemyColour);
    tft.drawPixel(x*SPACING+13,y*SPACING+11,enemyColour);
    tft.fillRect(x*SPACING+5,y*SPACING+12,6,2,enemyColour);
    tft.fillRect(x*SPACING+4,y*SPACING+13,2,2,enemyColour);
    tft.fillRect(x*SPACING+3,y*SPACING+14,2,2,enemyColour);
    tft.fillRect(x*SPACING+10,y*SPACING+13,2,2,enemyColour);
    tft.fillRect(x*SPACING+11,y*SPACING+14,2,2,enemyColour);
    // Draw Belt
    tft.fillRect(x*SPACING+5,y*SPACING+10,6,2,0xCC61);
    // Draw sword
    tft.fillRect(x*SPACING+1,y*SPACING+11,2,2,0x82C0);
    tft.drawFastVLine(x*SPACING+1,y*SPACING+5,4,0xB555);
    tft.drawFastVLine(x*SPACING+2,y*SPACING+6,3,0xB555);
    if(Characters[i].type == 2 || Characters[i].type == 4){
      // Draw Sword Extension
      tft.drawFastVLine(x*SPACING+1,y*SPACING+3,2,0xB555);
      tft.drawFastVLine(x*SPACING+2,y*SPACING+4,2,0xB555);
      // Draw Shield
      tft.drawFastHLine(x*SPACING+12,y*SPACING+7,3,0xB555);
      tft.drawFastHLine(x*SPACING+13,y*SPACING+8,2,0xB555);
      tft.drawFastVLine(x*SPACING+14,y*SPACING+9,3,0xB555);
      tft.drawFastHLine(x*SPACING+12,y*SPACING+12,3,0xB555);
      tft.drawPixel(x*SPACING+12,y*SPACING+11,0xB555);
      // Draw Helmet
      tft.fillRect(x*SPACING+6,y*SPACING,4,8,helmetColour);
      tft.drawFastVLine(x*SPACING+3,y*SPACING+2,4,helmetColour);
      tft.drawFastVLine(x*SPACING+4,y*SPACING+1,6,helmetColour);
      tft.drawFastVLine(x*SPACING+5,y*SPACING,7,helmetColour);
      tft.drawFastVLine(x*SPACING+10,y*SPACING,7,helmetColour);
      tft.drawFastVLine(x*SPACING+11,y*SPACING+1,6,helmetColour);
      tft.drawFastVLine(x*SPACING+12,y*SPACING+2,4,helmetColour);
    }
  }
  // Enemy Facing Right
  else if(Characters[i].cDirection == 2){
    // Enemy Horns
    tft.drawFastVLine(x*SPACING,y*SPACING,3,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+1,y*SPACING+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+2,y*SPACING+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+11,y*SPACING+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+12,y*SPACING+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+13,y*SPACING,3,ILI9340_WHITE);
    // Enemy Head
    tft.fillRect(x*SPACING+4,y*SPACING,6,7,enemyColour);
    tft.drawFastVLine(x*SPACING+3,y*SPACING+1,6,enemyColour);
    tft.drawFastVLine(x*SPACING+2,y*SPACING+2,4,enemyColour);
    tft.drawFastVLine(x*SPACING+10,y*SPACING+1,3,enemyColour);
    tft.drawFastVLine(x*SPACING+11,y*SPACING+2,2,enemyColour);
    tft.drawFastHLine(x*SPACING+5,y*SPACING+7,5,enemyColour);
    tft.drawPixel(x*SPACING+10,y*SPACING+6,enemyColour);
    // Enemy Body
    tft.fillRect(x*SPACING+2,y*SPACING+8,13,2,enemyColour);
    tft.fillRect(x*SPACING+1,y*SPACING+9,2,3,enemyColour);
    tft.fillRect(x*SPACING+4,y*SPACING+12,6,2,enemyColour);
    tft.fillRect(x*SPACING+3,y*SPACING+13,2,2,enemyColour);
    tft.fillRect(x*SPACING+2,y*SPACING+14,2,2,enemyColour);
    tft.fillRect(x*SPACING+9,y*SPACING+13,2,2,enemyColour);
    tft.fillRect(x*SPACING+10,y*SPACING+14,2,2,enemyColour);
    // Draw Eyes
    tft.fillRect(x*SPACING+6,y*SPACING+4,2,2,0xDDE7);
    tft.fillRect(x*SPACING+10,y*SPACING+4,2,2,0xDDE7);
    // Draw Belt
    tft.fillRect(x*SPACING+4,y*SPACING+10,6,2,0xCC61);
    tft.fillRect(x*SPACING+7,y*SPACING+10,2,2,0xCE58);
    // Draw sword
    tft.fillRect(x*SPACING+13,y*SPACING+10,2,2,0x82C0);
    tft.drawFastVLine(x*SPACING+13,y*SPACING+5,3,0xB555);
    tft.drawFastVLine(x*SPACING+14,y*SPACING+4,4,0xB555);
    if(Characters[i].type == 2 || Characters[i].type == 4){
      // Draw Sword Extension
      tft.drawFastVLine(x*SPACING+15,y*SPACING,6,0xB555);
      // Draw Shield
      tft.fillRect(x*SPACING+1,y*SPACING+7,3,6,0xB555);
      tft.drawFastVLine(x*SPACING+2,y*SPACING+8,4,0x6B6D);
      // Draw Helmet
      tft.fillRect(x*SPACING+5,y*SPACING,5,8,helmetColour);
      tft.drawFastVLine(x*SPACING+2,y*SPACING+2,4,helmetColour);
      tft.drawFastVLine(x*SPACING+3,y*SPACING+1,6,helmetColour);
      tft.drawFastVLine(x*SPACING+4,y*SPACING,7,helmetColour);
      tft.drawFastVLine(x*SPACING+10,y*SPACING+1,6,helmetColour);
      tft.drawFastVLine(x*SPACING+11,y*SPACING+2,4,helmetColour);
      tft.fillRect(x*SPACING+5,y*SPACING+4,6,2,0x0000);
      tft.fillRect(x*SPACING+7,y*SPACING+6,2,2,0x0000);
    }
  }
  // Enemy Facing Down
  else{
      // Enemy Horns
    tft.drawFastVLine(x*SPACING+1,y*SPACING,3,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+2,y*SPACING+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+3,y*SPACING+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+12,y*SPACING+1,1,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+13,y*SPACING+1,3,ILI9340_WHITE);
    tft.drawFastVLine(x*SPACING+14,y*SPACING,3,ILI9340_WHITE);
    // Enemy Head
    tft.fillRect(x*SPACING+5,y*SPACING,6,7,enemyColour);
    tft.drawFastVLine(x*SPACING+4,y*SPACING+1,6,enemyColour);
    tft.drawFastVLine(x*SPACING+3,y*SPACING+2,4,enemyColour);
    tft.drawFastVLine(x*SPACING+11,y*SPACING+1,6,enemyColour);
    tft.drawFastVLine(x*SPACING+12,y*SPACING+2,4,enemyColour);
    tft.drawFastHLine(x*SPACING+6,y*SPACING+7,4,enemyColour);
    // Draw Eyes
    tft.fillRect(x*SPACING+5,y*SPACING+4,2,2,0xDDE7);
    tft.fillRect(x*SPACING+9,y*SPACING+4,2,2,0xDDE7);
    // Enemy Body
    tft.fillRect(x*SPACING+3,y*SPACING+8,10,2,enemyColour);
    tft.fillRect(x*SPACING+1,y*SPACING+9,2,2,enemyColour);
    tft.fillRect(x*SPACING+12,y*SPACING+9,2,2,enemyColour);
    tft.drawPixel(x*SPACING+13,y*SPACING+11,enemyColour);
    tft.fillRect(x*SPACING+5,y*SPACING+12,6,2,enemyColour);
    tft.fillRect(x*SPACING+4,y*SPACING+13,2,2,enemyColour);
    tft.fillRect(x*SPACING+3,y*SPACING+14,2,2,enemyColour);
    tft.fillRect(x*SPACING+10,y*SPACING+13,2,2,enemyColour);
    tft.fillRect(x*SPACING+11,y*SPACING+14,2,2,enemyColour);
    // Draw Belt
    tft.fillRect(x*SPACING+5,y*SPACING+10,6,2,0xCC61);
    tft.fillRect(x*SPACING+7,y*SPACING+10,2,2,0xCE58);
    // Draw sword
    tft.fillRect(x*SPACING+13,y*SPACING+11,2,2,0x82C0);
    tft.drawFastVLine(x*SPACING+13,y*SPACING+6,3,0xB555);
    tft.drawFastVLine(x*SPACING+14,y*SPACING+5,4,0xB555);
    if(Characters[i].type == 2 || Characters[i].type == 4){
      // Draw Sword Extension
      tft.drawFastVLine(x*SPACING+13,y*SPACING+1,5,0xB555);
      tft.drawFastVLine(x*SPACING+14,y*SPACING+1,4,0xB555);
      // Draw Shield
      tft.fillRect(x*SPACING+1,y*SPACING+7,3,6,0xB555);
      tft.drawFastVLine(x*SPACING+2,y*SPACING+8,4,0x6B6D);
      // Draw Helmet
      tft.fillRect(x*SPACING+6,y*SPACING,4,8,helmetColour);
      tft.drawFastVLine(x*SPACING+3,y*SPACING+2,4,helmetColour);
      tft.drawFastVLine(x*SPACING+4,y*SPACING+1,6,helmetColour);
      tft.drawFastVLine(x*SPACING+5,y*SPACING,7,helmetColour);
      tft.drawFastVLine(x*SPACING+10,y*SPACING,7,helmetColour);
      tft.drawFastVLine(x*SPACING+11,y*SPACING+1,6,helmetColour);
      tft.drawFastVLine(x*SPACING+12,y*SPACING+2,4,helmetColour);
      tft.fillRect(x*SPACING+5,y*SPACING+4,6,2,0x0000);
      tft.fillRect(x*SPACING+7,y*SPACING+6,2,2,0x0000);
    }
  }
}

void drawHero(int x,int y) {
  drawTile(x*SPACING,y*SPACING,Characters[0].cTile);
  // Hero facing left
  if(Characters[0].cDirection == 0) {
    // Draw Hat
    tft.drawFastHLine(x*SPACING+5,y*SPACING,11,0x1CB0);
    tft.drawFastHLine(x*SPACING+4,y*SPACING+1,11,0x1CB0);
    tft.drawFastHLine(x*SPACING+3,y*SPACING+2,11,0x1CB0);
    tft.drawFastHLine(x*SPACING+10,y*SPACING+3,4,0x1CB0);
    tft.drawFastHLine(x*SPACING+11,y*SPACING+4,3,0x1CB0);
    tft.drawFastHLine(x*SPACING+12,y*SPACING+5,2,0x1CB0);
    // Draw Eyes
    tft.fillRect(x*SPACING+4,y*SPACING+4,2,2,0x0000);
    tft.fillRect(x*SPACING+8,y*SPACING+4,2,2,0x0000);
    // Draw Skin
    tft.drawFastHLine(x*SPACING+3,y*SPACING+3,7,0xEEEF);
    tft.drawFastVLine(x*SPACING+10,y*SPACING+4,2,0xEEEF);
    tft.drawFastVLine(x*SPACING+3,y*SPACING+4,3,0xEEEF);
    tft.fillRect(x*SPACING+6,y*SPACING+4,2,2,0xEEEF);
    tft.fillRect(x*SPACING+4,y*SPACING+6,10,2,0xEEEF);
    tft.drawPixel(x*SPACING+11,y*SPACING+5,0xEEEF);
    tft.fillRect(x*SPACING,y*SPACING+8,2,2,0xEEEF);
    tft.fillRect(x*SPACING+14,y*SPACING+10,2,2,0xEEEF);
    // Draw Tunic
    tft.fillRect(x*SPACING+2,y*SPACING+8,14,2,0x1CB0);
    tft.fillRect(x*SPACING+4,y*SPACING+12,10,2,0x1CB0);
    // Draw Belt
    tft.fillRect(x*SPACING+4,y*SPACING+10,10,2,0xDD00);
    tft.fillRect(x*SPACING+6,y*SPACING+10,2,2,ILI9340_YELLOW);
    // Draw Boots
    tft.drawFastHLine(x*SPACING+4,y*SPACING+14,2,0xA3C0);
    tft.drawFastHLine(x*SPACING+3,y*SPACING+15,3,0xA3C0);
    tft.drawFastHLine(x*SPACING+12,y*SPACING+14,2,0xA3C0);
    tft.drawFastHLine(x*SPACING+11,y*SPACING+15,3,0xA3C0);
    if(weapon == 0){
      // Draw Sword
      tft.drawFastHLine(x*SPACING,y*SPACING+7,2,0xA000);
      tft.fillRect(x*SPACING,y*SPACING+10,2,2,0xA000);
      tft.fillRect(x*SPACING,y*SPACING+2,2,7,0xDEDB);
    }
    else if (weapon == 1) {
      // Draw Spear
      tft.drawFastVLine(x*SPACING+1,y*SPACING,2,0x9DB7);
      tft.drawFastVLine(x*SPACING+2,y*SPACING+1,5,0x9DB7);
      tft.drawFastVLine(x*SPACING+1,y*SPACING+5,2,0x9DB7);
      tft.drawFastVLine(x*SPACING,y*SPACING+3,5,0x9DB7);
      tft.drawFastVLine(x*SPACING,y*SPACING+10,4,0x9DB7);
    }
    else if(weapon == 2){
      // Draw Bow
      tft.fillRect(x*SPACING,y*SPACING+6,2,2,0xC440);
      tft.fillRect(x*SPACING,y*SPACING+10,2,2,0xC440);
      tft.drawFastHLine(x*SPACING,y*SPACING+5,2,0xA513);
      tft.drawFastHLine(x*SPACING+1,y*SPACING+4,2,0xA513);
      tft.drawFastHLine(x*SPACING+2,y*SPACING+3,2,0xA513);
      tft.drawFastHLine(x*SPACING,y*SPACING+12,2,0xA513);
      tft.drawFastHLine(x*SPACING+1,y*SPACING+13,2,0xA513);
      tft.drawFastHLine(x*SPACING+2,y*SPACING+14,2,0xA513);
      tft.drawFastVLine(x*SPACING+3,y*SPACING+4,10,0xFFFF);
    }
  }
  // Hero facing up
  else if(Characters[0].cDirection == 1) {
    tft.drawFastHLine(x*SPACING+5,y*SPACING,6,0x1CB0);
    tft.drawFastHLine(x*SPACING+4,y*SPACING+1,8,0x1CB0);
    tft.fillRect(x*SPACING+3,y*SPACING+2,10,4,0x1CB0);
    tft.drawFastHLine(x*SPACING+6,y*SPACING+6,4,0x1CB0);
    tft.drawFastHLine(x*SPACING+7,y*SPACING+7,2,0x1CB0);
    // Draw Skin
    tft.drawFastHLine(x*SPACING+4,y*SPACING+6,2,0xEEEF);
    tft.drawFastHLine(x*SPACING+4,y*SPACING+7,3,0xEEEF);
    tft.drawFastHLine(x*SPACING+10,y*SPACING+6,2,0xEEEF);
    tft.drawFastHLine(x*SPACING+9,y*SPACING+7,3,0xEEEF);
    tft.drawFastVLine(x*SPACING+1,y*SPACING+9,2,0xEEEF);
    tft.fillRect(x*SPACING+12,y*SPACING+10,2,2,0xEEEF);
    // Draw Tunic
    tft.fillRect(x*SPACING+3,y*SPACING+8,11,2,0x1CB0);
    tft.fillRect(x*SPACING+2,y*SPACING+9,2,2,0x1CB0);
    tft.fillRect(x*SPACING+4,y*SPACING+12,8,2,0x1CB0);
    // Draw Belt
    tft.fillRect(x*SPACING+4,y*SPACING+10,8,2,0xDD00);
    // Draw Boots
    tft.fillRect(x*SPACING+4,y*SPACING+14,2,2,0xA3C0);
    tft.fillRect(x*SPACING+10,y*SPACING+14,2,2,0xA3C0);
    if(weapon == 0){
      // Draw Sword
      tft.drawFastHLine(x*SPACING+1,y*SPACING+8,2,0xA000);
      tft.fillRect(x*SPACING+1,y*SPACING+11,2,2,0xA000);
      tft.fillRect(x*SPACING+1,y*SPACING+1,2,7,0xDEDB);
    }
    else if (weapon == 1) {
      // Draw Spear
      tft.drawFastVLine(x*SPACING+1,y*SPACING+1,2,0x9DB7);
      tft.drawPixel(x*SPACING+3,y*SPACING+1,0x9DB7);
      tft.drawFastVLine(x*SPACING,y*SPACING+2,5,0x9DB7);
      tft.drawFastVLine(x*SPACING+1,y*SPACING+6,2,0x9DB7);
      tft.drawFastVLine(x*SPACING+2,y*SPACING+4,5,0x9DB7);
      tft.drawFastVLine(x*SPACING+3,y*SPACING+6,2,0x9DB7);
      tft.drawFastVLine(x*SPACING+2,y*SPACING+11,4,0x9DB7);
    }
    else if(weapon == 2) {
      // Draw Bow
      tft.drawFastVLine(x*SPACING+1,y*SPACING+7,2,0xC440);
      tft.drawFastVLine(x*SPACING+1,y*SPACING+11,2,0xC440);
      tft.drawFastHLine(x*SPACING+1,y*SPACING+4,2,0xA513);
      tft.drawFastVLine(x*SPACING+1,y*SPACING+5,2,0xA513);
      tft.drawFastVLine(x*SPACING+1,y*SPACING+13,2,0xA513);
      tft.drawFastHLine(x*SPACING+1,y*SPACING+15,2,0xA513);
      tft.drawFastVLine(x*SPACING+2,y*SPACING+5,4,0xFFFF);
      tft.drawFastVLine(x*SPACING+2,y*SPACING+11,4,0xFFFF);
    }
  }
  // Hero facing right
  else if(Characters[0].cDirection == 2) {
    // Draw Hat
    tft.drawFastHLine(x*SPACING,y*SPACING,11,0x1CB0);
    tft.drawFastHLine(x*SPACING+1,y*SPACING+1,11,0x1CB0);
    tft.drawFastHLine(x*SPACING+2,y*SPACING+2,11,0x1CB0);
    tft.drawFastHLine(x*SPACING+2,y*SPACING+3,4,0x1CB0);
    tft.drawFastHLine(x*SPACING+2,y*SPACING+4,3,0x1CB0);
    tft.drawFastHLine(x*SPACING+2,y*SPACING+5,2,0x1CB0);
    // Draw eyes
    tft.fillRect(x*SPACING+6,y*SPACING+4,2,2,0x0000);
    tft.fillRect(x*SPACING+10,y*SPACING+4,2,2,0x0000);
    // Draw Skin
    tft.drawFastHLine(x*SPACING+6,y*SPACING+3,7,0xEEEF);
    tft.drawFastVLine(x*SPACING+5,y*SPACING+4,4,0xEEEF);
    tft.drawFastVLine(x*SPACING+12,y*SPACING+4,3,0xEEEF);
    tft.fillRect(x*SPACING+8,y*SPACING+4,2,2,0xEEEF);
    tft.fillRect(x*SPACING+2,y*SPACING+6,10,2,0xEEEF);
    tft.drawPixel(x*SPACING+4,y*SPACING+5,0xEEEF);
    tft.fillRect(x*SPACING,y*SPACING+10,2,2,0xEEEF);
    tft.fillRect(x*SPACING+14,y*SPACING+8,2,2,0xEEEF);
    // Draw Tunic
    tft.fillRect(x*SPACING,y*SPACING+8,14,2,0x1CB0);
    tft.fillRect(x*SPACING+2,y*SPACING+10,10,1,0x1CB0);
    tft.fillRect(x*SPACING+2,y*SPACING+12,10,2,0x1CB0);
    // Draw Belt
    tft.fillRect(x*SPACING+2,y*SPACING+10,10,2,0xDD00);
    tft.fillRect(x*SPACING+8,y*SPACING+10,2,2,ILI9340_YELLOW);
    // Draw Boots
    tft.drawFastHLine(x*SPACING+2,y*SPACING+14,2,0xA3C0);
    tft.drawFastHLine(x*SPACING+2,y*SPACING+15,3,0xA3C0);
    tft.drawFastHLine(x*SPACING+10,y*SPACING+14,2,0xA3C0);
    tft.drawFastHLine(x*SPACING+10,y*SPACING+15,3,0xA3C0); 
    if(weapon == 0){
      // Draw Sword
      tft.drawFastHLine(x*SPACING+14,y*SPACING+7,2,0xA000);
      tft.fillRect(x*SPACING+14,y*SPACING+10,2,2,0xA000);
      tft.fillRect(x*SPACING+14,y*SPACING+2,2,7,0xDEDB);
    }
    else if (weapon == 1) {
      // Draw Spear
      tft.drawFastVLine(x*SPACING+14,y*SPACING,2,0x9DB7);
      tft.drawFastVLine(x*SPACING+13,y*SPACING+1,5,0x9DB7);
      tft.drawFastVLine(x*SPACING+14,y*SPACING+5,2,0x9DB7);
      tft.drawFastVLine(x*SPACING+15,y*SPACING+3,5,0x9DB7);
      tft.drawFastVLine(x*SPACING+15,y*SPACING+10,4,0x9DB7);
    }
    else if(weapon == 2){
      // Draw Bow
      tft.fillRect(x*SPACING+14,y*SPACING+6,2,2,0xC440);
      tft.fillRect(x*SPACING+14,y*SPACING+10,2,2,0xC440);
      tft.drawFastHLine(x*SPACING+14,y*SPACING+5,2,0xA513);
      tft.drawFastHLine(x*SPACING+13,y*SPACING+4,2,0xA513);
      tft.drawFastHLine(x*SPACING+12,y*SPACING+3,2,0xA513);
      tft.drawFastHLine(x*SPACING+14,y*SPACING+12,2,0xA513);
      tft.drawFastHLine(x*SPACING+13,y*SPACING+13,2,0xA513);
      tft.drawFastHLine(x*SPACING+12,y*SPACING+14,2,0xA513);
      tft.drawFastVLine(x*SPACING+12,y*SPACING+4,10,0xFFFF);
    }
  }
  // Hero facing down
  else {
    // Draw Hat
    tft.drawFastHLine(x*SPACING+5,y*SPACING,6,0x1CB0);
    tft.drawFastHLine(x*SPACING+4,y*SPACING+1,8,0x1CB0);
    tft.drawFastHLine(x*SPACING+5,y*SPACING+2,6,0x1CB0);
    tft.fillRect(x*SPACING+3,y*SPACING+2,2,2,0x1CB0);
    tft.fillRect(x*SPACING+11,y*SPACING+2,2,2,0x1CB0);
    tft.drawFastVLine(x*SPACING+3,y*SPACING+4,2,0x1CB0);
    tft.drawFastVLine(x*SPACING+12,y*SPACING+4,2,0x1CB0);
    // Draw eyes
    tft.fillRect(x*SPACING+5,y*SPACING+4,2,2,0x0000);
    tft.fillRect(x*SPACING+9,y*SPACING+4,2,2,0x0000);
    // Draw Skin
    tft.drawFastHLine(x*SPACING+5,y*SPACING+3,6,0xEEEF);
    tft.drawFastVLine(x*SPACING+4,y*SPACING+4,2,0xEEEF);
    tft.drawFastVLine(x*SPACING+11,y*SPACING+4,2,0xEEEF);
    tft.fillRect(x*SPACING+7,y*SPACING+4,2,2,0xEEEF);
    tft.fillRect(x*SPACING+4,y*SPACING+6,8,2,0xEEEF);
    tft.fillRect(x*SPACING+2,y*SPACING+10,2,2,0xEEEF);
    tft.fillRect(x*SPACING+13,y*SPACING+9,2,2,0xEEEF);
    // Draw Tunic
    tft.fillRect(x*SPACING+2,y*SPACING+8,11,2,0x1CB0);
    tft.fillRect(x*SPACING+4,y*SPACING+12,8,2,0x1CB0);
    // Draw Belt
    tft.fillRect(x*SPACING+4,y*SPACING+10,8,2,0xDD00);
    tft.fillRect(x*SPACING+7,y*SPACING+10,2,2,ILI9340_YELLOW);
    // Draw Boots
    tft.fillRect(x*SPACING+4,y*SPACING+14,2,2,0xA3C0);
    tft.fillRect(x*SPACING+10,y*SPACING+14,2,2,0xA3C0);
    if(weapon == 0) {
      // Draw Sword
      tft.drawFastHLine(x*SPACING+13,y*SPACING+8,2,0xA000);
      tft.fillRect(x*SPACING+13,y*SPACING+11,2,2,0xA000);
      tft.fillRect(x*SPACING+13,y*SPACING+1,2,7,0xDEDB);
    }
    else if (weapon == 1) {
      // Draw Spear
      tft.drawFastVLine(x*SPACING+12,y*SPACING+1,2,0x9DB7);
      tft.drawFastVLine(x*SPACING+14,y*SPACING+1,2,0x9DB7);
      tft.drawFastVLine(x*SPACING+11,y*SPACING+2,5,0x9DB7);
      tft.drawFastVLine(x*SPACING+15,y*SPACING+2,5,0x9DB7);
      tft.drawFastVLine(x*SPACING+12,y*SPACING+6,2,0x9DB7);
      tft.drawFastVLine(x*SPACING+14,y*SPACING+6,2,0x9DB7);
      tft.drawFastVLine(x*SPACING+13,y*SPACING+4,5,0x9DB7);
      tft.drawFastVLine(x*SPACING+13,y*SPACING+11,4,0x9DB7);
    }
    else if(weapon == 2) {
      // Draw Bow
      tft.fillRect(x*SPACING+13,y*SPACING+7,2,2,0xC440);
      tft.fillRect(x*SPACING+13,y*SPACING+11,2,2,0xC440);
      tft.fillRect(x*SPACING+13,y*SPACING+4,2,2,0xA513);
      tft.fillRect(x*SPACING+13,y*SPACING+13,2,2,0xA513);
    }
  }
}
