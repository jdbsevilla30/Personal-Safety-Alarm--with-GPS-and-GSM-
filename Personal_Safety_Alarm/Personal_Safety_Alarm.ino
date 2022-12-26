int buttonPressCountStreet = 0;// Number of times button 1 has been pressed
int buttonPressCountDisaster = 0;// Number of times button 2 has been pressed

unsigned long currentTime;   // Current time
unsigned long pressStartTime;// Time when a button was first pressed

const int streetButtonPin = 2;    // the number of the pushbutton pin
int streetButtonState;             // the current reading from the input pin
int lastStreetButtonState = HIGH;   // the previous reading from the input pin

unsigned long lastStreetButtonDebounceTime = 0;  // the last time the output pin was toggled

const int disasterButtonPin = 3;    // the number of the pushbutton pin
int disasterButtonState;             // the current reading from the input pin
int lastDisasterButtonState = HIGH;   // the previous reading from the input pin

unsigned long lastDisasterButtonDebounceTime = 0;  // the last time the output pin was toggled


unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers



void setup() {
  Serial.begin(9600);
  pinMode(streetButtonPin, INPUT_PULLUP);
  pinMode(disasterButtonPin, INPUT_PULLUP);
}

void loop() {
  handleReset();
  handleStreetButton();
  handleDisasterButton();
  handleCondition();
}

void handleCondition() {
  currentTime = millis();
  if (currentTime - pressStartTime > 5000) {
    // A button has been pressed for more than 5 seconds, execute the appropriate case statement
    if (buttonPressCountStreet > 0) {
      // Execute case statement for button 1
      switch (buttonPressCountStreet) {
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
      }
      buttonPressCountStreet = 0;
    }
    else if (buttonPressCountDisaster > 0) {
      // Execute case statement for button 2
      switch (buttonPressCountDisaster) {
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
      buttonPressCountDisaster = 0;
    }
  }
}


void handleStreetButton()
{
  int reading = digitalRead(streetButtonPin);
  if (reading != lastStreetButtonState) {
    lastStreetButtonDebounceTime = millis();
  }

  if ((millis() - lastStreetButtonDebounceTime) > debounceDelay) {
    if (reading != streetButtonState) {
      streetButtonState = reading;
      if (streetButtonState == LOW) {
        pressStartTime = millis();
        buttonPressCountStreet++;
        Serial.println(buttonPressCountStreet);
      }
    }
  }
  lastStreetButtonState = reading;
}


void handleDisasterButton()
{
  int reading = digitalRead(disasterButtonPin);
  if (reading != lastDisasterButtonState) {
    lastDisasterButtonDebounceTime = millis();
  }

  if ((millis() - lastDisasterButtonDebounceTime) > debounceDelay) {
    if (reading != disasterButtonState) {
      disasterButtonState = reading;
      if (disasterButtonState == LOW) {
        pressStartTime = millis();
        buttonPressCountDisaster++;
        Serial.println(buttonPressCountDisaster);
      }
    }
  }
  lastDisasterButtonState = reading;
}

void handleReset()
{
  if (buttonPressCountStreet == 4)
  {
    buttonPressCountStreet = 1;
  }
  else if (buttonPressCountDisaster == 4)
  {
    buttonPressCountDisaster = 1;
  }
}
