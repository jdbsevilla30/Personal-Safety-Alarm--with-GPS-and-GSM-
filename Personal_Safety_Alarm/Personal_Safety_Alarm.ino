/*GPS Module library*/ 
#include <TinyGPSPlus.h>
TinyGPSPlus gps;

/*Assign Serial1 as gpsModule*/
#define gpsModule Serial1

/*Assign Serial2 as sim900*/
#define sim900 Serial2

/**
  GSM related variable. It would be preferred if you would use Smart number so you
  can receive links, as globe has blocked them
*/
bool isFirstBoot = true;
String mobileNumber = "9275156419";  //always remember, starts with 9, and not 0
String introMessage = "Hi! This is a test message. \n Receiving this message means that the device is working, and is booted for the first time.";

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
int intrusionCount = 0; //times there are intrusion (limit switch removed or OPEN

/*debounce delay for buttons*/
unsigned long debounceDelay = 50;

/*time handler variable*/
unsigned long currentTime;
unsigned long pressStartTime;

/*String container for messages we're sending to user*/
String streetDanger = "";
String disasterDanger = "";

/*street scenarios*/
String streetScenarioOne = "Harrasment \n";
String streetScenarioTwo = "Theft \n";
String streetScenarioThree = "Abduction \n";

/*disaster scenarios*/
String disasterScenarioOne = "Flood \n";
String disasterScenarioTwo = "Fire \n";
String disasterScenarioThree = "Earthquake \n";

/*device intrusion scenario*/
String intrusionMessage = "Your device is either faulty or hi-jacked. \n";

void sendMessage(String dangerMessage)
{
  sim900.println("AT+CMGF=1");
  delay(1000);
  sim900.print("AT+CMGS=\"+63");
  sim900.print(mobileNumber);
  sim900.println("\"\r");
  delay(1000);
  if (isFirstBoot)
  {
    sim900.print(dangerMessage);
  }
  else
  {
    sim900.print(F("There is a \n")); //modify this
    sim900.print(dangerMessage);
    sim900.print(F("Link to my location: \n")); //and this if you want to add/change the SMS message
    // sim900.print(F("https://www.google.com/maps/place/")); //uncomment this to get actual link
    if (gps.location.isValid())
    {
      sim900.print(gps.location.lat(), 6);
      sim900.print(F(","));
      sim900.print(gps.location.lng(), 6);
    }
    else
    {
      sim900.print(F("INVALID LOCATION"));  //will be sent if there's no valid GPS data
    }
  }

  delay(100);
  sim900.println((char) 26);  // ASCII code of CTRL+Z
  delay(1000);
}

void setup()
{
  pinMode(streetButtonPin, INPUT_PULLUP);
  pinMode(disasterButtonPin, INPUT_PULLUP);
  pinMode(limitSwitchPin, INPUT_PULLUP);
  gpsModule.begin(9600);
  sim900.begin(115200);
  sendMessage(introMessage);  //uncomment this if you don't want introductory message
  //sendMessage("YOUR TEXT HERE");  //or you can comment above and type your text manually here
}

void loop()
{
  handleCondition();
  handleStreetButton();
  handleDisasterButton();
  handleLimitSwitch();
  handleReset();
  gpsCheck();
}

void gpsCheck()
{
  while (gpsModule.available() > 0)
    if (gps.encode(gpsModule.read())) {}
}

void streetCase()
{
  switch (buttonPressCountStreet)
  {
    case 1:
      streetDanger = streetScenarioOne;
      break;
    case 2:
      streetDanger = streetScenarioTwo;
      break;
    case 3:
      streetDanger = streetScenarioThree;
      break;
  }

  sendMessage(streetDanger);
  buttonPressCountStreet = 0;
  streetDanger = "";
}

void disasterCase()
{
  switch (buttonPressCountDisaster)
  {
    case 1:
      disasterDanger = disasterScenarioOne;
      break;
    case 2:
      disasterDanger = disasterScenarioTwo;
      break;
    case 3:
      disasterDanger = disasterScenarioThree;
      break;
  }

  sendMessage(disasterDanger);
  buttonPressCountDisaster = 0;
  disasterDanger = "";

}

void intrusionCase()
{
  if (intrusionCount > 0)
  {
    sendMessage(intrusionMessage);
  }
}

void handleCondition()
{
  isFirstBoot = false;
  currentTime = millis();
  if (currentTime - pressStartTime > 5000)
  {
    if (buttonPressCountStreet > 0)
    {
      streetCase();
    }
    else if (buttonPressCountDisaster > 0)
    {
      disasterCase();
    }
    else if (intrusionCount > 0)
    {
      intrusionCase();
    }
  }
}

void handleStreetButton()
{
  int reading = digitalRead(streetButtonPin);
  if (reading != lastStreetButtonState)
  {
    lastStreetButtonDebounceTime = millis();
  }

  if ((millis() - lastStreetButtonDebounceTime) > debounceDelay)
  {
    if (reading != streetButtonState)
    {
      streetButtonState = reading;
      if (streetButtonState == LOW)
      {
        pressStartTime = millis();
        buttonPressCountStreet++;
      }
    }
  }

  lastStreetButtonState = reading;
}

void handleDisasterButton()
{
  int reading = digitalRead(disasterButtonPin);
  if (reading != lastDisasterButtonState)
  {
    lastDisasterButtonDebounceTime = millis();
  }

  if ((millis() - lastDisasterButtonDebounceTime) > debounceDelay)
  {
    if (reading != disasterButtonState)
    {
      disasterButtonState = reading;
      if (disasterButtonState == LOW)
      {
        pressStartTime = millis();
        buttonPressCountDisaster++;
      }
    }
  }

  lastDisasterButtonState = reading;
}

void handleLimitSwitch()
{
  int reading = digitalRead(limitSwitchPin);
  if (reading != lastLimitSwitchState)
  {
    lastLimitSwitchDebounceTime = millis();
  }

  if ((millis() - lastLimitSwitchDebounceTime) > debounceDelay)
  {
    if (reading != limitSwitchState)
    {
      limitSwitchState = reading;
      if (limitSwitchState == HIGH && intrusionCount < 5)
      {
        intrusionCount++;
      }
      else
      {
        intrusionCount = 0;
      }
    }
  }

  lastLimitSwitchState = reading;
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
