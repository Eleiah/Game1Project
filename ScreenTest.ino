#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

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

Adafruit_ILI9341 tft = Adafruit_ILI9341(_cs, _dc, _mosi, _sclk, _rst, _miso);

//Character class for Hero and Enemies
class CHARACTER
{
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
  void properties(int _type,int _posX,int _posY,int _cDirection,int _row,int _col,int _health,bool _cStatus,int _cTile) {
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

// Room class
class ROOM
{
  public:
  int enemies;
  int centres[4][2];
  void properties(int _enemies,int _centre1X,int _centre1Y,int _centre2X,int _centre2Y,int _centre3X,int _centre3Y,int _centre4X,int _centre4Y) {
    enemies = _enemies;
    centres[0][0] = _centre1X;
    centres[0][1] = _centre1Y;
    centres[1][0] = _centre2X;
    centres[1][0] = _centre2Y;
    centres[2][0] = _centre3X;
    centres[2][0] = _centre3Y;
    centres[3][0] = _centre4X;
    centres[3][0] = _centre4Y;
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
                            {10,5,5,5,5,5,5,5,5,5,5,5,5,5,10},
                            {10,5,0,0,0,0,0,0,0,0,0,0,0,5,10},
                            {10,5,0,0,0,0,0,0,0,0,0,0,0,5,10},
                            {10,5,0,0,0,5,5,0,0,5,5,0,0,5,10},
                            {10,5,0,0,0,5,5,0,0,5,5,0,0,5,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {10,5,0,0,0,5,5,0,0,5,5,0,0,5,10},
                            {10,5,0,0,0,5,5,0,0,5,5,0,0,5,10},
                            {10,5,0,0,0,0,0,0,0,0,0,0,0,5,10},
                            {10,5,0,0,0,0,0,0,0,0,0,0,0,5,10},
                            {10,5,5,5,5,5,5,5,5,5,5,5,5,5,10},
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
                            {10,0,0,0,0,0,14,14,14,14,14,14,14,0,10},
                            {10,0,0,14,14,0,0,14,10,14,14,14,14,0,10},
                            {10,0,14,14,14,14,0,0,10,10,14,14,14,0,10},
                            {10,14,14,14,14,14,10,0,0,10,0,14,14,0,10},
                            {0,0,0,0,0,0,0,0,0,10,0,1,1,0,0},
                            {10,14,14,14,14,14,10,0,0,10,0,14,14,0,10},
                            {10,0,14,14,14,14,0,0,10,10,14,14,14,0,10},
                            {10,0,0,14,14,0,0,14,10,14,14,14,14,0,10},
                            {10,0,0,0,0,0,14,14,14,14,14,14,14,0,10},
                            {10,0,0,0,0,14,14,14,14,14,14,14,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}},
                            
                           {{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10}, //T Room 2
                            {10,15,15,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,15,15,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,15,15,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,1,1,15,15,15,15,15,15,15,15,15,15,15,10},
                            {10,1,1,1,15,15,15,15,15,15,15,15,15,15,10},
                            {10,1,1,1,1,1,1,1,1,1,1,1,1,1,10},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {10,1,1,1,1,1,1,1,1,1,1,1,1,1,10},
                            {10,1,1,1,15,15,15,15,15,15,15,15,15,15,10},
                            {10,1,1,15,15,15,15,15,15,15,15,15,15,15,10},
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
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
                            {10,10,10,10,10,10,10,0,10,10,10,10,10,10,10}},

                           {{10,10,10,10,10,10,10,0,10,10,10,10,10,10,10}, //D Room1
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
int spacing = 16;
bool dPad[4];
//Initialize array of character objects
CHARACTER Characters[20];
// Initialize array of room objects
ROOM Rooms[3][3];
//Boolean statments to make sure the button press is only registered once
bool moveState = digitalRead(26);
bool lastMoveState = digitalRead(26);
bool attackState = digitalRead(27);
bool lastAttackState = digitalRead(27);
int wRow = 2;
int wCol = 1;
const int ENEMYCOUNT = 7;

void setup() {
  randomSeed(analogRead(0));
  //Character properties: type,posX,posY,cDirection,row,col,health,cStatus,cTile
  //Character 0 is the Hero
  Characters[0].properties(0,2,2,3,2,1,10,1,0);
  //All other characters are enemies
  //Left side Enemies
  Characters[4].properties(1,7,3,0,2,0,3,1,0);
  //Middle side Enemies
  Characters[1].properties(5,13,13,0,2,1,3,1,0);
  Characters[2].properties(5,13,2,1,2,1,3,1,0);
  Characters[7].properties(5,7,7,2,2,1,3,1,0);
  Characters[3].properties(5,2,13,3,2,1,3,1,0);
  //Right side Enemies
  Characters[5].properties(3,12,7,0,2,2,3,1,0);
  Characters[6].properties(3,12,7,0,1,2,3,1,0);
  //Room properties: enemies, centre1X, centre1Y, centre2X, centre2Y, centre3X, centre3Y, centre4X, centre4Y
  // Top Rooms
  Rooms[0][0].properties(3,7,7,0,0,0,0,0,0);
  Rooms[0][1].properties(1,7,7,0,0,0,0,0,0);
  Rooms[0][2].properties(4,6,4,8,4,6,10,8,10);
  // Middle Rooms
  Rooms[1][0].properties(2,7,13,7,0,0,0,0,0);
  Rooms[1][1].properties(0,7,7,0,0,0,0,0,0);
  Rooms[1][2].properties(2,0,7,9,4,9,10,0,0);
  // Bottom Rooms
  Rooms[2][0].properties(4,7,7,0,0,0,0,0,0);
  Rooms[2][1].properties(4,7,7,0,0,0,0,0,0);
  Rooms[2][2].properties(3,7,7,0,0,0,0,0,0);
  //Initialize pins for Dpad and buttons
  for(int i = 22;i<28;i++){
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
  //Initialize the screen
  tft.begin();
  //Set the screen to landscape and clear it
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  //initializeMap();
  drawMap();
}

void loop() {
  //Check current state of buttons
  moveState = digitalRead(26);
  attackState = digitalRead(27);
  //Check if user wants to change character direction
  faceDirection();
  //Inventory
  //If the move button's state changes
  if(moveState && !lastMoveState) {
    heroMove();
    enemiesMove();
    lastMoveState = moveState;
  }
  //If the button is in the same state, do nothing
  else{
    lastMoveState = moveState;
  }
  //Check if the attack button's state changes
  if(attackState && !lastAttackState){
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
  wMap[wRow][wCol][Characters[0].posX][Characters[0].posY] = 0;
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
  if(wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] < 10) {
    drawTile(Characters[0].posX*spacing,Characters[0].posY*spacing,Characters[0].cTile);
      wMap[wRow][wCol][Characters[0].posX][Characters[0].posY] = Characters[0].cTile;
      Characters[0].cTile = wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]];
      wMap[wRow][wCol][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] = 42;
      Characters[0].posX = Characters[0].posX + newPos[0];
      Characters[0].posY = Characters[0].posY + newPos[1];
      drawHero(Characters[0].posX,Characters[0].posY);
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
  if(wMap[wRow][wCol][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]] >= 100){
    int currentEnemy = wMap[wRow][wCol][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]]-100;
    Characters[currentEnemy].health--;
    if(Characters[currentEnemy].health <= 0) {
      Characters[currentEnemy].cStatus = 0;
      Rooms[Characters[currentEnemy].row][Characters[currentEnemy].col].enemies--;
      wMap[wRow][wCol][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]] = Characters[currentEnemy].cTile;
      drawTile(Characters[currentEnemy].posX*spacing,Characters[currentEnemy].posY*spacing,Characters[currentEnemy].cTile);
      Characters[0].health = Characters[0].health + random(2);
    }
  }
}
void enemiesMove(){
  for(int i = 1;i<ENEMYCOUNT+1;i++){
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
   if(charDistance(Characters[i].posX, Characters[i].posY) > 8) {
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
      tft.setCursor(240,20);
      tft.setTextSize(3);
      tft.setTextColor(0x0000);
      tft.print(Characters[0].health);
      tft.setCursor(240,20);
      Characters[0].health--;
      tft.setTextColor(0xF800);
      tft.print(Characters[0].health);
      if(Characters[0].health <= 0){
        drawTile(Characters[0].posX*spacing, Characters[0].posY*spacing,Characters[i].cTile);
        wMap[wRow][wCol][Characters[0].posX][Characters[0].posY] = Characters[0].cTile;
        tft.setCursor(41,110);
        tft.setTextSize(3);
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
            if(Characters[i].posX < Rooms[Characters[i].row][Characters[i].col].centres[j-10][0] && Characters[0].posX < Rooms[Characters[i].row][Characters[i].col].centres[j-10][0] && wMap[wRow][wCol][Characters[i].posX-1][Characters[i].posY] <= 9)
              row--;
            else if(Characters[i].posX > Rooms[Characters[i].row][Characters[i].col].centres[j-10][0] && Characters[0].posX > Rooms[Characters[i].row][Characters[i].col].centres[j-10][0] && wMap[wRow][wCol][Characters[i].posX+1][Characters[i].posY] <= 9)
              row++;
            else if(Characters[i].posY < Rooms[Characters[i].row][Characters[i].col].centres[j-10][1] && Characters[0].posY < Rooms[Characters[i].row][Characters[i].col].centres[j-10][1] && wMap[wRow][wCol][Characters[i].posX][Characters[i].posY-1] <= 9)
              column--;
            else if(Characters[i].posY > Rooms[Characters[i].row][Characters[i].col].centres[j-10][1] && Characters[0].posY > Rooms[Characters[i].row][Characters[i].col].centres[j-10][1] && wMap[wRow][wCol][Characters[i].posX][Characters[i].posY+1] <= 9)
              column++;
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

float charDistance(int charX,int charY) {
   return sqrt(pow((Characters[0].posX - charX),2) + pow((Characters[0].posY-charY),2));
}

void faceDirection() {
  for(int i = 0;i<4;i++) {
    dPad[i] = digitalRead(22+i);
    if(!dPad[i] && Characters[0].cDirection !=i) {
      Characters[0].cDirection = i;
      drawTile(Characters[0].posX*spacing,Characters[0].posY*spacing,Characters[0].cTile);
      drawHero(Characters[0].posX,Characters[0].posY);
    }
  }
}

void deleteChunk(int x,int y) {
  tft.fillRect(x*spacing,y*spacing,16,16,ILI9341_BLACK);
}

void drawMap() {
  tft.fillScreen(ILI9341_BLACK);
  for(int i = 0;i<15;i++) {
    for(int j = 0;j<15;j++) {
      drawTile(i*spacing,j*spacing,wMap[wRow][wCol][i][j]);
    }
  }
}

void drawTile(int xPos,int yPos,int tileType) {
  
  if(tileType == 0 || tileType >= 63 ){
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
  // Walls
  if((tileType >= 10 && tileType <= 13) || (tileType <= 62 && tileType >=60)) {
    tft.fillRect(xPos,yPos,spacing,spacing,0x5AEB);//Darker
    tft.fillRect(xPos+1,yPos+1,4,4,0x8430);//Lighter
    tft.fillRect(xPos+7,yPos+1,3,3,0x8430);
    tft.fillRect(xPos+12,yPos+1,3,3,0x8430);
    tft.fillRect(xPos+1,yPos+7,6,3,0x8430);
    tft.fillRect(xPos+9,yPos+6,6,3,0x8430);
    tft.fillRect(xPos+1,yPos+12,8,3,0x8430);
    tft.fillRect(xPos+11,yPos+11,4,4,0x8430);
  }
  if(tileType == 9) {
    //draw a door
  }
  // Traps
  if(tileType == 3){
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
  // Water
  if(tileType == 14){
    tft.fillRect(xPos,yPos,spacing,spacing,0x19D9);
  }
  //Lava
  if(tileType == 15){
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
  // Planks
  if(tileType == 1){
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
  if(tileType == 50){
    //Hearts?
  }
  int LG = 0x52A9;
  int DG = 0x3A07;
  int RD = 0xD041;
  int HP = 0xC808;
  int OR = 0xFD25;
  int YW = 0xFEC5;
  if(tileType == 60){ 
    //Block 1/9
    tft.drawFastVLine(xPos+6,yPos+7,5,LG);
    tft.drawFastVLine(xPos+7,yPos+5,3,LG);
    tft.drawFastVLine(xPos+8,yPos+4,2,LG);
    tft.drawFastVLine(xPos+9,yPos+3,2,LG);
    tft.drawFastVLine(xPos+10,yPos+2,2,LG);
    tft.drawFastVLine(xPos+11,yPos+1,2,LG);
    tft.drawFastVLine(xPos+12,yPos,5,LG);
    tft.drawFastVLine(xPos+13,yPos+4,3,LG);
    tft.drawFastVLine(xPos+14,yPos+6,3,LG);
    tft.drawFastVLine(xPos+15,yPos+8,2,LG);
    tft.drawFastVLine(xPos+7,yPos+11,3,LG);
    tft.drawFastVLine(xPos+8,yPos+13,2,LG);
    tft.drawFastVLine(xPos+9,yPos+14,2,LG);
    tft.drawFastHLine(xPos+10,yPos+15,2,LG);
    //Dark Grey
    tft.drawFastVLine(xPos+7,yPos+8,3,DG);
    tft.drawFastVLine(xPos+8,yPos+6,7,DG);
    tft.drawFastVLine(xPos+9,yPos+5,9,DG);
    tft.drawFastVLine(xPos+10,yPos+4,11,DG);
    tft.drawFastVLine(xPos+11,yPos+3,12,DG);
    tft.drawFastVLine(xPos+12,yPos+5,11,DG);
    tft.drawFastVLine(xPos+13,yPos+7,9,DG);
    tft.drawFastVLine(xPos+14,yPos+9,7,DG);
    tft.drawFastVLine(xPos+15,yPos+10,6,DG);
  }
  //Block 2/9
  else if(tileType == 61){
    //Light Gray Crown
    tft.drawFastVLine(xPos+5,yPos+7,2,LG);
    tft.drawFastVLine(xPos+6,yPos+5,6,LG);
    tft.drawFastVLine(xPos+7,yPos+4,7,LG);
    tft.drawFastVLine(xPos+8,yPos+4,7,LG);
    tft.drawFastVLine(xPos+9,yPos+5,6,LG);
    tft.drawFastVLine(xPos+10,yPos+7,2,LG);
    //Horns
    tft.drawFastHLine(xPos,yPos+9,2,LG);
    tft.drawFastHLine(xPos+14,yPos+9,2,LG);
    tft.drawFastHLine(xPos+1,yPos+10,14,LG);
    tft.drawFastHLine(xPos+2,yPos+11,12,LG);
    tft.drawFastHLine(xPos+3,yPos+13,10,LG);
    tft.drawFastHLine(xPos+2,yPos+14,2,LG);
    tft.drawFastHLine(xPos+12,yPos+14,2,LG);
    tft.drawFastHLine(xPos,yPos+15,3,LG);
    tft.drawFastHLine(xPos+13,yPos+15,3,LG);
    tft.drawFastVLine(xPos+6,yPos+11,4,LG);
    tft.drawFastVLine(xPos+9,yPos+11,4,LG);
    //Dark Grey Crown and Horns
    tft.fillRect(xPos+7,yPos+6,2,3,DG);
    tft.drawFastVLine(xPos,yPos+10,5,DG);
    tft.drawFastVLine(xPos+15,yPos+10,5,DG);
    tft.drawFastVLine(xPos+1,yPos+11,4,DG);
    tft.drawFastVLine(xPos+14,yPos+11,4,DG);
    tft.drawFastVLine(xPos+2,yPos+12,2,DG);
    tft.drawFastVLine(xPos+13,yPos+12,2,DG);
    tft.drawFastHLine(xPos+3,yPos+12,3,DG);
    tft.drawFastHLine(xPos+10,yPos+12,3,DG);
    //Red
    tft.drawFastHLine(xPos+4,yPos+10,3,RD);
    tft.drawFastHLine(xPos+9,yPos+10,3,RD);
    tft.drawPixel(xPos+5,yPos+9,RD);
    tft.drawPixel(xPos+10,yPos+9,RD);
    //HotPink
    tft.fillRect(xPos+7,yPos+11,2,2,HP);
    //Yellow
    tft.fillRect(xPos+7,yPos+13,2,3,YW);
    tft.drawFastHLine(xPos+5,yPos+15,6,YW);
    //Orange
    tft.drawFastHLine(xPos+4,yPos+14,2,OR);
    tft.drawFastHLine(xPos+10,yPos+14,2,OR);
    tft.drawFastHLine(xPos+3,yPos+15,2,OR);
    tft.drawFastHLine(xPos+11,yPos+15,2,OR);
  }
  //Block 3/9
  else if(tileType == 62){
    //Light Grey
    tft.drawFastVLine(xPos+9,yPos+7,5,LG);
    tft.drawFastVLine(xPos+8,yPos+5,3,LG);
    tft.drawFastVLine(xPos+7,yPos+4,2,LG);
    tft.drawFastVLine(xPos+6,yPos+3,2,LG);
    tft.drawFastVLine(xPos+5,yPos+2,2,LG);
    tft.drawFastVLine(xPos+4,yPos+1,2,LG);
    tft.drawFastVLine(xPos+3,yPos,5,LG);
    tft.drawFastVLine(xPos+2,yPos+4,3,LG);
    tft.drawFastVLine(xPos+1,yPos+6,3,LG);
    tft.drawFastVLine(xPos,yPos+8,2,LG);
    tft.drawFastVLine(xPos+8,yPos+11,3,LG);
    tft.drawFastVLine(xPos+7,yPos+13,2,LG);
    tft.drawFastVLine(xPos+6,yPos+14,2,LG);
    tft.drawFastHLine(xPos+4,yPos+15,2,LG);
    //Dark Grey
    tft.drawFastVLine(xPos+8,yPos+8,3,DG);
    tft.drawFastVLine(xPos+7,yPos+6,7,DG);
    tft.drawFastVLine(xPos+6,yPos+5,9,DG);
    tft.drawFastVLine(xPos+5,yPos+4,11,DG);
    tft.drawFastVLine(xPos+4,yPos+3,12,DG);
    tft.drawFastVLine(xPos+3,yPos+5,11,DG);
    tft.drawFastVLine(xPos+2,yPos+7,9,DG);
    tft.drawFastVLine(xPos+1,yPos+9,7,DG);
    tft.drawFastVLine(xPos,yPos+10,6,DG);
  }
  //Block 4/9
  else if(tileType == 63){
    //Light Grey
    tft.drawFastHLine(xPos+11,yPos,5,LG);
    tft.drawFastVLine(xPos+6,yPos+3,2,LG);
    tft.drawFastVLine(xPos+5,yPos+4,2,LG);
    tft.drawFastVLine(xPos+4,yPos+5,2,LG);
    tft.drawFastVLine(xPos+3,yPos+6,3,LG);
    tft.drawFastVLine(xPos+2,yPos+8,2,LG);
    tft.drawFastHLine(xPos+1,yPos+10,2,LG);
    tft.drawFastHLine(xPos+1,yPos+11,6,LG);
    tft.drawFastHLine(xPos+6,yPos+12,7,LG);
    tft.drawFastVLine(xPos+7,yPos+4,2,LG);
    tft.drawFastVLine(xPos+8,yPos+5,2,LG);
    tft.drawFastHLine(xPos+9,yPos+6,2,LG);
    tft.drawFastHLine(xPos+10,yPos+7,3,LG);
    tft.drawFastVLine(xPos+12,yPos+8,2,LG);
    tft.drawFastVLine(xPos+13,yPos+9,3,LG);
    tft.drawFastVLine(xPos+12,yPos+11,2,LG);
    //Dark Grey
    tft.drawFastHLine(xPos+6,yPos+5,1,DG);
    tft.drawFastHLine(xPos+5,yPos+6,3,DG);
    tft.drawFastHLine(xPos+4,yPos+7,6,DG);
    tft.drawFastHLine(xPos+4,yPos+8,8,DG);
    tft.drawFastHLine(xPos+3,yPos+9,9,DG);
    tft.drawFastHLine(xPos+3,yPos+10,10,DG);
    tft.drawFastHLine(xPos+7,yPos+11,6,DG);
    //Orange
    tft.drawFastVLine(xPos+10,yPos,2,OR);
    tft.drawFastHLine(xPos+11,yPos+1,5,OR);
    tft.drawPixel(xPos+7,yPos+3,OR);
    tft.drawPixel(xPos+8,yPos+4,OR);
    tft.drawFastHLine(xPos+9,yPos+5,2,OR);
    tft.drawFastHLine(xPos+11,yPos+6,2,OR);
    tft.drawFastVLine(xPos+13,yPos+7,2,OR);
    tft.drawFastVLine(xPos+14,yPos+9,3,OR);
    tft.drawPixel(xPos+13,yPos+12,OR);
    tft.drawFastHLine(xPos,yPos+12,6,OR);
    tft.drawFastHLine(xPos+5,yPos+13,8,OR);
    tft.drawFastVLine(xPos,yPos+14,2,OR);
    //Yellow
    tft.drawFastHLine(xPos+8,yPos,2,YW);
    tft.drawPixel(xPos+9,yPos+1,YW);
    tft.drawFastHLine(xPos+10,yPos+2,6,YW);
    tft.drawFastHLine(xPos+6,yPos+2,2,YW);
    tft.drawPixel(xPos+7,yPos+1,YW);
    tft.drawPixel(xPos+8,yPos+3,YW);
    tft.drawPixel(xPos+9,yPos+4,YW);
    tft.drawFastHLine(xPos+13,yPos+4,3,YW);
    tft.drawFastHLine(xPos+14,yPos+5,2,YW);
    tft.drawFastVLine(xPos+15,yPos+6,2,YW);
    tft.drawFastHLine(xPos+2,yPos+13,3,YW);
    tft.drawFastHLine(xPos+5,yPos+14,8,YW);
    tft.drawFastHLine(xPos+13,yPos+13,2,YW);
    tft.drawPixel(xPos+14,yPos+12,YW);
    tft.drawPixel(xPos+15,yPos+15,YW);
    tft.drawFastHLine(xPos+1,yPos+15,3,YW);
    //Red
    tft.drawPixel(xPos+8,yPos+1,RD);
    tft.drawFastHLine(xPos+8,yPos+2,2,RD);
    tft.drawFastHLine(xPos+9,yPos+3,7,RD);
    tft.drawFastHLine(xPos+10,yPos+4,3,RD);
    tft.drawFastHLine(xPos+12,yPos+5,2,RD);
    tft.drawFastHLine(xPos+13,yPos+6,2,RD);
    tft.drawFastVLine(xPos+14,yPos+7,2,RD);
    tft.drawFastVLine(xPos+15,yPos+8,7,RD);
    tft.drawFastHLine(xPos,yPos+13,2,RD);
    tft.drawFastHLine(xPos+1,yPos+14,4,RD);
    tft.drawFastHLine(xPos+4,yPos+15,11,RD);
    tft.drawFastHLine(xPos+13,yPos+14,3,RD);
  }
  //Block 5/9
  else if(tileType == 64){
    //Light Grey
    tft.drawPixel(xPos,yPos,LG);
    tft.drawPixel(xPos+15,yPos,LG);
    tft.drawFastHLine(xPos+6,yPos+5,4,LG);
    tft.drawFastHLine(xPos+2,yPos+6,12,LG);
    tft.drawFastHLine(xPos+2,yPos+7,12,LG);
    tft.drawFastHLine(xPos+3,yPos+8,10,LG);
    tft.drawFastHLine(xPos+5,yPos+9,6,LG);
    tft.drawFastHLine(xPos+5,yPos+13,6,LG);
    tft.drawFastHLine(xPos+4,yPos+14,8,LG);
    tft.drawFastHLine(xPos+3,yPos+15,10,LG);
    //Dark Grey
    tft.drawFastHLine(xPos+6,yPos+6,4,DG);
    tft.drawFastHLine(xPos+4,yPos+7,8,DG);
    tft.drawFastHLine(xPos+5,yPos+8,6,DG);
    tft.drawFastHLine(xPos+7,yPos+9,2,DG);
    tft.drawFastHLine(xPos+6,yPos+14,4,DG);
    tft.drawFastHLine(xPos+5,yPos+15,6,DG);
    //Orange
    tft.drawFastHLine(xPos+1,yPos,4,OR);
    tft.drawFastHLine(xPos,yPos+1,3,OR);
    tft.drawFastHLine(xPos+11,yPos,4,OR);
    tft.drawFastHLine(xPos+13,yPos+1,3,OR);
    tft.drawFastHLine(xPos+6,yPos+4,4,OR);
    tft.drawFastHLine(xPos,yPos+5,6,OR);
    tft.drawFastHLine(xPos+10,yPos+5,6,OR);
    tft.drawFastHLine(xPos,yPos+6,2,OR);
    tft.drawFastHLine(xPos+14,yPos+6,2,OR);
    tft.drawPixel(xPos+1,yPos+7,OR);
    tft.drawPixel(xPos+14,yPos+7,OR);
    tft.drawFastHLine(xPos+1,yPos+8,2,OR);
    tft.drawFastHLine(xPos+13,yPos+8,2,OR);
    tft.drawFastHLine(xPos+2,yPos+9,3,OR);
    tft.drawFastHLine(xPos+11,yPos+9,3,OR);
    tft.drawFastHLine(xPos+5,yPos+10,6,OR);
    tft.drawFastHLine(xPos+4,yPos+12,8,OR);
    tft.drawFastHLine(xPos+3,yPos+13,2,OR);
    tft.drawFastHLine(xPos+11,yPos+13,2,OR);
    tft.drawFastHLine(xPos+2,yPos+14,2,OR);
    tft.drawFastHLine(xPos+12,yPos+14,2,OR);
    tft.drawPixel(xPos+2,yPos+15,OR);
    tft.drawPixel(xPos+13,yPos+15,OR);
    //Draw Yellow
    tft.drawFastHLine(xPos,yPos+2,4,YW);
    tft.drawFastHLine(xPos+3,yPos+1,4,YW);
    tft.drawFastHLine(xPos+5,yPos,2,YW);
    tft.drawFastHLine(xPos+9,yPos,2,YW);
    tft.drawFastHLine(xPos+9,yPos+1,4,YW);
    tft.drawFastHLine(xPos+12,yPos+2,4,YW);
    tft.drawFastHLine(xPos,yPos+4,6,YW);
    tft.drawFastHLine(xPos+6,yPos+3,4,YW);
    tft.drawFastHLine(xPos+10,yPos+4,6,YW);
    tft.drawFastVLine(xPos,yPos+7,3,YW);
    tft.drawFastVLine(xPos+1,yPos+9,2,YW);
    tft.drawFastHLine(xPos+2,yPos+10,3,YW);
    tft.drawFastHLine(xPos+11,yPos+10,3,YW);
    tft.drawFastVLine(xPos+14,yPos+9,2,YW);
    tft.drawFastVLine(xPos+15,yPos+7,3,YW);
    tft.drawFastHLine(xPos+7,yPos+11,2,YW);
    tft.drawFastHLine(xPos+2,yPos+12,2,YW);
    tft.drawFastHLine(xPos+12,yPos+12,2,YW);
    tft.drawFastHLine(xPos+1,yPos+13,2,YW);
    tft.drawFastHLine(xPos+13,yPos+13,2,YW);
    tft.drawPixel(xPos+1,yPos+14,YW);
    tft.drawPixel(xPos+14,yPos+14,YW);
    tft.drawFastHLine(xPos,yPos+15,2,YW);
    tft.drawFastHLine(xPos+14,yPos+15,2,YW);
    //Draw Red
    tft.drawFastHLine(xPos,yPos+3,6,RD);
    tft.drawFastHLine(xPos+4,yPos+2,8,RD);
    tft.drawFastHLine(xPos+10,yPos+3,6,RD);
    tft.fillRect(xPos+7,yPos,2,2,RD);
    tft.drawPixel(xPos,yPos+10,RD);
    tft.drawPixel(xPos+15,yPos+10,RD);
    tft.drawPixel(xPos+1,yPos+12,RD);
    tft.drawPixel(xPos+14,yPos+12,RD);
    tft.drawFastHLine(xPos+1,yPos+11,6,RD);
    tft.drawFastHLine(xPos+9,yPos+11,6,RD);
    tft.drawFastVLine(xPos,yPos+13,2,RD);
    tft.drawFastVLine(xPos+15,yPos+13,2,RD);
    //Draw Pink
    tft.drawFastVLine(xPos,yPos+11,2,HP);
    tft.drawFastVLine(xPos+15,yPos+11,2,HP);
  }
  //Block 6/9
  else if(tileType == 65){
    //Light Grey
    tft.drawFastHLine(xPos,yPos,5,LG);
    tft.drawFastVLine(xPos+9,yPos+3,2,LG);
    tft.drawFastVLine(xPos+10,yPos+4,2,LG);
    tft.drawFastVLine(xPos+11,yPos+5,2,LG);
    tft.drawFastVLine(xPos+12,yPos+6,3,LG);
    tft.drawFastVLine(xPos+13,yPos+8,2,LG);
    tft.drawFastHLine(xPos+13,yPos+10,2,LG);
    tft.drawFastHLine(xPos+9,yPos+11,6,LG);
    tft.drawFastHLine(xPos+3,yPos+12,7,LG);
    tft.drawFastVLine(xPos+8,yPos+4,2,LG);
    tft.drawFastVLine(xPos+7,yPos+5,2,LG);
    tft.drawFastHLine(xPos+5,yPos+6,2,LG);
    tft.drawFastHLine(xPos+3,yPos+7,3,LG);
    tft.drawFastVLine(xPos+3,yPos+8,2,LG);
    tft.drawFastVLine(xPos+2,yPos+9,3,LG);
    tft.drawFastVLine(xPos+3,yPos+11,2,LG);
    //Dark Grey
    tft.drawFastHLine(xPos+9,yPos+5,1,DG);
    tft.drawFastHLine(xPos+8,yPos+6,3,DG);
    tft.drawFastHLine(xPos+6,yPos+7,6,DG);
    tft.drawFastHLine(xPos+4,yPos+8,8,DG);
    tft.drawFastHLine(xPos+4,yPos+9,9,DG);
    tft.drawFastHLine(xPos+3,yPos+10,10,DG);
    tft.drawFastHLine(xPos+4,yPos+11,6,DG);
    //Orange 
    tft.drawFastVLine(xPos+5,yPos,2,OR);
    tft.drawFastHLine(xPos,yPos+1,5,OR);
    tft.drawPixel(xPos+8,yPos+3,OR);
    tft.drawPixel(xPos+7,yPos+4,OR);
    tft.drawFastHLine(xPos+5,yPos+5,2,OR);
    tft.drawFastHLine(xPos+3,yPos+6,2,OR);
    tft.drawFastVLine(xPos+2,yPos+7,2,OR);
    tft.drawFastVLine(xPos+1,yPos+9,3,OR);
    tft.drawPixel(xPos+2,yPos+12,OR);
    tft.drawFastHLine(xPos+10,yPos+12,6,OR);
    tft.drawFastHLine(xPos+3,yPos+13,8,OR);
    tft.drawFastVLine(xPos+15,yPos+14,2,OR);
    //Yellow
    tft.drawFastHLine(xPos+6,yPos,2,YW);
    tft.drawPixel(xPos+6,yPos+1,YW);
    tft.drawFastHLine(xPos,yPos+2,6,YW);
    tft.drawFastHLine(xPos+8,yPos+2,2,YW);
    tft.drawPixel(xPos+8,yPos+1,YW);
    tft.drawPixel(xPos+7,yPos+3,YW);
    tft.drawPixel(xPos+6,yPos+4,YW);
    tft.drawFastHLine(xPos,yPos+4,3,YW);
    tft.drawFastHLine(xPos,yPos+5,2,YW);
    tft.drawFastVLine(xPos,yPos+6,2,YW);
    tft.drawFastHLine(xPos+11,yPos+13,3,YW);
    tft.drawFastHLine(xPos+3,yPos+14,8,YW);
    tft.drawFastHLine(xPos+1,yPos+13,2,YW);
    tft.drawPixel(xPos+1,yPos+12,YW);
    tft.drawPixel(xPos,yPos+15,YW);
    tft.drawFastHLine(xPos+12,yPos+15,3,YW);
    //Red
    tft.drawPixel(xPos+7,yPos+1,RD);
    tft.drawFastHLine(xPos+6,yPos+2,2,RD);
    tft.drawFastHLine(xPos,yPos+3,7,RD);
    tft.drawFastHLine(xPos+3,yPos+4,3,RD);
    tft.drawFastHLine(xPos+2,yPos+5,2,RD);
    tft.drawFastHLine(xPos+1,yPos+6,2,RD);
    tft.drawFastVLine(xPos+1,yPos+7,2,RD);
    tft.drawFastVLine(xPos,yPos+8,7,RD);
    tft.drawFastHLine(xPos+14,yPos+13,2,RD);
    tft.drawFastHLine(xPos+11,yPos+14,4,RD);
    tft.drawFastHLine(xPos+1,yPos+15,11,RD);
    tft.drawFastHLine(xPos,yPos+14,3,RD);
  }
  //Block 7/9
  else if(tileType == 66){
    //Light Grey
    tft.drawPixel(xPos,yPos,LG);
    tft.drawPixel(xPos+1,yPos+2,LG);
    tft.drawFastHLine(xPos+2,yPos+3,6,LG);
    tft.drawPixel(xPos+9,yPos+2,LG);
    tft.drawFastVLine(xPos+10,yPos+2,3,LG);
    tft.drawPixel(xPos+11,yPos+9,LG);
    tft.drawFastHLine(xPos+10,yPos+10,2,LG);
    tft.drawFastHLine(xPos+8,yPos+11,2,LG);
    tft.drawPixel(xPos+7,yPos+12,LG);
    tft.drawPixel(xPos+12,yPos+11,LG);
    tft.drawPixel(xPos+14,yPos+12,LG);
    //Dark Grey
    tft.drawFastHLine(xPos,yPos+1,3,DG);
    tft.drawFastHLine(xPos+2,yPos+2,7,DG);
    tft.drawFastVLine(xPos,yPos+2,2,DG);
    tft.drawFastVLine(xPos+1,yPos+3,3,DG);
    tft.drawFastVLine(xPos+2,yPos+4,3,DG);
    tft.drawFastVLine(xPos+3,yPos+4,4,DG);
    tft.drawFastVLine(xPos+4,yPos+4,1,DG);
    tft.drawFastVLine(xPos+5,yPos+4,3,DG);
    tft.drawFastVLine(xPos+6,yPos+4,4,DG);
    tft.drawFastVLine(xPos+7,yPos+4,1,DG);
    tft.drawFastVLine(xPos+8,yPos+3,4,DG);
    tft.drawFastVLine(xPos+9,yPos+3,5,DG);
    tft.drawFastVLine(xPos+10,yPos+5,1,DG);
    tft.drawFastVLine(xPos+6,yPos+14,2,DG);
    tft.drawFastVLine(xPos+7,yPos+13,3,DG);
    tft.drawFastVLine(xPos+8,yPos+12,3,DG);
    tft.drawFastVLine(xPos+9,yPos+12,2,DG);
    tft.drawFastVLine(xPos+10,yPos+11,5,DG);
    tft.drawFastVLine(xPos+11,yPos+11,4,DG);
    tft.drawFastVLine(xPos+12,yPos+12,2,DG);
    tft.drawFastVLine(xPos+13,yPos+12,4,DG);
    tft.drawFastVLine(xPos+14,yPos+13,2,DG);
    tft.drawFastVLine(xPos+15,yPos+13,1,DG);
    //Orange
    tft.drawFastHLine(xPos+1,yPos,3,OR);
    tft.drawFastHLine(xPos+3,yPos+1,9,OR);
    tft.drawFastVLine(xPos+11,yPos+1,3,OR);
    tft.drawFastVLine(xPos+15,yPos+3,2,OR);
    tft.drawFastHLine(xPos+12,yPos+10,2,OR);
    tft.drawFastHLine(xPos+13,yPos+11,3,OR);
    tft.drawPixel(xPos+15,yPos+12,OR);
    //Yellow
    tft.drawFastHLine(xPos+4,yPos,8,YW);
    tft.drawFastVLine(xPos+12,yPos+1,3,YW);
    tft.drawPixel(xPos+11,yPos+4,YW);
    tft.drawFastVLine(xPos+15,yPos,3,YW);
    tft.drawFastVLine(xPos+14,yPos+3,3,YW);
    tft.drawPixel(xPos+15,yPos+5,YW);
    tft.drawFastHLine(xPos+12,yPos+9,2,YW);
    tft.drawFastHLine(xPos+14,yPos+10,2,YW);
    //Red
    tft.drawPixel(xPos+12,yPos,RD);
    tft.drawFastVLine(xPos+14,yPos,3,RD);
    tft.drawFastVLine(xPos+13,yPos+3,3,RD);
    tft.drawFastVLine(xPos+11,yPos+5,2,RD);
    tft.drawFastVLine(xPos+12,yPos+6,3,RD);
    tft.drawFastVLine(xPos+15,yPos+6,2,RD);
    tft.drawPixel(xPos+14,yPos+6,RD);
    tft.drawFastHLine(xPos+13,yPos+8,2,RD);
    tft.drawFastHLine(xPos+14,yPos+9,2,RD);
    tft.drawFastVLine(xPos+9,yPos+14,2,RD);
    tft.drawFastVLine(xPos+12,yPos+14,2,RD);
    tft.drawFastVLine(xPos+15,yPos+14,2,RD);
    tft.drawPixel(xPos+8,yPos+15,RD);
    tft.drawPixel(xPos+11,yPos+15,RD);
    tft.drawPixel(xPos+14,yPos+15,RD);
    //Pink
    tft.drawFastVLine(xPos+13,yPos,3,HP);
    tft.drawFastVLine(xPos+12,yPos+4,2,HP);
    tft.drawFastVLine(xPos+13,yPos+6,2,HP);
    tft.drawPixel(xPos+14,yPos+7,HP);
    tft.drawPixel(xPos+15,yPos+8,HP);
  }
  //Block 8/9
  else if(tileType == 67){
    //Light Grey
    tft.drawFastHLine(xPos+2,yPos,2,LG);
    tft.drawFastHLine(xPos+12,yPos,2,LG);
    tft.drawFastHLine(xPos+1,yPos+1,2,LG);
    tft.drawFastHLine(xPos+13,yPos+1,2,LG);
    tft.drawPixel(xPos+1,yPos+2,LG);
    tft.drawPixel(xPos+14,yPos+2,LG);
    tft.drawFastHLine(xPos,yPos+3,2,LG);
    tft.drawFastHLine(xPos+14,yPos+3,2,LG);
    tft.drawFastHLine(xPos,yPos+4,5,LG);
    tft.drawFastHLine(xPos+11,yPos+4,5,LG);
    tft.drawFastHLine(xPos+4,yPos+5,2,LG);
    tft.drawFastHLine(xPos+10,yPos+5,2,LG);
    tft.drawPixel(xPos+6,yPos+6,LG);
    tft.drawPixel(xPos+9,yPos+6,LG);
    tft.drawFastHLine(xPos+7,yPos+7,2,LG);
    tft.drawFastVLine(xPos+1,yPos+13,2,LG);
    tft.drawFastVLine(xPos+14,yPos+13,2,LG);
    //Dark Grey
    tft.drawFastHLine(xPos+4,yPos,8,DG);
    tft.drawFastHLine(xPos+3,yPos+1,10,DG);
    tft.drawFastHLine(xPos+2,yPos+2,12,DG);
    tft.drawFastHLine(xPos+2,yPos+3,12,DG);
    tft.drawFastHLine(xPos+5,yPos+4,6,DG);
    tft.drawFastHLine(xPos+6,yPos+5,4,DG);
    tft.drawFastHLine(xPos+7,yPos+6,2,DG);
    tft.drawFastVLine(xPos,yPos+13,3,DG);
    tft.drawFastVLine(xPos+15,yPos+13,3,DG);
    //Orange
    tft.drawFastVLine(xPos,yPos,3,OR);
    tft.drawPixel(xPos+1,yPos,OR);
    tft.drawFastVLine(xPos+15,yPos,3,OR);
    tft.drawPixel(xPos+14,yPos,OR);
    tft.drawFastHLine(xPos,yPos+5,4,OR);
    tft.drawFastHLine(xPos+12,yPos+5,4,OR);
    tft.drawFastHLine(xPos+4,yPos+6,2,OR);
    tft.drawFastHLine(xPos+10,yPos+6,2,OR);
    tft.drawPixel(xPos+6,yPos+7,OR);
    tft.drawPixel(xPos+9,yPos+7,OR);
    tft.drawFastHLine(xPos+7,yPos+8,2,OR);
    tft.drawFastHLine(xPos,yPos+12,16,OR);
    tft.drawFastHLine(xPos+6,yPos+13,4,OR);
    //Yellow
    tft.drawFastHLine(xPos,yPos+6,4,YW);
    tft.drawFastHLine(xPos+12,yPos+6,4,YW);
    tft.drawFastHLine(xPos+3,yPos+7,3,YW);
    tft.drawFastHLine(xPos+10,yPos+7,3,YW);
    tft.drawFastHLine(xPos+5,yPos+8,2,YW);
    tft.drawFastHLine(xPos+9,yPos+8,2,YW);
    tft.drawFastHLine(xPos+6,yPos+9,4,YW);
    tft.drawPixel(xPos,yPos+10,YW);
    tft.drawPixel(xPos+15,yPos+10,YW);
    tft.drawFastHLine(xPos,yPos+11,16,YW);
    tft.drawFastHLine(xPos+7,yPos+12,2,YW);
    //Red
    tft.drawFastHLine(xPos,yPos+7,3,RD);
    tft.drawFastHLine(xPos+13,yPos+7,3,RD);
    tft.drawFastHLine(xPos+2,yPos+8,3,RD);
    tft.drawFastHLine(xPos+11,yPos+8,3,RD);
    tft.drawFastHLine(xPos+4,yPos+9,2,RD);
    tft.drawFastHLine(xPos+10,yPos+9,2,RD);
    tft.drawPixel(xPos,yPos+9,RD);
    tft.drawPixel(xPos+15,yPos+9,RD);
    tft.drawFastHLine(xPos+1,yPos+10,14,RD);
    tft.drawFastHLine(xPos+7,yPos+11,2,RD);
    //Pink
    tft.drawFastHLine(xPos,yPos+8,2,HP);
    tft.drawFastHLine(xPos+14,yPos+8,2,HP);
    tft.drawFastHLine(xPos+1,yPos+9,3,HP);
    tft.drawFastHLine(xPos+12,yPos+9,3,HP);
  }
  else if(tileType == 68){
    //Light Grey
    tft.drawPixel(xPos+15,yPos,LG);
    tft.drawPixel(xPos+14,yPos+2,LG);
    tft.drawFastHLine(xPos+8,yPos+3,6,LG);
    tft.drawPixel(xPos+6,yPos+2,LG);
    tft.drawFastVLine(xPos+5,yPos+2,3,LG);
    tft.drawPixel(xPos+4,yPos+9,LG);
    tft.drawFastHLine(xPos+4,yPos+10,2,LG);
    tft.drawFastHLine(xPos+6,yPos+11,2,LG);
    tft.drawPixel(xPos+8,yPos+12,LG);
    tft.drawPixel(xPos+3,yPos+11,LG);
    tft.drawPixel(xPos+1,yPos+12,LG);
    //Dark Grey
    tft.drawFastHLine(xPos+13,yPos+1,3,DG);
    tft.drawFastHLine(xPos+7,yPos+2,7,DG);
    tft.drawFastVLine(xPos+15,yPos+2,2,DG);
    tft.drawFastVLine(xPos+14,yPos+3,3,DG);
    tft.drawFastVLine(xPos+13,yPos+4,3,DG);
    tft.drawFastVLine(xPos+12,yPos+4,4,DG);
    tft.drawFastVLine(xPos+11,yPos+4,1,DG);
    tft.drawFastVLine(xPos+10,yPos+4,3,DG);
    tft.drawFastVLine(xPos+9,yPos+4,4,DG);
    tft.drawFastVLine(xPos+8,yPos+4,1,DG);
    tft.drawFastVLine(xPos+7,yPos+3,4,DG);
    tft.drawFastVLine(xPos+6,yPos+3,5,DG);
    tft.drawFastVLine(xPos+5,yPos+5,1,DG);
    tft.drawFastVLine(xPos+9,yPos+14,2,DG);
    tft.drawFastVLine(xPos+8,yPos+13,3,DG);
    tft.drawFastVLine(xPos+7,yPos+12,3,DG);
    tft.drawFastVLine(xPos+6,yPos+12,2,DG);
    tft.drawFastVLine(xPos+5,yPos+11,5,DG);
    tft.drawFastVLine(xPos+4,yPos+11,4,DG);
    tft.drawFastVLine(xPos+3,yPos+12,2,DG);
    tft.drawFastVLine(xPos+2,yPos+12,4,DG);
    tft.drawFastVLine(xPos+1,yPos+13,2,DG);
    tft.drawFastVLine(xPos,yPos+13,1,DG);
    //Orange
    tft.drawFastHLine(xPos+12,yPos,3,OR);
    tft.drawFastHLine(xPos+4,yPos+1,9,OR);
    tft.drawFastVLine(xPos+4,yPos+1,3,OR);
    tft.drawFastVLine(xPos,yPos+3,2,OR);
    tft.drawFastHLine(xPos+2,yPos+10,2,OR);
    tft.drawFastHLine(xPos,yPos+11,3,OR);
    tft.drawPixel(xPos,yPos+12,OR);
    //Yellow
    tft.drawFastHLine(xPos+4,yPos,8,YW);
    tft.drawFastVLine(xPos+3,yPos+1,3,YW);
    tft.drawPixel(xPos+4,yPos+4,YW);
    tft.drawFastVLine(xPos,yPos,3,YW);
    tft.drawFastVLine(xPos+1,yPos+3,3,YW);
    tft.drawPixel(xPos,yPos+5,YW);
    tft.drawFastHLine(xPos+2,yPos+9,2,YW);
    tft.drawFastHLine(xPos,yPos+10,2,YW);
    //Red
    tft.drawPixel(xPos+3,yPos,RD);
    tft.drawFastVLine(xPos+1,yPos,3,RD);
    tft.drawFastVLine(xPos+2,yPos+3,3,RD);
    tft.drawFastVLine(xPos+4,yPos+5,2,RD);
    tft.drawFastVLine(xPos+3,yPos+6,3,RD);
    tft.drawFastVLine(xPos,yPos+6,2,RD);
    tft.drawPixel(xPos+1,yPos+6,RD);
    tft.drawFastHLine(xPos+1,yPos+8,2,RD);
    tft.drawFastHLine(xPos,yPos+9,2,RD);
    tft.drawFastVLine(xPos+6,yPos+14,2,RD);
    tft.drawFastVLine(xPos+3,yPos+14,2,RD);
    tft.drawFastVLine(xPos,yPos+14,2,RD);
    tft.drawPixel(xPos+7,yPos+15,RD);
    tft.drawPixel(xPos+4,yPos+15,RD);
    tft.drawPixel(xPos+1,yPos+15,RD);
    //Pink
    tft.drawFastVLine(xPos+2,yPos,3,HP);
    tft.drawFastVLine(xPos+3,yPos+4,2,HP);
    tft.drawFastVLine(xPos+2,yPos+6,2,HP);
    tft.drawPixel(xPos+1,yPos+7,HP);
    tft.drawPixel(xPos,yPos+8,HP);
  }
}

void drawEnemy(int i){
  int x = Characters[i].posX;
  int y = Characters[i].posY;
  int colour;
  if(Characters[i].type == 1 || Characters[i].type == 2){
    colour = 0x3C00;
  }
  else if(Characters[i].type == 5){
    colour = 0x6011;
  }
  else{
    colour = 0xA020;
  }

  //Enemy Facing Left
  if(Characters[i].cDirection == 0){
    //Enemy Horns
    tft.drawFastVLine(x*spacing+2,y*spacing,3,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+3,y*spacing+1,3,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+4,y*spacing+1,1,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+13,y*spacing+1,1,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+14,y*spacing+1,3,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+15,y*spacing,3,ILI9341_WHITE);
    //Enemy Head
    tft.fillRect(x*spacing+6,y*spacing,6,7,colour);
    tft.drawFastVLine(x*spacing+12,y*spacing+1,6,colour);
    tft.drawFastVLine(x*spacing+13,y*spacing+2,4,colour);
    tft.drawFastVLine(x*spacing+5,y*spacing+1,3,colour);
    tft.drawFastVLine(x*spacing+4,y*spacing+2,2,colour);
    tft.drawFastHLine(x*spacing+6,y*spacing+7,5,colour);
    tft.drawPixel(x*spacing+5,y*spacing+6,colour);
    //Enemy Body
    tft.fillRect(x*spacing+1,y*spacing+8,13,2,colour);
    tft.fillRect(x*spacing+13,y*spacing+9,2,3,colour);
    tft.fillRect(x*spacing+6,y*spacing+12,6,2,colour);
    tft.fillRect(x*spacing+5,y*spacing+13,2,2,colour);
    tft.fillRect(x*spacing+4,y*spacing+14,2,2,colour);
    tft.fillRect(x*spacing+11,y*spacing+13,2,2,colour);
    tft.fillRect(x*spacing+12,y*spacing+14,2,2,colour);
    //Draw Eyes
    tft.fillRect(x*spacing+4,y*spacing+4,2,2,0xDDE7);
    tft.fillRect(x*spacing+8,y*spacing+4,2,2,0xDDE7);
    //Draw Belt
    tft.fillRect(x*spacing+6,y*spacing+10,6,2,0xCC61);
    tft.fillRect(x*spacing+7,y*spacing+10,2,2,0xCE58);
  }
  //Enemy Facing Up
  else if(Characters[i].cDirection == 1){
    //Enemy Horns
    tft.drawFastVLine(x*spacing+1,y*spacing,3,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+2,y*spacing+1,3,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+3,y*spacing+1,1,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+12,y*spacing+1,1,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+13,y*spacing+1,3,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+14,y*spacing,3,ILI9341_WHITE);
    //Enemy Head
    tft.fillRect(x*spacing+5,y*spacing,6,7,colour);
    tft.drawFastVLine(x*spacing+4,y*spacing+1,6,colour);
    tft.drawFastVLine(x*spacing+3,y*spacing+2,4,colour);
    tft.drawFastVLine(x*spacing+11,y*spacing+1,6,colour);
    tft.drawFastVLine(x*spacing+12,y*spacing+2,4,colour);
    tft.drawFastHLine(x*spacing+6,y*spacing+7,4,colour);
    //Draw Body
    tft.fillRect(x*spacing+3,y*spacing+8,10,2,colour);
    tft.fillRect(x*spacing+1,y*spacing+9,2,2,colour);
    tft.fillRect(x*spacing+12,y*spacing+9,2,2,colour);
    tft.drawPixel(x*spacing+13,y*spacing+11,colour);
    tft.fillRect(x*spacing+5,y*spacing+12,6,2,colour);
    tft.fillRect(x*spacing+4,y*spacing+13,2,2,colour);
    tft.fillRect(x*spacing+3,y*spacing+14,2,2,colour);
    tft.fillRect(x*spacing+10,y*spacing+13,2,2,colour);
    tft.fillRect(x*spacing+11,y*spacing+14,2,2,colour);
    //Draw Belt
    tft.fillRect(x*spacing+5,y*spacing+10,6,2,0xCC61);
  }
  //Enemy Facing Right
  else if(Characters[i].cDirection == 2){
    //Enemy Horns
    tft.drawFastVLine(x*spacing,y*spacing,3,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+1,y*spacing+1,3,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+2,y*spacing+1,1,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+11,y*spacing+1,1,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+12,y*spacing+1,3,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+13,y*spacing,3,ILI9341_WHITE);
    //Enemy Head
    tft.fillRect(x*spacing+4,y*spacing,6,7,colour);
    tft.drawFastVLine(x*spacing+3,y*spacing+1,6,colour);
    tft.drawFastVLine(x*spacing+2,y*spacing+2,4,colour);
    tft.drawFastVLine(x*spacing+10,y*spacing+1,3,colour);
    tft.drawFastVLine(x*spacing+11,y*spacing+2,2,colour);
    tft.drawFastHLine(x*spacing+5,y*spacing+7,5,colour);
    tft.drawPixel(x*spacing+10,y*spacing+6,colour);
    //Draw Body
    tft.fillRect(x*spacing+2,y*spacing+8,13,2,colour);
    tft.fillRect(x*spacing+1,y*spacing+9,2,3,colour);
    tft.fillRect(x*spacing+4,y*spacing+12,6,2,colour);
    tft.fillRect(x*spacing+3,y*spacing+13,2,2,colour);
    tft.fillRect(x*spacing+2,y*spacing+14,2,2,colour);
    tft.fillRect(x*spacing+9,y*spacing+13,2,2,colour);
    tft.fillRect(x*spacing+10,y*spacing+14,2,2,colour);
    //Draw Eyes
    tft.fillRect(x*spacing+6,y*spacing+4,2,2,0xDDE7);
    tft.fillRect(x*spacing+10,y*spacing+4,2,2,0xDDE7);
    //Draw Belt
    tft.fillRect(x*spacing+4,y*spacing+10,6,2,0xCC61);
    tft.fillRect(x*spacing+7,y*spacing+10,2,2,0xCE58);
    }
    //Enemy Facing Down
    else{
      //Enemy Horns
    tft.drawFastVLine(x*spacing+1,y*spacing,3,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+2,y*spacing+1,3,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+3,y*spacing+1,1,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+12,y*spacing+1,1,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+13,y*spacing+1,3,ILI9341_WHITE);
    tft.drawFastVLine(x*spacing+14,y*spacing,3,ILI9341_WHITE);
    //Enemy Head
    tft.fillRect(x*spacing+5,y*spacing,6,7,colour);
    tft.drawFastVLine(x*spacing+4,y*spacing+1,6,colour);
    tft.drawFastVLine(x*spacing+3,y*spacing+2,4,colour);
    tft.drawFastVLine(x*spacing+11,y*spacing+1,6,colour);
    tft.drawFastVLine(x*spacing+12,y*spacing+2,4,colour);
    tft.drawFastHLine(x*spacing+6,y*spacing+7,4,colour);
    //Draw Eyes
    tft.fillRect(x*spacing+5,y*spacing+4,2,2,0xDDE7);
    tft.fillRect(x*spacing+9,y*spacing+4,2,2,0xDDE7);
    //Draw Body
    tft.fillRect(x*spacing+3,y*spacing+8,10,2,colour);
    tft.fillRect(x*spacing+1,y*spacing+9,2,2,colour);
    tft.fillRect(x*spacing+12,y*spacing+9,2,2,colour);
    tft.drawPixel(x*spacing+13,y*spacing+11,colour);
    tft.fillRect(x*spacing+5,y*spacing+12,6,2,colour);
    tft.fillRect(x*spacing+4,y*spacing+13,2,2,colour);
    tft.fillRect(x*spacing+3,y*spacing+14,2,2,colour);
    tft.fillRect(x*spacing+10,y*spacing+13,2,2,colour);
    tft.fillRect(x*spacing+11,y*spacing+14,2,2,colour);
    //Draw Belt
    tft.fillRect(x*spacing+5,y*spacing+10,6,2,0xCC61);
    tft.fillRect(x*spacing+7,y*spacing+10,2,2,0xCE58);
    }

  //if(Characters[i].type == 1 || Characters[i].type == 3 || Characters[i].type == 5){
    //tft.fillRect(x*spacing+13,y*spacing+10,2,2,0x8323);
    //tft.fillRect(x*spacing+13,y*spacing+5,2,3,0xBDD7);
    //tft.drawPixel(x*spacing+14,y*spacing+4,0xBDD7);
  //}


  
}


void drawHero(int x,int y) {
  //Colours, BLACK,BLUE,RED,GREEN,CYAN,MAGENTA,YELLOW,WHITE
  //tft.drawFastVLine(x,y,length, colour)
  //tft.drawFastHLine(x,y,length, colour)
  //tft.drawPixel(x*10,y*10,ILI9341_BLACK)
  //tft.drawRect(x,y,width,height,colour)
  //Hero facing left
  if(Characters[0].cDirection == 0) {
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
    tft.fillRect(x*spacing+6,y*spacing+10,2,2,ILI9341_YELLOW);
    
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
  }
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
    //Draw Sword
    tft.drawFastHLine(x*spacing+1,y*spacing+8,2,0xA000);
    tft.fillRect(x*spacing+1,y*spacing+11,2,2,0xA000);
    tft.fillRect(x*spacing+1,y*spacing+1,2,7,0xDEDB);
    //Draw Boots
    tft.fillRect(x*spacing+4,y*spacing+14,2,2,0xA3C0);
    tft.fillRect(x*spacing+10,y*spacing+14,2,2,0xA3C0);
    
  //Hero facing right  
  }
  else if(Characters[0].cDirection == 2) {
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
    tft.fillRect(x*spacing+8,y*spacing+10,2,2,ILI9341_YELLOW);
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
  }
  else {
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
    tft.fillRect(x*spacing+7,y*spacing+10,2,2,ILI9341_YELLOW);
    
    //Draw Sword
    tft.drawFastHLine(x*spacing+13,y*spacing+8,2,0xA000);
    tft.fillRect(x*spacing+13,y*spacing+11,2,2,0xA000);
    tft.fillRect(x*spacing+13,y*spacing+1,2,7,0xDEDB);
    //Draw Boots
    tft.fillRect(x*spacing+4,y*spacing+14,2,2,0xA3C0);
    tft.fillRect(x*spacing+10,y*spacing+14,2,2,0xA3C0);
  }
}
