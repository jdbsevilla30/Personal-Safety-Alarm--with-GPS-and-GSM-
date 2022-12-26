const int buttonPin1 = 2; // Pin that button 1 is connected to
const int buttonPin2 = 3; // Pin that button 2 is connected to
int buttonState1 = 0;     // Current state of button 1
int buttonState2 = 0;     // Current state of button 2
int lastButtonState1 = 0; // Previous state of button 1
int lastButtonState2 = 0; // Previous state of button 2
int buttonPressCount1 = 0;// Number of times button 1 has been pressed
int buttonPressCount2 = 0;// Number of times button 2 has been pressed

unsigned long currentTime;   // Current time
unsigned long pressStartTime;// Time when a button was first pressed

void setup() {
  // Initialize button pins as inputs
  Serial.begin(9600);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
}

void loop() {
  // Read the current states of the buttons
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);


  if (buttonPressCount1 == 3 || buttonPressCount2 == 3)
  {
    buttonPressCount1 = 0;
    buttonPressCount2 = 0;
  }

  // Check if button 1 has just been pressed (transition from LOW to HIGH)
  if (buttonState1 == LOW && lastButtonState1 == HIGH) {

    // Button 1 has just been pressed, save the current time
    pressStartTime = millis();

    buttonPressCount1++;  // Increment button 1 press count

    Serial.println(buttonPressCount1);

    delay(50);

  }

  // Check if button 2 has just been pressed (transition from LOW to HIGH)
  if (buttonState2 == LOW && lastButtonState2 == HIGH) {

    // Button 2 has just been pressed, save the current time
    pressStartTime = millis();
    buttonPressCount2++;  // Increment button 2 press count
    Serial.println(buttonPressCount2);
    delay(50);
  }

  // Save the current button states for comparison in the next iteration
  lastButtonState1 = buttonState1;
  lastButtonState2 = buttonState2;

  // Check if a button has been pressed for more than 5 seconds
  currentTime = millis();
  if (currentTime - pressStartTime > 5000) {
    // A button has been pressed for more than 5 seconds, execute the appropriate case statement
    if (buttonPressCount1 > 0) {
      // Execute case statement for button 1
      switch (buttonPressCount1) {
        case 1:
          Serial.println("Fire");
          // Case a: button 1 pressed once
          // Insert code here to execute when button 1 is pressed once
          break;
        case 2:
          Serial.println("Water");
          // Case b: button 1 pressed twice
          // Insert code here to execute when button 1 is pressed twice
          break;
        case 3:
          Serial.println("Earth");
          // Case c: button 1 pressed thrice
          // Insert code here to execute when button 1 is pressed thrice

          break;


          // Button 1 pressed more than three times, do nothing
          break;
      }
      buttonPressCount1 = 0;
    }
    else if (buttonPressCount2 > 0) {
      // Execute case statement for button 2
      switch (buttonPressCount2) {
        case 1:
          Serial.println("Harrasment");
          // Case d: button 2 pressed once
          // Insert code here to execute when button 2 is pressed once
          break;
        case 2:
          Serial.println("Theft");
          // Case e: button 2 pressed twice
          // Insert code here to execute when button 2 is pressed twice
          break;
        case 3:
          Serial.println("Abduction");
          // Case f: button 2 pressed thrice
          // Insert code here to execute when button 2 is pressed thrice
          break;
      }
      buttonPressCount2 = 0;
    }
  }
}
