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

//Character objects for Hero and Enemies
class CHARACTER
{
  public:
  int health;
  int posX;
  int posY;
  int cDirection;
  int room;
  bool cStatus;
  int type;
  void properties(int _type,int _posX,int _posY,int _cDirection,int _room,int _health,bool _cStatus)
  {
    type = _type;
    posX = _posX;
    posY = _posY;
    cDirection = _cDirection;
    room = _room;
    health = _health;
    cStatus = _cStatus;
  }
};

//Spacing sets the size of each block, 16x16 pixels
int spacing = 16;
int wMap[3][3][15][15] = {};
bool dPad[4];
int centre[2] = {7, 7};
int origin = 3;
//Initialize array or character objects
CHARACTER Characters[20];
//Boolean statments to make sure the button press is only registered once
bool moveState = digitalRead(26);
bool lastMoveState = digitalRead(26);
bool attackState = digitalRead(27);
bool lastAttackState = digitalRead(27);

void setup() {
  //Character 0 is the Hero
  Characters[0].properties(0,2,2,3,0,3,1);
  //All other characters are enemies
  Characters[1].properties(1,13,13,0,0,3,1);
  Characters[2].properties(1,13,2,0,0,3,1);
  Characters[3].properties(1,2,13,0,0,3,1);
  //Initialize pins for Dpad and buttons
  for(int i = 22;i<28;i++){
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
  //Initialize the screen
  tft.begin();
  //Set the screen to landscape and clear it
  tft.setRotation(3);
  tft.fillScreen(ILI9340_BLACK);
  initializeMap();
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
void drawMapMiddle(){
  int poop[15][15]= {{1,1,1,1,1,1,0,1,1,1,1,1,1,1,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,1,1,1,1,1,1,0,1,1,1,1,1,1,1}};
}
//Place the character after a map change
void placeChar() {
  //Remove Hero from current spot
  deleteChunk(Characters[0].posX,Characters[0].posY);
  wMap[0][0][Characters[0].posX][Characters[0].posY] = 0;
  //Check where he should be placed
   switch(Characters[0].cDirection) {
      case 0: Characters[0].posX = 14;
         Characters[0].posY = 7;
         break;
      case 1: Characters[0].posX = 7;
         Characters[0].posY = 14;
         break;
      case 2: Characters[0].posX = 0;
         Characters[0].posY = 7;
         break;
      case 3: Characters[0].posX = 7;
         Characters[0].posY = 0;
         break;
   }
   //Place the Hero on the map
   wMap[0][0][Characters[0].posX][Characters[0].posY] = 2;
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
  if(wMap[0][0][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] == 0) {
      wMap[0][0][Characters[0].posX][Characters[0].posY] = 0;
      wMap[0][0][Characters[0].posX + newPos[0]][Characters[0].posY + newPos[1]] = 2;
      deleteChunk(Characters[0].posX,Characters[0].posY);
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
  if(wMap[0][0][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]] >= 100){
    int currentEnemy = wMap[0][0][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]]-100;
    Characters[currentEnemy].health--;
    if(Characters[currentEnemy].health <= 0){
      Characters[currentEnemy].cStatus = 0;
      wMap[0][0][Characters[0].posX + attacks[0]][Characters[0].posY + attacks[1]] = 0;
      deleteChunk(Characters[currentEnemy].posX,Characters[currentEnemy].posY);
    }
  }
}
void enemiesMove(){
  for(int i = 1;i<4;i++){
    if(Characters[i].cStatus == 1 && Characters[i].room == Characters[0].room)
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
            row = Characters[i].posX + (rand()%3 - 1);
            column = Characters[i].posY;
         }
         else {
            row = Characters[i].posX;
            column = Characters[i].posY + (rand()%3 - 1);
         }
      }while(wMap[0][0][row][column] == 1);
   }
   else if(charDistance(Characters[i].posX, Characters[i].posY) == 1) {
      //Attack!
   }
   else {
      if((Characters[0].posX - Characters[i].posX) != 0)
         moveDirection[0] = (Characters[0].posX - Characters[i].posX)/abs(Characters[0].posX - Characters[i].posX);
      if((Characters[0].posY - Characters[i].posY) != 0)
         moveDirection[1] = (Characters[0].posY - Characters[i].posY)/abs(Characters[0].posY - Characters[i].posY);
      for(int i = 0; i < 2; i++)
         moves[i] = moveDirection[i];
      if(abs(Characters[0].posX - Characters[i].posX) >= abs(Characters[0].posY - Characters[i].posY)) {
         if(wMap[0][0][Characters[i].posX + moves[0]][Characters[i].posY] == 0 && moves[0] != 0)
            row += moves[0];
         else if(wMap[0][0][Characters[i].posX][Characters[i].posY + moves[1]] == 0 && moves[1] != 0)
            column += moves[1];
      }
      else if(moves[0] != 0 || moves[1] != 0) {
         if(wMap[0][0][Characters[i].posX][Characters[i].posY + moves[1]] == 0 && moves[1] != 0)
            column += moves[1];
         else if(wMap[0][0][Characters[i].posX + moves[0]][Characters[i].posY] == 0 && moves[0] != 0)
            row += moves[0];
      }
      if (row == Characters[i].posX && column == Characters[i].posY) {
         if(Characters[i].posX < centre[0] && Characters[0].posX < centre[0] && wMap[0][0][Characters[i].posX-1][Characters[i].posY] == 0)
            row--;
         else if(Characters[i].posX > centre[0] && Characters[0].posX > centre[0] && wMap[0][0][Characters[i].posX+1][Characters[i].posY] == 0)
            row++;
         else if(Characters[i].posY < centre[1] && Characters[0].posY < centre[1] && wMap[0][0][Characters[i].posX][Characters[i].posY-1] == 0)
            column--;
         else if(Characters[i].posY > centre[1] && Characters[0].posY > centre[1] && wMap[0][0][Characters[i].posX][Characters[i].posY+1] == 0)
            column++;
      }
   }
   deleteChunk(Characters[i].posX, Characters[i].posY);
   wMap[0][0][Characters[i].posX][Characters[i].posY] = 0;
   wMap[0][0][row][column] = 100+i;
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
      deleteChunk(Characters[0].posX,Characters[0].posY);
      drawHero(Characters[0].posX,Characters[0].posY);
      
    }
  }
}



