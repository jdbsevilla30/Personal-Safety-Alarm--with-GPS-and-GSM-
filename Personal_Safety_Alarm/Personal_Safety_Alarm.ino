/*Street button*/
const int streetButtonPin = 2;
int streetButtonState;
int lastStreetButtonState = HIGH;
unsigned long lastStreetButtonDebounceTime = 0;
int buttonPressCountStreet = 0; //counter for when street button is pressed

/*Disaster button*/
const int disasterButtonPin = 3;
int disasterButtonState;
int lastDisasterButtonState = HIGH;
unsigned long lastDisasterButtonDebounceTime = 0;
int buttonPressCountDisaster = 0; //counter for when disaster button is pressed

/*Limit switch*/
const int limitSwitchPin = 4;
int limitSwitchState;
int lastLimitSwitchState = HIGH;
unsigned long lastLimitSwitchDebounceTime = 0;
int limitSwitchCount = 0;

/*debounce delay for buttons*/
unsigned long debounceDelay = 50;

/*time handler variable*/
unsigned long currentTime;
unsigned long pressStartTime;

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
      if (buttonPressCountStreet > 0) {
         switch (buttonPressCountStreet) {
         case 1:
            Serial.println("Fire");
            break;
         case 2:
            Serial.println("Water");
            break;
         case 3:
            Serial.println("Earth");
            break;
         }
         buttonPressCountStreet = 0;
      } else if (buttonPressCountDisaster > 0) {
         switch (buttonPressCountDisaster) {
         case 1:
            Serial.println("Harrasment");
            break;
         case 2:
            Serial.println("Theft");
            break;
         case 3:
            Serial.println("Abduction");
            break;
         }
         buttonPressCountDisaster = 0;
      }
   }
}

void handleStreetButton() {
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

void handleDisasterButton() {
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

void handleReset() {
   if (buttonPressCountStreet == 4) {
      buttonPressCountStreet = 1;
   } else if (buttonPressCountDisaster == 4) {
      buttonPressCountDisaster = 1;
   }
}
