/********
* Arduino Pong
* By Pete Lamonica
*  modified by duboisvb
*  updated by James Bruce (http://www.makeuseof.com/tag/author/jbruce
* A simple implementation of Pong on the Arduino using a TV for output.
*
*/


/*
This code has been modified heavily by Mathew Dorish for his CS 207 project.
He has allowed the game to play with two additional types of sensors:
photoresistors and temperature sensors.
Also, in this version, either of the two players can be played by a computer,
so the paddle moves on its own. This version and build also work with sound,
and the users can choose the speed of the ball, as well as the score they would like
to play to.
*/


//Download these two libraries here: https://code.google.com/archive/p/arduino-tvout/downloads
//Download the TVoutBeta1.zip and extract it. Place the three folders in it in your Arduino libraries folder.
#include <TVout.h>
#include <fontALL.h>


//Variables

//Player buttons
const int button1Pin = 2; //digital to start game
const int button2Pin = 3; //digital to reset and go back to main menu


//Paddle definitions
const int paddleHeight = 14;
const int paddleWidth = 1;

#define rightPaddleXcoord (TV.hres()-4)
const int leftPaddleX = 2;


//Game modes
const int modeDrawStationary = 0;
//Gamea draws the game box such as the walls, and the middle line

const int modeDrawMoving = 0;
//Gameb draws everything on screen that moves, the ball and paddles

const int modeMenu = 1;
//in menu at the start of the game before the start button is pushed

const int modeGameOver = 2; //game over state
//After someone has won

//Left and Right score x are used to draw each players score
//Based on the size of the television played on
#define leftPlayerScoreX (TV.hres()/2-15)
#define rightPlayerScoreX (TV.hres()/2+10)

//Height of where the score will be drawn.
const int scoreDisplayHeight = 4;

//Max speed of the ball
const int maxBallSpeed = 6;

//Score the users play to, which they can change at the start of the game
int playToScore = 3;

//These two variables are used to choose which sensor (or computer) the user will play with
//0 plays with potentiometer, 1 for photoresistor, 2 for temperature sensor, 3 plays as a computer by itself
int player1Sensor = 0;
int player2Sensor = 0;

//These are used to determine which player scored.
#define LEFT 0
#define RIGHT 1

//Creates a new tv object to use the tvout functions
TVout TV;

//Used to store the paddle x values and y values when drawing the paddles
unsigned char x, y;

boolean button1Status = false;
// boolean button2Status = false;

//Wheel positions will be used to store the sensor values after it reads them, which will move the paddles up and down
int wheelOnePosition = 0;
int wheelTwoPosition = 0;

//Height of the paddles. Can't go above or below a certain value
int rightPaddleY = 0;
int leftPaddleY = 0;

//Coordinates of the ball moving around the screen.
unsigned char ballX = 0;
unsigned char ballY = 0;

//Speed of the ball
char ballVolX = 2;
char ballVolY = 2;

//Both players start at a score of zero
int leftPlayerScore = 0;
int rightPlayerScore = 0;

//Game updates ever iSpeed frames
int frame = 0;
int iSpeed = 8;

//Used to count number of button pushes
int prevButtonStatus = LOW;
int currButtonStatus = LOW;

//Game starts in menu, after setup is finished
int state = modeMenu;

