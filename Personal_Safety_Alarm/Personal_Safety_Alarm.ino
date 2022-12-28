/*GPS Module library*/
#include <TinyGPSPlus.h>
TinyGPSPlus gps;

/*Assign Serial1 as gpsModule*/
#define gpsModule Serial1

/*Assign Serial2 as sim900*/
#define sim900 Serial2

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

String streetDanger = "";
String disasterDanger = "";

int gsmLocation = 0;

int intrusionCount = 0;

void setup() {
  Serial.begin(115200);
  pinMode(streetButtonPin, INPUT_PULLUP);
  pinMode(disasterButtonPin, INPUT_PULLUP);
  pinMode(limitSwitchPin, INPUT_PULLUP);
  gpsModule.begin(9600);
  sim900.begin(115200);
  introMessage();
}

void loop() {
  handleCondition();
  handleStreetButton();
  handleDisasterButton();
  handleLimitSwitch();
  handleReset();

  while (gpsModule.available() > 0)
    if (gps.encode(gpsModule.read()) && gsmLocation <= 1)
    {

      gsmLocation++;
    }
}

void streetCase() {
  switch (buttonPressCountStreet) {
    case 1:
      Serial.println("Abduction Happens");
      streetDanger = "There is an abduction here in my area:";
      break;
    case 2:
      Serial.println("Mugging Happens");
      streetDanger = "There's a burglary here in my area:";
      break;
    case 3:
      Serial.println("Harrasment Happens");
      streetDanger = "Someone got harrased in my area:";
      break;
  }
  sendDangerMessage(streetDanger);
  buttonPressCountStreet = 0;
  streetDanger = "";
}
void disasterCase() {
  switch (buttonPressCountDisaster) {
    case 1:
      Serial.println("Flood and Landslide happens");
      disasterDanger = "Flood and Landslide here";
      break;
    case 2:
      Serial.println("Earthquake Happens");
      disasterDanger = "Earthquake here";
      break;
    case 3:
      Serial.println("Fire indicent happens");
      disasterDanger = "Fire here";
      break;
  }
  sendDangerMessage(disasterDanger);
  buttonPressCountDisaster = 0;
  disasterDanger = "";

}

void intrusionCase() {
  switch (intrusionCount)
  {
    case 0:
      Serial.println("no intrusion");
      break;
    default:
      Serial.println("Intrusion");
      sendDangerMessage("Intrusion");

  }
  buttonPressCountStreet = 0;
}

void handleCondition() {
  currentTime = millis();
  if (currentTime - pressStartTime > 5000) {
    if (buttonPressCountStreet > 0) {
      streetCase();
    } else if (buttonPressCountDisaster > 0) {
      disasterCase();
    }
    else if (intrusionCount > 0) {
      Serial.println(intrusionCount);
      intrusionCase();
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
        //        Serial.println("Intrusion");
        //        sendDangerMessage("");
        intrusionCount++;
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

void displayInfo()
{
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    //    sendCoordinates();
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  Serial.println("");
}

void sendDangerMessage(String dangerMessage)
{
  Serial2.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial2.println("AT+CMGS=\"+639275156419\"\r"); // Replace x with mobile number
  delay(1000);
  Serial2.print("Hi! This is a test message. Don't get alarmed. <3 \n" );
  Serial2.print(dangerMessage);
  Serial2.print("Link to my location: \n");

  if (gps.location.isValid())
  {
    Serial2.print(gps.location.lat(), 6);
    Serial2.print(F(","));
    Serial2.print(gps.location.lng(), 6);
  }
  else
  {
    Serial2.print(F("INVALID LOCATION"));
  }
  delay(100);
  Serial2.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void introMessage()
{
  Serial2.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial2.println("AT+CMGS=\"+639275156419\"\r"); // Replace x with mobile number
  delay(1000);
  Serial2.print("Receiving this text means that the device works!" );
  delay(100);
  Serial2.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
