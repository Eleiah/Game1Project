# Game1Project
this a gam

this is a dac guide: http://www.instructables.com/id/Arduino-Audio-Output/


void bossMechanics(int mechanicNum) {
  
  if(mechanicNum == 0){
    if(random(bossLavaRate) == 0 && Characters[0].cTile == 2) {
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
        if(wMap[wRow][wCol][i][j] == 2  && random(bossLavaRate) == 0  ) {
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
  else if(mechanicNum == 2) {
    for(int i = 2;i<5;i++) {
      if(wMap[wRow][wCol][4][i] != 42) {
        if(wMap[wRow][wCol][4][i] == 2) {
          wMap[wRow][wCol][4][i] = 50;
          drawTile(4*spacing,i*spacing,wMap[wRow][wCol][4][i]);
        }
        else if(wMap[wRow][wCol][4][i] < 52) {
          wMap[wRow][wCol][4][i]++;
          drawTile(4*spacing,i*spacing,wMap[wRow][wCol][4][i]);
        }
        else if(wMap[wRow][wCol][4][i] == 52) {
          wMap[wRow][wCol][4][i] = 2;
          drawTile(4*spacing,i*spacing,wMap[wRow][wCol][4][i]);
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
          drawTile(10*spacing,i*spacing,wMap[wRow][wCol][10][i]);
        }
        else if(wMap[wRow][wCol][10][i] < 52) {
          wMap[wRow][wCol][10][i]++;
          drawTile(10*spacing,i*spacing,wMap[wRow][wCol][10][i]);
        }
        else if(wMap[wRow][wCol][10][i] == 52) {
          wMap[wRow][wCol][10][i] = 2;
          drawTile(10*spacing,i*spacing,wMap[wRow][wCol][10][i]);
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
          drawTile(i*spacing,4*spacing,wMap[wRow][wCol][i][4]);
        }
        else if(wMap[wRow][wCol][i][4] < 52) {
          wMap[wRow][wCol][i][4]++;
          drawTile(i*spacing,4*spacing,wMap[wRow][wCol][i][4]);
        }
        else if(wMap[wRow][wCol][i][4] == 52) {
          wMap[wRow][wCol][i][4] = 2;
          drawTile(i*spacing,4*spacing,wMap[wRow][wCol][i][4]);
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