//processInputs() reads values from the sensors, which will then be used to move the paddles to the corresponding height.
//Values from the sensors will range from 0 to 1023. The higher the value being read from the sensor, the lower the paddle moves.
//So a value of 0 will have the paddle at the top of the screen, and a value of 1023 will move the paddle to the bottom of the screen.
void processInputs() {
  if (player1Sensor == 0) {//Player 1 has chosen potentiometer
    wheelOnePosition = analogRead(A3);
  }
  else if (player1Sensor == 1) {//Player 1 has chosen a photoresistor
    wheelOnePosition = analogRead(A5) * 5 - 3635;
  }
  else if (player1Sensor == 2) {//Player 1 has chosen a temperature sensor
    wheelOnePosition = analogRead(A4) * 102 - 15360;
  }
  else {//Player 1 has chosen to be a computer
    if (leftPaddleY <= ballY) {
      wheelOnePosition += 15;
    }
    else {
      wheelOnePosition -= 15;
    }
    //Next to if statements are if the paddle tries going off screen. It will be placed back on screen
    if (wheelOnePosition < 0) {
      wheelOnePosition = 50;
    }
    if (wheelOnePosition > 1023) {
      wheelOnePosition = 1000;
    }
  }


  if (player2Sensor == 0) {//Player 2 has chosen a potentiometer
    wheelTwoPosition = analogRead(A0);
  }
  else if (player2Sensor == 1) {//Player 2 has chosen a photoresistor
    wheelTwoPosition = analogRead(A2) * 5 - 3635;
  }
  else if (player2Sensor == 2) {//Player 2 has chosen a temperature sensor
    wheelTwoPosition = analogRead(A1) * 102 - 15360;
  }
  else {//Player 2 has chosen to be a computer
    if (rightPaddleY <= ballY) {
      wheelTwoPosition += 15;
    }
    else {
      wheelTwoPosition -= 15;
    }

    //If paddle goes too high or low:
    if (wheelTwoPosition < 0) {
      wheelTwoPosition = 50;
    }
    if (wheelTwoPosition > 1023) {
      wheelTwoPosition = 1000;
    }
  }

  if ((digitalRead(button1Pin) == LOW) && (state == modeGameOver))
  {
    //calling drawMenu() in the next function is causing an error, which gives no explanation.
    //drawMenu();
  }
  delay(50);
}


//This code is the same as the code from the original project. The ddraw_line fuctions are fairly complicated. I do not take credit for this! 
//All I changed/added were the comments to make them easier to understand.
void drawGameScreen() {


  //These next few lines of code draw the right paddle
  rightPaddleY = ((wheelOnePosition / 8) * (TV.vres() - paddleHeight)) / 128;
  x = rightPaddleXcoord;
  for (int i = 0; i<paddleWidth; i++) {//Draws each vertical line for each pixel the paddle is wide
    TV.draw_line(x + i, rightPaddleY, x + i, rightPaddleY + paddleHeight, 1);
  }

  //These lines draw the left paddle
  leftPaddleY = ((wheelTwoPosition / 8) * (TV.vres() - paddleHeight)) / 128;
  x = leftPaddleX;
  for (int i = 0; i<paddleWidth; i++) {//Draws each vertical line for each pixel the paddle is wide
    TV.draw_line(x + i, leftPaddleY, x + i, leftPaddleY + paddleHeight, 1);
  }

  //Draws score of left player
  TV.print_char(leftPlayerScoreX, scoreDisplayHeight, '0' + leftPlayerScore);

  //Draws score of right player
  TV.print_char(rightPlayerScoreX, scoreDisplayHeight, '0' + rightPlayerScore);






  //draw ball
  TV.set_pixel(ballX, ballY, 2);
}

//This was part of the original code however I recreated it myself and added the comments.
void playerScored(byte player) {
  //if Left player scores
  if (player == LEFT) {
    leftPlayerScore += 1;
  }

  //else Right player scores
  else {
    rightPlayerScore += 1;
  }

  //Checks if anyone wins
  if (leftPlayerScore == playToScore || rightPlayerScore == playToScore) {
    state = modeGameOver;//If someone wins game is done
  }

  //Ball will now switch directions since when a player scores the ball will be hitting a left or right wall
  ballVolX = -ballVolX;
}


//This function draws all the stationary parts of the game, the four walls and the middle dotted line.
//I do NOT take credit for this function. I only added/changed the comments to make it easy to understand what is going on.
void drawBox() {
  TV.clear_screen();//Clears the screen if anything was originally on it.

            //draws the dotted line
  for (int i = 1; i<TV.vres() - 4; i += 6) {
    TV.draw_line(TV.hres() / 2, i, TV.hres() / 2, i + 3, 1);
  }

  //Draws the four walls
  TV.draw_line(0, 0, 0, 95, 1);  // left wall
  TV.draw_line(0, 0, 126, 0, 1); // top wall
  TV.draw_line(126, 0, 126, 95, 1); // right wall
  TV.draw_line(0, 95, 126, 95, 1); // bottom wall


  state = modeDrawMoving;//Now move on to drawing the moving paddles and ball.
}

