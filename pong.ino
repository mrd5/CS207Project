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
const int WHEEL_ONE_PIN = 0;
const int WHEEL_TWO_PIN = 1;
const int BUTTON_ONE_PIN = 2;
const int BUTTON_TWO_PIN = 3;

//these will draw the game on the TV
int IN_GAMEA = 0; 
int IN_GAMEB = 0;

//modes
int IN_MENU = 1;
int GAME_OVER = 2;

//paddles
const int PADDLE_HEIGHT = 14;
const int PADDLE_WIDTH = 1;
#define RIGHT_PADDLE_X (TV.hres-4)
#define LEFT_PADDLE_X 2

//scores
#define LEFT_SCORE_X (TV.hres()/2-15)
#define RIGHT_SCORE_X (TV.hres()/2+10)
int SCORE_Y =  4;

//other. These are constants in Bruce's code -- I initialize to 0 and the users will choose the values
int PLAY_TO = 3;


#define LEFT 0
#define RIGHT 1

TVout TV; //create a TVout object called TV
unsigned char x,y;

bool button1Status = false;
bool button2Status = false;

//////////////

int sensorPos = 0;
int sensor2Pos = 0;

int leftPaddleHeight = 0;
int rightPaddleHeight = 0;

signed char ballX = 0;
signed char ballY = 0;

char ballVolX = 2;
char ballVolY = 2;

int leftPlayerScore = 0;
int rightPlayerScore = 0;

int frame = 0;
int state = IN_MENU;
int iSpeed = 10;

//This function gets the values of the sensors and buttons, which will be used to move 
//the paddles on screen. Also checks if it's a gameover. 
void HumanInputs()
{
	//sensorPos = analogRead(WHEEL_ONE_PIN);
	sensorPos = (analogRead(A0)) * (512/5) - 15360; // for temperature sensor
	
	//sensor2Pos = analogRead(WHEEL_TWO_PIN);
	if (leftPaddlyY <= ballY){
		sensor2Pos += 15;
	}else{
		sensor2Pos -= 15;
	}
	
	button1Status = digitalRead(BUTTON_ONE_PIN);
	
	if (state == GAME_OVER){
		Serial.println("Game over! Restarting...");
		drawMenu();
	}
}

/*
This is the code found on Bruce's website. I do not claim responsibility for this code.
I might try to recreate it before the project is due, but it is pretty complicated. 
*/

void drawMenu()
//This function draws the menu at the start of the game
{
  x = 0;
  y = 0;
  char volX =3;
  char volY = 3;
  TV.clear_screen();
  TV.select_font(font8x8);
  TV.print(10, 5, "Arduino Pong");
  TV.select_font(font4x6);
  TV.print(22, 35, "Press Button");
  TV.print(30, 45, "To Start");
  //The above lines print the words on screen
  
  delay(1000);
  while(!button1Status) {//The game starts when button 1 is pushed
    Serial.println("menu");
  Serial.println(button1Status);
  
    //While the game is in menu, a ball is drawn on screen and moves around the menu
    processInputs();
    TV.delay_frame(3);
    if(x + volX < 1 || x + volX > TV.hres() - 1) volX = -volX;
    if(y + volY < 1 || y + volY > TV.vres() - 1) volY = -volY;
    if(TV.get_pixel(x + volX, y + volY)) {
      TV.set_pixel(x + volX, y + volY, 0);
    
      if(TV.get_pixel(x + volX, y - volY) == 0) {
        volY = -volY;
      }
      else if(TV.get_pixel(x - volX, y + volY) == 0) {
        volX = -volX;
      }
      else {
        volX = -volX;
        volY = -volY;
      }
    }
    TV.set_pixel(x, y, 0);
    x += volX;
    y += volY;
    TV.set_pixel(x, y, 1);
  }
 
 
 
  TV.select_font(font4x6);
  state = IN_GAMEA;
}

//I do not claim responsibility for this code either. I don't think I will attempt to
//recreate this function on my own, as it seems incredibly complicated for me.
void drawGameScreen()
{
 //  TV.clear_screen();
  //draw right paddle
  rightPaddleY = ((wheelOnePosition /8) * (TV.vres()-PADDLE_HEIGHT))/ 128;
  x = RIGHT_PADDLE_X;
  for(int i=0; i<PADDLE_WIDTH; i++) {
    TV.draw_line(x+i,rightPaddleY,x+i,rightPaddleY+PADDLE_HEIGHT,1);
  }
 
  //draw left paddle
  leftPaddleY = ((wheelTwoPosition /8) * (TV.vres()-PADDLE_HEIGHT))/ 128;
  x = LEFT_PADDLE_X;
  for(int i=0; i<PADDLE_WIDTH; i++) {
    TV.draw_line(x+i,leftPaddleY,x+i,leftPaddleY+PADDLE_HEIGHT,1);
  }
 
  //draw score
  TV.print_char(LEFT_SCORE_X,SCORE_Y,'0'+leftPlayerScore);
  TV.print_char(RIGHT_SCORE_X,SCORE_Y,'0'+rightPlayerScore);
 
  
  
 
 
  
  //draw ball
  TV.set_pixel(ballX, ballY, 2);
}

void score(byte Player)
{
	
}

void setSpeed()
{
  while (digitalRead(BUTTON_ONE_PIN) == LOW){
    if (digitalRead(BUTTON_TWO_PIN) == HIGH){
      iSpeed--; //iSpeed set to some high value, and the lower it gets the faster the pong ball moves.
      delay(100);
    }
  }
}

//setSpeed is my own function. It lets the user choose the speed they would like to play to.
//While button 1 is not pushed, if button 2 is pushed, it decreases iSpeed
//iSpeed is an integer where the game updates on every iSpeed'th frame.
//Hence a lower iSpeed means the game updates more frequently, and plays faster,
//which is why iSpeed gets decreased instead of increased as you might think. 
void setSpeed()
{
  while (digitalRead(BUTTON_ONE_PIN) == LOW){
    if (digitalRead(BUTTON_TWO_PIN) == HIGH){
      iSpeed--;
      delay(1000);
      Serial.println(iSpeed);
    }
  }
}

//setScore is also a function I created. It functions very similar to setSpeed,
//where while button 1 is UP (not pushed), if you push button 2, the score you
//play to gets incremented by 1. 
void setScore()
{
  while (digitalRead(BUTTON_ONE_PIN) == LOW){
    if (digitalRead(BUTTON_TWO_PIN) == HIGH){
      PLAY_TO += 1;
      Serial.println(PLAY_TO);
    }
    delay(1000);
  }
}

	    




void setup()  {
  Serial.begin(9600);
  x=0;
  y=0;
  TV.begin(_NTSC);   //For north american televisions
 
  ballX = TV.hres() / 2; //ball starts in the middle of the screen
  ballY = TV.vres() / 2;


  Serial.println("Choose your game speed\n"); 
  setSpeed();//User chooses speed with this function
  delay(1000);
  Serial.println("Choose the score you want to play to\n");
  setScore();//User sets play-to score with this function
  delay(1000);

  if (iSpeed <= 0){ //If iSpeed is decremented to low, it is set to 1, which is the fastest speed.
    iSpeed = 1;
    Serial.print("New speed: ");//Used these two lines just to make sure it was working properly.
    Serial.println(iSpeed);
    delay(1000);
  }
}

void loop()
{
   processInputs();	
	
   if(state == IN_MENU) { //Draws the menu 
      Serial.println("F"); //To show you're in the menu
      drawMenu();
  }

    if(state == IN_GAMEA) { //Draws the game box at the start of the game
    drawBox();
  }
}








