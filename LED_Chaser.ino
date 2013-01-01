/***************************************************************
 * Arduino LED chaser with button control
 ***************************************************************
 * by Sam Kearns
 *
 * This circuit has 6 LEDs and 3 buttons, the LEDs are arranged in a circle
 *
 * The LEDs are connected to PWM outputs and their brightness is continuously adjusted on a sine wave pattern
 * The brightness of each led is 60 degrees out of phase with the next
 * This gives the impression of LED brightness rotating around the circle
 *
 * Button 1 increases the speed of rotation
 * Button 2 decreases the speed of rotation
 * Button 3 reverses the speed of rotation
 */

/***************************************************************
 * SETUP 
 ***************************************************************/
int numButtons = 3;                            // The number of buttons in the circuit
int buttonPin[3] = { 2, 4, 7 };       // An array of the pin numbers these buttons are connected to
boolean buttonState[3] = { false, false, false }; // An array indicating the last known state of each button

int numLEDs = 6;                               // The number of LEDs in the circuit 
int ledPin[6] = { 3, 5, 6, 9, 10, 11 };  // An array of the pin numbers these LEDs are connected to

// These variables control the sine wave for LED brightness
float angle = 0;                               // The current angle of the sine wave
float pi = 3.141592653589793238462643383279;   // pi constant for calculations
float increment = pi/255;                      // The amount the angle is incremented by each time the LEDs are updated
                                               // (Note there are 255 possible steps of LED brightness)
// These variables control the LED update speed
int loopCounter = 0;                           // Incremented on each iteration of the main loop 
int delayTime = 100;                           // How many iterations of the main loop to wait before updating the LEDS
int delayIncrement = 20;                       // When the faster/slower buttons are pushed, delayTime is changed by this much

void setup() {
    for( int i=0; i < numButtons; i++ ) {      // Set the button pins to be inputs
        pinMode(buttonPin[i], INPUT);
    }
    for( int i=0; i < numLEDs; i++ ) {         // Set the LED pins to be outputs
        pinMode(ledPin[i], OUTPUT);
    }
}

/***************************************************************
 * MAIN LOOP 
 ***************************************************************/
void loop() {
    if( buttonPressed(0) && delayTime >= delayIncrement ) {  // If button 1 has been pressed increase the speed
        if( delayTime -delayIncrement < 0 ) {
            delayTime = 0;
        } else {
            delayTime = delayTime -delayIncrement;
        }
    }

    if( buttonPressed(1) ) {                       // If button 2 has been pressed decrease the speed
        delayTime = delayTime +delayIncrement;
    }

    if( buttonPressed(2) ) {                       // If button 3 has been pressed reverse the direction of rotation
        increment = -increment;
    }

    if( loopCounter >= delayTime ) {             // If the number of main loop iterations is high enough then change the LEDs
        float phase = 0;
        angle = angle +increment;
        
        for( int i=0; i<numLEDs; i++ ) {         // For each LED
            analogWrite(ledPin[i], (128*sin(angle+phase)+128) ); // Set the LED brightness using a sin() function
            phase = phase +pi/3;                // After setting each LED we increase the phase by 60 degrees
        }
        loopCounter=0;                          // Reset the main loop counter
        
    } else {                                    // Otherwise increment the counter
        loopCounter = loopCounter +1;
    }
}


/***************************************************************
 * FUNCTIONS 
 ***************************************************************/

/*
 * This function contains logic that ensures we only register each button press once
 * A button must be released and pressed again before it will be noticed again
 */
boolean buttonPressed(int button) {
    
    if( digitalRead(buttonPin[button]) == HIGH && buttonState[button] == false ) {
        // The button has been pressed since we last checked it
        buttonState[button] = true;
        return true;
    } else if( digitalRead(buttonPin[button]) == LOW && buttonState[button] == true ) {
        // The button has been released since we last checked it
        buttonState[button] = false;
    }
    return false;
}