//I do NOT take credit for this function, it was in the original code.
//I did however make all of the comments, to make sure everyone knows what is going on.
//I also formatted it nicer. 
void drawMenu() {
  x = 0;
  y = 0;
  char volX = 3;
  char volY = 3;

  TV.clear_screen();//Clears the screen if anything is on it
  TV.select_font(font8x8);//Chooses a font size
  TV.print(10, 5, "Arduino Pong");//Draws "Arduino Pong Mathew Dorish" on the screen at a certain position
  TV.select_font(font4x6); //Then draws "Press Button To Start" in a smaller font
  TV.print(22, 35, "Press Button");
  TV.print(30, 45, "To Start");


  delay(1000);

  while (button1Status == LOW) {
    //Serial.println("menu");
    //Serial.println(button1Status);

    //This next part has the ball and paddles moving while on the menu. 
    //It is not actually playing the game yet, but the ball will still move and the paddles will too.
    //The ball also bounced off of the text that is printed several lines above.
    processInputs();
    TV.delay_frame(3);
    if (x + volX < 1 || x + volX > TV.hres() - 1) {//If the ball has hit a left or right wall it changes direction
      volX = -volX;
    }
    if (y + volY < 1 || y + volY > TV.vres() - 1) {//If the ball hits a top or bottom wall it changes direction
      volY = -volY;
    }
    if (TV.get_pixel(x + volX, y + volY)) {//Next few lines are if the ball hits the text on the screen, it will change directions based off where it hits
      TV.set_pixel(x + volX, y + volY, 0);
      if (TV.get_pixel(x + volX, y - volY) == 0) {//If a top or bottom wall is hit
        volY = -volY;
      }
      else if (TV.get_pixel(x - volX, y + volY) == 0) {//If a left or right wall is hit
        volX = -volX;
      }
      else {//If a corner is hit
        volX = -volX;
        volY = -volY;
      }
    }
    //Done checking if a collision occurred, now increment the ball position to the next spot
    TV.set_pixel(x, y, 0);
    x += volX;
    y += volY;
    TV.set_pixel(x, y, 1);
  }



  TV.select_font(font4x6);//Smaller font for the actual game is used
  state = modeDrawStationary;
}

//I made this myself. It sets the speed of the ball, using the iSpeed variable.
//The game updates every iSpeed frame, hence a smaller iSpeed value means the game updates quicker. 
//So the lower iSpeed, the quicker the ball moves.
//Exit by pushing the first button.
void setSpeed()
{
  while (digitalRead(button1Pin) == LOW) {//Exit by pushing the first button
    prevButtonStatus = currButtonStatus;
    currButtonStatus = digitalRead(button2Pin);
    if (prevButtonStatus == LOW && currButtonStatus == HIGH) {//The button has been pushed
      if (iSpeed > 1) {//iSpeed can go no lower than 1 as 1 is the fastest
        iSpeed--;
      }
      //Serial.println(iSpeed);
    }
  }
}

//I made this myself. It sets the play to score, which is at default set to 3. There is no maximum number to what the users can play to.
//Every button push (using the second button) will increment the play to score by 1. Exit by pushing the first button.
void setScore()
{
  while (digitalRead(button1Pin) == LOW) {
    prevButtonStatus = currButtonStatus;
    currButtonStatus = digitalRead(button2Pin);
    if (prevButtonStatus == LOW && currButtonStatus == HIGH) {//Button has been pushed
      playToScore++;
      //Serial.println(playToScore);
    }
  }
}

//I made these next two functions as well.
//They are used to let the user choose which type of sensor they want to use, or if they want it to play by itself as a computer player.
//Choose by pushing the second button which increments player1Sensor and player2Sensor by 1. 
//0 = potentiometer, 1 = photoresistor, 2 = temperature sensor, 3 = computer
//If the second button is pushed when the value is at 3 it will go back to 0.
void setPlayer1Sensor()
{
  while (digitalRead(button1Pin) == LOW) {
    prevButtonStatus = currButtonStatus;
    currButtonStatus = digitalRead(button2Pin);
    if (prevButtonStatus == LOW && currButtonStatus == HIGH) {
      if (player1Sensor == 3) {
        player1Sensor = 0;
      }
      else {
        player1Sensor++;
      }
    }
  }
}

void setPlayer2Sensor()
{
  while (digitalRead(button1Pin) == LOW) {
    prevButtonStatus = currButtonStatus;
    currButtonStatus = digitalRead(button2Pin);
    if (prevButtonStatus == LOW && currButtonStatus == HIGH) {
      if (player2Sensor == 3) {
        player2Sensor = 0;
      }
      else {
        player2Sensor++;
      }
    }
  }
}

