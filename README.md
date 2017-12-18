# CS207Project
My project for CS207, 2017 Fall term

The project I have created is an enhanced Pong game on the Arduino. The original version uses two potentiometers to play a two player game of Pong.
This project plays a game of Pong, by connecting an Arduin to an actual television. The television must have composite input which RCA cables can be plugged into. My version allows the users to play with photoresistors, temperature sensors, potentiometers, and even has a mode to play against a computer. 

For the full build you will need:
1x television with composite input
1x RCA cable (two halves)
2x photoresistors
2x temperature sensors
2x potentiometers
4x 10k ohm resistors
1x 1k ohm resistor
1x 470 ohm resistor
2x push buttons
many wires

Note: You do not need all of the six sensors, the game will still workout without them all, you will just have to play with a type of sensor you have. 

Connect the photoresistors to pins A2 and A5. Connect the temperature sensors to A1 and A4, and the potentiometers to A0 and A3. 

Next, cut an RCA cable in half. With one end, connect the inside wiring of the cable to a wire, and connect it to pin 11 on the Arduino. This will be used for audio.

Next using the other half of the cable, connect the inside wiring to the 470 ohm and 1k ohm resistors. The 470 resistor connects to pin 7 on the arduino, and the 1k resistor connects to pin 9. Then, connect the outside wiring on the inside of the cable to ground on the Arduino (this can be done by connecting it to a cut in half jumper wire, which connects to ground.)

The last part of the build requires to pushbuttons being connected. The buttons must be pull down, so that they read a high voltage when pushed. Connect one to pin 2, and the other to pin 3. The button connected to pin 2 will be referred to as the first button, and the button connected to pin 3 will be referred to as the second button.

Once you have the build set up, download the code from my repository. 
Then, download the TVoutBeta1.zip from here: https://code.google.com/archive/p/arduino-tvout/downloads
Extract the files into your Arduino libraries directory. 
If this is not downloaded, the code will NOT work. 

Now, begin the program. Open the serial monitor. The first user will now choose which type of sensor to play with.
Pushing the second button will increment a number by 1, which is printed to the serial monitor. 
When the number is 0, you will play with the potentiometers. 1 plays with the photoresistor, 2 with the temperature sensors,
and if the number is 3, the paddle will move on its own. Pressing the first button will lock in your answer, and move on to the second player making their choice. If you want to play single player, choose 3 so the paddle will move on its own. Pressing the first button once more will move on to choosing the speed of the ball.

To choose the speed of the ball, each second button push will make the ball move faster. The number on the serial monitor will decrement by 1, as the game updates every x frame, where x is the number on screen. If you want to play slow, keep the number at 7 or 8. If you want to play fast, let the number be 1 or 2. The number will not go below 1, because having the game update every frame is as fast as it goes. Press the first button to move on.

After choosing the speed of the ball, you will now choose the score you want to play to. Every second button push will increment the play to score by 1, and it will show the score on the serial monitor. Pressing the first button now starts the game.

The game will now be drawn on screen, and you will have to push the first button to begin. You will have to use the sensors you chose in the first part of the program. Enjoy! 

Once the play to score has been reached, it will automatically reset the game, and wait for the first button to be pushed to start the new game.
