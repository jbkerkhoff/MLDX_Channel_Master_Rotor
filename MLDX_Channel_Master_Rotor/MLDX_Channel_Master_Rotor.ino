/* MLDX_Rotor_1
Converts MacLogger DX rotor USB commands to infrared pulses to drive a Channel Master consumer-grade antenna rotor
Uses the MLDX "EZ Rotor" configuration
Responds to two commands; Request current bearing (ATI1;) and Send New Bearing (AP1xxx;) followed by (AM1;)
Full Channel Master IR command set is implemented in the IR function. Based on NEC IR codes.
Uses depricated IRremote library version 2.4.0b
IR emitter connects between 03 and gnd through 220 ohm resistor
*/
// include the library code:
#include <IRremote.h> //IRremote 2.4.0b (retired) 

IRsend irsend; //Initiate IR operations

// Variables
const unsigned int MAX_MESSAGE_LENGTH = 8; //longest MLDX command is 8 chars
const char nullString[MAX_MESSAGE_LENGTH] = "        ";
//static char message[MAX_MESSAGE_LENGTH];
static char command[MAX_MESSAGE_LENGTH]; //command portion of decoded from MLDX string
static char fullMessage[MAX_MESSAGE_LENGTH]; //entire decoded MLDX string
const char inquiry[] = "AI1"; //request for bearing from MLDX
const char stopRotor[] = ";"; //from MLDX
const char setHeading[] = "AP1"; //from MLDX
const char goHeading[] = "AM1;"; //execute from MLDX, always follows setHeading
static char heading[] = "000"; //heading from MLDX
static char bearing[] = ";000"; //bearing set, sent to MLDX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //open serial port
}

void loop() {
  
  //Check to see if anything is available in the serial receive buffer
  while (Serial.available() > 0)
  {
    // Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;

    //Read the next available byte in the serial receiver buffer
    char inByte = Serial.read();

    //Message coming in (check not terminating character)
    if (inByte != ';' && (message_pos < MAX_MESSAGE_LENGTH - 1))
    {
      //Add the coming byte to our message
      message[message_pos] = inByte;
      message_pos++;
    } //end if
    //Full message received . . .
    else
    {
        //Add null character to string
        message[message_pos] = '\0';

        //Save the message into command
        strncpy(fullMessage, message, sizeof(fullMessage)); //saves full message
        strncpy(command, message, 3);// save first 3 char of message to command
        command[3] = NULL; //places NULL at position 4, command is now 3 chars long (0,1,2,3 -- first position is 0)
        //Reset for the next message
        message_pos = 0;
        //strncpy (message,nullString,sizeof(message));
    } //end else
  } //end while

  // ----- command decision tree -----
  
  if (strcmp (command, inquiry) == 0) { // looking for "AI1;" from MLDX
    Serial.write (bearing); //send the current bearing
    strncpy(command,nullString,sizeof(command)); //reset command
  }
  else if (strcmp (command, stopRotor) == 0) { // looking for ";" from MLDX
  }
  else if (strcmp (command, setHeading) == 0) { //looking for "AP1" from MLDX
    strncpy(heading, &fullMessage[3], sizeof(heading));//pull just "bearing" from command (address of command at position 3)
    runRotor (heading); //send heading to rotor
    strncpy(bearing,";", sizeof(bearing)); // begin building the bearing with a ';'
    strncpy(&bearing[1],heading, sizeof(bearing)); //and add the commanded heading (address of bearing at position 1 to make ;xxx)
    strncpy(command,nullString,sizeof(command)); //reset command
  }
  else if (strcmp (command, goHeading) == 0) {

  }
  else {

  }
}

void runRotor(char heading[3]) {
  char incomingByte;
  int i = 0;
  for (i = 0; i <= 2; i++)
    {
      incomingByte = heading[i];
      switch(incomingByte) {
        case 'p':irsend.sendNEC(0x35ca38c7, 38); // Rotor power code
        delay(500); break;
        case 'u':irsend.sendNEC(0x35ca08f7, 38); // Rotor up code
        delay(500); break;
        case 'd':irsend.sendNEC(0x35ca8877, 38); // Rotor down code
        delay(500); break;
        case '0':irsend.sendNEC(0x35ca00ff, 38); // Rotor TV "0" code
        delay(300); break;
        case '1':irsend.sendNEC(0x35ca807f, 38); // Rotor TV "1" code
        delay(300); break;
        case '2':irsend.sendNEC(0x35ca40bf, 38); // Rotor TV "2" code
        delay(300); break;
        case '3':irsend.sendNEC(0x35cac03f, 38); // Rotor TV "3" code
        delay(300); break;
        case '4':irsend.sendNEC(0x35ca20df, 38); // Rotor TV "4" code
        delay(300); break;
        case '5':irsend.sendNEC(0x35caa05f, 38); // Rotor TV "5" code
        delay(300); break;
        case '6':irsend.sendNEC(0x35ca609f, 38); // Rotor TV "6" code
        delay(300); break;
        case '7':irsend.sendNEC(0x35cae01f, 38); // Rotor TV "7" code
        delay(300); break;
        case '8':irsend.sendNEC(0x35ca10ef, 38); // Rotor TV "8" code
        delay(300); break;
        case '9':irsend.sendNEC(0x35ca906f, 38); // Rotor TV "9" code
        delay(300); break;
      } // end case
  } //end for
} //end func setRotor