/*
*********************************
**************SETUP**************
*********************************
*/

void setup() {
  Serial.begin(9600);
  x = 0;
  y = 0;
  TV.begin(_NTSC);//NTSC is for north american televisions. PAL is for european.

          //Ball starts in the middle of the screen
  ballX = TV.hres() / 2;
  ballY = TV.vres() / 2;

  //let player 1 choose their type of sensor to play with
  setPlayer1Sensor();

  //player 2 chooses their type of sensor to play with
  setPlayer2Sensor();

  //player will choose the speed of the ball
  //Serial.println("Choose your game speed\n");
  setSpeed();

  //then choose the score to play too
  //Serial.println("Choose the score you want to play to\n");
  setScore();
}

//Mix of original code and my editing, and all my comments.
//The physics for the ball are pretty complicated, and I do NOT take credit for them.
void loop() {
  processInputs();

  if (state == modeMenu) {//Draws the menu.
    drawMenu();
  }

  if (state == modeDrawStationary) {//gameA draws the non moving parts of the game, the four walls and the dotted line in the middle.
    drawBox();
  }

  if (state == modeDrawMoving) {//gameB draws and plays the actual game.
    if (frame % iSpeed == 0) { //every iSpeed frame the game updates by moving ball position
                   //Incremenet the balls x and y coordinates (its position)
      ballX += ballVolX;
      ballY += ballVolY;

      // Changes direction and makes a sound if the ball hits the top or bottom wall
      if (ballY <= 1 || ballY >= TV.vres() - 1)
      {
        ballVolY = -ballVolY;
        delay(100);
        TV.tone(2000, 30);
      }

      //Checks if the ball hits the left side, and if it does, makes a sound. Only works when a score is NOT made.
      if (ballVolX < 0 && ballX == leftPaddleX + paddleWidth - 1 && ballY >= leftPaddleY && ballY <= leftPaddleY + paddleHeight) {
        ballVolX = -ballVolX;
        ballVolY += 2 * ((ballY - leftPaddleY) - (paddleHeight / 2)) / (paddleHeight / 2);
        delay(100);
        TV.tone(2000, 30);
      }

      // makes a sound if the ball hits the right wall, and a score is NOT made (ie the ball and paddle are at the same height) 
      if (ballVolX > 0 && ballX == rightPaddleXcoord && ballY >= rightPaddleY && ballY <= rightPaddleY + paddleHeight) {
        ballVolX = -ballVolX;
        ballVolY += 2 * ((ballY - rightPaddleY) - (paddleHeight / 2)) / (paddleHeight / 2);
        delay(100);
        TV.tone(2000, 30);
      }

      //Limits the speed of the ball if users are not playing at the fastest mode
      if (iSpeed > 1) {
        if (ballVolY > maxBallSpeed) {
          ballVolY = maxBallSpeed;
        }
        if (ballVolY < -maxBallSpeed) {
          ballVolY = -maxBallSpeed;
        }
      }
      // Scoring
      if (ballX <= 1) {//Right player scores
        playerScored(RIGHT);
        delay(100);
        TV.tone(500, 300);//Makes a different type of sound during a score
      }
      if (ballX >= TV.hres() - 1) {//If the left player scores
        playerScored(LEFT);
        delay(100);
        TV.tone(500, 300);//Makes a different type of sound during a score
      }
    }
    drawGameScreen();//Updates the screen on the tv after every iSpeed frame.
  }

  if (state == modeGameOver) {//Once a player has won the game
    drawGameScreen();
    //Prints game over message.
    TV.select_font(font8x8);
    TV.print(29, 25, "GAME");
    TV.print(68, 25, "OVER");

    while (button1Status == LOW) {//Must push the first button to advance
      processInputs();
      delay(50);
    }
    TV.select_font(font4x6);

    //Resets the scores back to zero, and goes back to the menu.
    leftPlayerScore = 0;
    rightPlayerScore = 0;
    state = modeMenu;
  }


  TV.delay_frame(1);//Increments frame by 1
  if (++frame == 60) {
    frame = 0; //Resets frame counter if at 60
  }
}
