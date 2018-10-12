# Cave of Imps

For this project we set out to make an Arduino controlled, turn-based video game. This project’s primary hardware consists of an Arduino microcontroller (we used a due and an Adafruit Feather M0 Basic Proto, but any Arduino-compatible microcontroller should work), three push-buttons, four limit switches and a 2.2” 18-bit colour TFT LCD, wired using jumper wires and a breadboard. Inspiration for the game came from the  Legend of Zelda franchise and the Rogue-like genre.
The game is set in a dungeon consisting of 9 rooms, in the shape of a square. Each area (left, right and centre) of the dungeon has a unique theme. The dungeon has a variety of enemies, breakable objects, weapons, traps, and possibly a few secrets. The left side of the display contains the playable space, while the right side displays the player character’s health, and has space for an inventory and a world map. The game is turn based, but it could be changed to real-time through the use of timers.
This project could be used as a basis for a variety of video games, including Rogue-likes, Zelda-like games, RPGs, and more.  These are all possible with the current code, by adjusting the world map matrix and enemy positions.

Our code makes use of Adafruit's ILI9340 Graphics Library, available here: https://github.com/adafruit/Adafruit_ILI9340

# Old Block Numbering System:

>0 - floor

>1 - wall 1

>2 - wall 2

>3 - wall 3

>4 - hero

>5 - switch

>6 - traps

>7 - water

>8 - lava

>9 - planks

>'>100 - enemies



# Current Block Numbering System:

Walkable:

  >0 - floor
	
  >1 - planks
	
  >2 - boss floor
	
  >3 - traps
	
  >4 - boss lava
	
  >5 - hearts
	
  >6 - heart containers
	
  >7 - items
	
  >8 - 
	
  >9 - doors


Static Unwalkable:
	
  >10 - wall 1 
	
  >11 - wall 2

  >12 - wall 3
	
  >13 - wall 4
	
  >14 - water
	
  >15 - lava
	
  >16 - 
	
  >17 - switches
	
  >18 - pots
	
  >19 - chests


Characters:
  >42 - Player

  >'>100 - enemies
