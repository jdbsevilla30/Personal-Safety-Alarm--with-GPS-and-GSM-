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
int limitSwitchCounter = 0; //counter for when limit switch is pressed

/*debounce delay for buttons*/
unsigned long debounceDelay = 50;

/*time handler variable*/
unsigned long currentTime;
unsigned long pressStartTime;

void setup() {
  Serial.begin(9600);
  pinMode(streetButtonPin, INPUT_PULLUP);
  pinMode(disasterButtonPin, INPUT_PULLUP);
  pinMode(limitSwitchPin, INPUT_PULLUP);
}

void loop() {
  handleCondition();
  handleStreetButton();
  handleDisasterButton();
  handleLimitSwitch();
  handleReset();
}

void streetCase() {
  switch (buttonPressCountStreet) {
    case 1:
      Serial.println("Abduction Happens");
      break;
    case 2:
      Serial.println("Mugging Happens");
      break;
    case 3:
      Serial.println("Harrasment Happens");
      break;
  }
  buttonPressCountStreet = 0;
}
void disasterCase() {
  switch (buttonPressCountDisaster) {
    case 1:
      Serial.println("Flood and Landslide happens");
      break;
    case 2:
      Serial.println("Mugging Happens");
      break;
    case 3:
      Serial.println("Fire indicent happens");
      break;
  }
  buttonPressCountDisaster = 0;
}

void handleCondition() {
  currentTime = millis();
  if (currentTime - pressStartTime > 5000) {
    if (buttonPressCountStreet > 0) {
      streetCase();
    } else if (buttonPressCountDisaster > 0) {
      disasterCase();
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

void handleLimitSwitch() {
  int reading = digitalRead(limitSwitchPin);
  if (reading != lastLimitSwitchState) {
    lastLimitSwitchDebounceTime = millis();
  }

  if ((millis() - lastLimitSwitchDebounceTime) > debounceDelay) {
    if (reading != limitSwitchState) {
      limitSwitchState = reading;
      if (limitSwitchState == HIGH) {
        Serial.println("Intrusion");
      }
      else {

      }
    }
  }
  lastLimitSwitchState = reading;
}

void handleReset() {
  if (buttonPressCountStreet == 4) {
    buttonPressCountStreet = 1;
  } else if (buttonPressCountDisaster == 4) {
    buttonPressCountDisaster = 1;
  }
}