void deleteChunk(int x,int y) {
  tft.fillRect(x*spacing,y*spacing,16,16,ILI9340_BLACK);
}

void initializeMap() {
  for(int i = 0; i < 15; i++) {
    wMap[0][0][0][i] = 1;
    wMap[0][0][14][i] = 1;
    wMap[0][0][i][0] = 1;
    wMap[0][0][i][14] = 1;
  }
  wMap[0][0][0][7] = 0;
  wMap[0][0][14][7] = 0;
  wMap[0][0][7][0] = 0;
  wMap[0][0][7][14] = 0;
  for(int i = 5; i < 10; i++) {
      wMap[0][0][i][7] = 1;
      wMap[0][0][7][i] = 1;
   }
   wMap[0][0][6][6] = 1;
   wMap[0][0][6][8] = 1;
   wMap[0][0][8][6] = 1;
   wMap[0][0][8][8] = 1;
   wMap[0][0][4][4] = 1;
   wMap[0][0][4][10] = 1;
   wMap[0][0][10][4] = 1;
   wMap[0][0][10][10] = 1;
}

void drawMap() {
  for(int i = 0;i<15;i++) {
    for(int j = 0;j<15;j++) {
      drawTile(i*spacing,j*spacing,wMap[0][0][i][j]);
    }
  }
}

void drawTile(int xPos,int yPos,int tileType) {
  if(tileType == 1) {
    tft.fillRect(xPos,yPos,spacing,spacing,0x8430);//Darker
    tft.fillRect(xPos+1,yPos+1,4,4,0xAD55);//Lighter
    tft.fillRect(xPos+7,yPos+1,3,3,0xAD55);
    tft.fillRect(xPos+12,yPos+1,3,3,0xAD55);
    tft.fillRect(xPos+1,yPos+7,6,3,0xAD55);
    tft.fillRect(xPos+9,yPos+6,6,3,0xAD55);
    tft.fillRect(xPos+1,yPos+12,8,3,0xAD55);
    tft.fillRect(xPos+11,yPos+11,4,4,0xAD55);
  }
  if(tileType == 4) {
    //draw a door
  }
  //Lava
  if(tileType == 8){
    //0x6B6D Darkest
    //0xFD42 Dark
    //0xFB40 Light
    tft.fillRect(xPos,yPos,16,16,0x6B6D);
    tft.fillRect(xPos,yPos,2,4,0xFD42);
    tft.fillRect(xPos+4,yPos,8,2,0xFD42);
    tft.fillRect(xPos+6,yPos+2,4,2,0xFD42);
    tft.fillRect(xPos+2,yPos+4,4,8,0xFD42);
    tft.fillRect(xPos,yPos+6,8,4,0xFD42);
    tft.fillRect(xPos+10,yPos+4,4,4,0xFD42);
    tft.fillRect(xPos+14,yPos,2,4,0xFD42);
    tft.fillRect(xPos+12,yPos+10,4,4,0xFD42);
    tft.fillRect(xPos+6,yPos+12,4,2,0xFD42);
    tft.fillRect(xPos+4,yPos+14,8,2,0xFD42);
  }
  //Wood Planks
  if(tileType == 9){
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
}

void drawEnemy(int i){
  tft.fillRect(Characters[i].posX*spacing,Characters[i].posY*spacing,16,16,ILI9340_MAGENTA);
}

void drawHero(int x,int y) {
  //Colours, BLACK,BLUE,RED,GREEN,CYAN,MAGENTA,YELLOW,WHITE
  //tft.drawFastVLine(x,y,length, colour)
  //tft.drawFastHLine(x,y,length, colour)
  //tft.drawPixel(x*10,y*10,ILI9340_BLACK)
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
