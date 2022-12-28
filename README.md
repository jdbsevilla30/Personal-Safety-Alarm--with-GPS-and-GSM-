# Personal-Safety-Alarm--with-GPS-and-GSM-

This project is a customized personal safety alarm, and it consist of Neo6M GPS Module, Sim900a GSM Module, two buttons, and a limit switch. As per my client's requests,
each button and the limit switch has specific designated message per action. 

mobileNumber is the number that we'll be using as the receiver of the text messages. Be sure to use a provider that doesn't block links.
String mobileNumber = "9275156419";  

You may modify the introMessage to your liking, depending on what you wanted to see when you turn on your project. 
String introMessage = "Hi! This is a test message. \n Receiving this message means that the device is working, and is booted for the first time.";


Button 1 
- Pressed one (1) time: Street scenario 1 
- Pressed two (2) times: Street Scenario 2
- Pressed three (3) times: Street Scenario 3

Here below are the button 1 press equivalent: 

String streetScenarioOne = "Harrasment \n";
String streetScenarioTwo = "Theft \n";
String streetScenarioThree = "Abduction \n";

Button 2
- Pressed one (1) time: Disaster scenario 1 
- Pressed two (2) times: Disaster Scenario 2
- Pressed three (3) times: Disaster Scenario 

Here below are the button 2 press equivalent: 

String disasterScenarioOne = "Flood \n"; 
String disasterScenarioTwo = "Fire \n";
String disasterScenarioThree = "Earthquake \n";

Limit Switch (OPEN or Physically removed) 
- Released: Device Intrusion message

Here below is the device intrusion message variable:

String intrusionMessage = "Your device is either faulty or hi-jacked. \n";

(Note: The values are replacable, depending on what the user will set) 


After the message has been set, it would be then now passed as an argument to our void sendMessage(String dangerMessage). 
Our sendMessage function handles the text message that we're going to send to our user, and also generates a google maps link for us. 

Wiring: 

NEO6M V2 Serial1
GND - GND
VCC - 5V 
TX - RX1
RX - TX1 

Sim900A Serial2
GND - GND
VCC - 5V 
TX - RX2
RX - TX2 

Push Button 1 (Street Scenario) INPUT_PULLUP
GND - GND 
Pin - D2

Push Button 2 (Disaster Scenario) INPUT_PULLUP
GND - GND
Pin - D3

Limit Switch INPUT_PULLUP
GND - Closed
NO - D4



