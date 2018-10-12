#include <iostream>
#include <ctime>
#include <cmath>
using namespace std;

void printMap(int[][15]);
void createMap(int[][15]);
void placeChar(int[][15],int[][3]);
void placeEnemy(int[][15],int[][3]);
void moveChar(int[][15],char,int[][3]);
void enemyMove(int[][15],int[][3]);
float distance(int, int);
int charPosition[15][3] = {};

const int enemyCount = 1;
int origin = 3;
int numChars = 0;

int lastMoves[enemyCount] = {0};

int main()
{
   int map[15][15] = {
      {1,1,1,1,1,1,1,0,1,1,1,1,1,1,1}, //L Room 2
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,0,0,0,0,1,1,1,1,1,1,1,0,0,1},
      {1,0,0,0,0,0,1,1,1,1,1,1,1,0,1},
      {1,0,0,1,1,0,0,1,1,1,1,1,1,0,1},
      {1,0,1,1,1,1,0,0,1,1,1,1,1,0,1},
      {1,1,1,1,1,1,1,0,0,1,0,1,1,0,1},
      {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
      {1,1,1,1,1,1,1,0,0,1,0,1,1,0,1},
      {1,0,1,1,1,1,0,0,1,1,1,1,1,0,1},
      {1,0,0,1,1,0,0,1,1,1,1,1,1,0,1},
      {1,0,0,0,0,0,1,1,1,1,1,1,1,0,1},
      {1,0,0,0,0,1,1,1,1,1,1,1,0,0,1},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
   };
   char move;
   srand(time(0));
   createMap(map);
   placeChar(map,charPosition);
   placeEnemy(map,charPosition);
   printMap(map);
   do
   {
      cin >> move;
      cout << endl;
      moveChar(map,move,charPosition);
      enemyMove(map,charPosition);
      printMap(map);
   }while(move == 'a' ||move == 's' ||move == 'd' ||move == 'w');
}
//Creates Map
void createMap(int map[][15])
{
   map[0][0] = 1;
   //Initalize the floor
      /*
   for(int i = 0;i<15;i++)
   {
      for(int j = 0;j<15;j++)
      {
         map[i][j] = 0;
      }
   }
   //Create outer walls

   for(int i = 0;i<1;i++)
   {
      for(int j = 0;j<15;j++)
      {
         map[i][j] = 1;
         map[i+14][j] = 1;
         map[j][0] = 1;
         map[j][14] = 1;
      }
   }
   */
   //Create walls in the center // For a different room
   /*for(int i = 5; i < 10; i++){
      map[i][7] = 1;
      map[7][i] = 1;
   }*/
   /*
   for (int i = 3; i<12; i++) // left volume
      map[i][1] = 1;
   for (int i = 4; i<11; i++)
      map[i][2] = 1;
   for (int i = 5; i<10; i++)
      map[i][3] = 1;
   for (int i = 6; i<9; i++)
      map[i][4] = 1;
   map[3][9] = 1; // upper right volume
   map[4][8] = 1;
   map[4][9] = 1;
   map[4][10] = 1;
   map[5][9] = 1;
   map[9][9] = 1; // lower right volume
   map[10][8] = 1;
   map[10][9] = 1;
   map[10][10] = 1;
   map[11][9] = 1;
   */
}
//Prints full map
void printMap(int map[][15])
{
   for(int i = 0;i<15;i++)
   {
      for(int j = 0;j<15;j++)
      {
         cout << map[i][j] << " ";
      }
      cout << endl;
   }
   cout << endl;
}
//Places the character in the appropriate doorway
void placeChar(int map[][15],int charPosition[][3])
{
   charPosition[numChars][0] = 1;
   switch(origin) {
      case 0: charPosition[numChars][1] = 14;
         charPosition[numChars][2] = 7;
         break;
      case 1: charPosition[numChars][1] = 7;
         charPosition[numChars][2] = 1;
         break;
      case 2: charPosition[numChars][1] = 1;
         charPosition[numChars][2] = 7;
         break;
      case 3: charPosition[numChars][1] = 7;
         charPosition[numChars][2] = 14;
         break;
   }
   map[charPosition[numChars][1]][charPosition[numChars][2]] = 5;
   numChars++;
}
//Place Enemy
void placeEnemy(int map[][15],int charPosition[][3])
{
   int row,column;
   //Keep placing the enemy until he is placed in an empty location
   for(int i = 0; i < enemyCount; i++) {
      do
      {
         row =1+rand()%13;
         column =1+rand()%13;
      }while(map[row][column] != 0);
      map[row][column] = 6;
      //Store enemy's coordinates
      charPosition[numChars][0] = 2;
      charPosition[numChars][1] = row;
      charPosition[numChars][2] = column;
      numChars++;
   }
}
//Character movement - in arduino!
void moveChar(int map[][15], char move,int charPosition[][3])
{
   int currentI,currentJ;
   currentI = charPosition[0][1];
   currentJ = charPosition[0][2];
   int newPos[2] = {};
   if(move == 'a')
   {
      newPos[1] = -1;
   }
   else if(move == 'd')
   {
      newPos[1] = 1;
   }
   else if(move == 's')
   {
      newPos[0] = 1;
   }
   else if(move == 'w')
   {
      newPos[0] = -1;
   }
   if(map[currentI + newPos[0]][currentJ + newPos[1]] == 0) { 
      map[currentI][currentJ] = 0;
      map[currentI + newPos[0]][currentJ + newPos[1]] = 5;
      charPosition[0][1] = currentI + newPos[0];
      charPosition[0][2] = currentJ + newPos[1];
   }
   else if(map[currentI + newPos[0]][currentJ + newPos[1]] == 6) {
      //Attack!
   }
}

