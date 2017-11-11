/*
 * Pong game on the Arduino
 * Based on James Bruce's version found here: 
 * http://www.makeuseof.com/tag/how-to-recreate-the-classic-pong-game-using-arduino/
 * enhanced, by Mathew Dorish
 * Uses wither photoresistors or temperature sensors as controllers! 
 * Plays on a TV.
 */

//Required arduino libraries
#include <TVout.h>
//Download TVout here: https://code.google.com/archive/p/arduino-tvout/downloads
//Download the TVoutBeta1.zip and extract it. Place the three folders in it in your Arduino libraries folder. 
#include <fontALL.h>
//fontALL.h is included in the above download

//VARIABLES

//potentiometer and button pins
const int wheel_one = 0;
const int wheel_one = 1;
const int button_one = 2;
const int button_two = 3;

//these will draw the game on the TV
int in_game1 = 0; 
int in_game2 = 0;

//modes
int menu = 1;
int gameOver = 2;

//paddles
int paddleHeight = 14;
int paddleWidth = 1;
#define rightPaddle (TV.hres-4)
#define leftPaddle 2

//scores
#define leftScore (TV.hres()/2-15)
#define rightScore (TV.hres()/2+10)
#define scoreY 4

//other. These are constants in Bruce's code -- I initialize to 0 and the users will choose the values
int velocity = 0;
int playTo = 0;

#define left 0
#define right 1

TVout TV; //create a TVout object called TV
unsigned char a,b;

bool button1 = false;
bool button2 = false;

//////////////

int potentiometerPos = 0;
int potentiometer2Pos = 0;

int leftPaddleHeight = 0;
int rightPaddleHeight = 0;

signed char ballXpos, ballYpos;
ballXpos = 0; ballYpos = 0;

char ballHoriVelo = 0;
char ballVertVelo = 0;

int frame = 0;
int mode = menu;

void HumanInputs()
{
	//Gets potentiometer values
	potentimeterPos = analogRead(wheel_one);
	potentiometer2Pos = analogRead(wheel_two);
	
	//Gets the pushbutton values
	button1 = digitalRead(button_one);
	button2 = digitalRead(button_two);
	

}

void drawMenu()
{

}
void drawTheGame()
{

}

void score(byte Player)
{
	
}



	    
