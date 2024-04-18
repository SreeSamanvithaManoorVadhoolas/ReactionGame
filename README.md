# Reaction Game
- The application runs as an Autosar Application
- Upon startup, the program will show a welcome message via the serial port.
- After pressing one of the two white buttons, the program will wait for a random time. 
- After waiting for 1s to 3s a random value (1 or 2) will be displayed on both 7segment displays
- The user has to press the right button in case a ‘1’ is displayed and the left button in case a ‘2’ is displayed
- In case the correct button is being pressed, the measured reaction time in [ms] will be shown and the game can be started again by pressing one of the two buttons.
- In case a wrong button is pressed, an error message will be displayed and the game can be started again by pressing one of the two buttons.
- In case the user does not press a button within 1 s, the message “Too slow” will appear and the game can be started again by pressing one of the two buttons.
- One game consists out of 10 rounds
- At the end of a game, print the score (i.e. correct number of button pressed), the total time and the average time
  
# Arcadian Style light
## Fader:
In the first step, we want to create a fading travelling light. The three LED’s are glowing using the following pattern:
![alt text](https://github.com/SreeSamanvithaManoorVadhoolas/ReactionGame/blob/master/fader.png)
## Glower:
Using the RGB LED, we want to implement an easily configurable glowing function. Using a const - table like the following (pseudo code)
const RG__Glow_t RG_glowtable_1[] = {
//Red Green Blue TimeInMS
{255, 0, 0, 500},
{0, 255, 0, 500},
{0, 0, 255, 500},
{0, 0, 0, 100},
{255, 255, 255, 100},
{0, 0, 0, 100},
{255, 255, 255, 100},
{0, 0, 0, 100},
{255, 255, 255, 100}
};
will create the sequence:
- 500ms red
- 500ms green
- 500ms blue
- 100ms off
- 100ms white
- 100ms off
- 100ms white
- 100ms off
- 100ms white