void enemyMove(int map[][15],int charPosition[][3])
{
   for(int i = 1; i < 1 + enemyCount; i++) {
      int row, column, currentI, currentJ, currentMove;
      // float moveDirection[2] = {};
      // int moves[2] = {};
      currentI = charPosition[i][1];
      currentJ = charPosition[i][2];
      row = currentI;
      column = currentJ;
      currentMove = 0;
      if(sqrt(pow((currentI - charPosition[0][1]),2) + pow((currentJ - charPosition[0][2]),2)) > 8)
      {
         do
         {
            if(rand()%2 == 0)
            {
               row = currentI + (rand()%3 - 1);
               column = currentJ;
            }else
            {
               row = currentI;
               column = currentJ + (rand()%3 - 1);
            }
         }while(map[row][column] == 1);
      }
      else if(sqrt(pow((currentI - charPosition[0][1]),2) + pow((currentJ - charPosition[0][2]),2)) == 1) {
         //Attack!
      }
      else
      {
         float minDist = 100;
         if (distance(currentI - 1, currentJ) < minDist && map[currentI - 1][currentJ] == 0 && (lastMoves[i - 1] != 1)) {
            minDist = sqrt(pow(((currentI - 1) - charPosition[0][1]),2) + pow(currentJ - charPosition[0][2],2));
            row = currentI - 1;
            currentMove = 2;
         }
         if (distance(currentI + 1, currentJ) < minDist && map[currentI + 1][currentJ] == 0 && (lastMoves[i - 1] != 2)) {
            minDist = sqrt(pow(((currentI + 1) - charPosition[0][1]),2) + pow(currentJ - charPosition[0][2],2));
            row = currentI + 1;
            currentMove = 1;
         }
         if (distance(currentI, currentJ - 1) < minDist && map[currentI][currentJ - 1] == 0 && (lastMoves[i - 1] != 3)) {
            minDist = sqrt(pow((currentI - charPosition[0][1]),2) + pow((currentJ - 1) - charPosition[0][2],2));
            row = currentI;
            column = currentJ - 1;
            currentMove = 4;
         }
         if (distance(currentI, currentJ + 1) < minDist && map[currentI][currentJ + 1] == 0 && (lastMoves[i - 1] != 4)) {
            minDist = sqrt(pow((currentI - charPosition[0][1]),2) + pow((currentJ + 1) - charPosition[0][2],2));
            row = currentI;
            column = currentJ + 1;
            currentMove = 3;
         }
         /*
         if((charPosition[0][1] - charPosition[i][1]) != 0)
            moveDirection[0] = (charPosition[0][1] - charPosition[i][1])/abs(charPosition[0][1] - charPosition[i][1]);
         if((charPosition[0][2] - charPosition[i][2]) != 0)
            moveDirection[1] = (charPosition[0][2] - charPosition[i][2])/abs(charPosition[0][2] - charPosition[i][2]);
            for(int i = 0; i < 2; i++)
            moves[i] = moveDirection[i];
         if(abs(charPosition[0][1] - charPosition[i][1]) >= abs(charPosition[0][2] - charPosition[i][2])) {
            if(map[currentI + moves[0]][currentJ] == 0 && moves[0] != 0)
               row += moves[0];
            else if(map[currentI][currentJ + moves[1]] == 0 && moves[1] != 0)
               column += moves[1];
         }
         else if(moves[0] != 0 || moves[1] != 0) {
            if(map[currentI][currentJ + moves[1]] == 0 && moves[1] != 0)
               column += moves[1];
            else if(map[currentI + moves[0]][currentJ] == 0 && moves[0] != 0)
               row += moves[0];
         }
         if (row == currentI && column == currentJ) {
            for (int i = 1; i < 5; i++) {
               if (map[currentI + moves[0]][currentJ] == i || map[currentI][currentJ + moves[1]] == i) {
                  if(currentI <= centre[i-1][0] && charPosition[0][1] <= centre[i-1][0] && map[currentI-1][currentJ] == 0)
                     row--;
                  else if(currentI >= centre[i-1][0] && charPosition[0][1] >= centre[i-1][0] && map[currentI+1][currentJ] == 0)
                     row++;
                  else if(currentJ <= centre[i-1][1] && charPosition[0][2] <= centre[i-1][1] && map[currentI][currentJ-1] == 0)
                     column--;
                  else if(currentJ >= centre[i-1][1] && charPosition[0][2] >= centre[i-1][1] && map[currentI][currentJ+1] == 0)
                     column++;
               }
            }
         }
         */
         /*if (row == currentI && column == currentJ) {  // Alternative pathing method
            if (moves[0] == 0 && moves[1] != 0) {
               int rand = rand%2;
               if (rand == 0)
                  rand = -1;
               else
                  rand = 1;
               row+=rand;
            }
            if (moves[0] != 0 && moves[1] == 0) {
               int rand = rand%2;
               if (rand == 0)
                  rand = -1;
               else
                  rand = 1;
               column+=rand;
            }
         }*/
      }
      lastMoves[i - 1] = currentMove;
      map[currentI][currentJ] = 0;
      map[row][column] = 6;
      charPosition[i][1] = row;
      charPosition[i][2] = column;
   }
}

float distance(int I, int J) {
   return sqrt(pow((I - charPosition[0][1]),2) + pow(J - charPosition[0][2],2));
}
