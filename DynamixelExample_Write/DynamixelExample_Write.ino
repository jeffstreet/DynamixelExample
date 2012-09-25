/*  DynamixelExample_Write
    Version 1
    
    This sketch gives the framework to communicate with Dynamixel servos.

    Based on example code from: https://robosavvy.com/forum/viewtopic.php?t=4505
    With the following changes:
    1.) Length byte increased from 3 to 4 in setLED()
    2.) Serial.print(value, BYTE) changed to Serial.write(value)
    3.) Set baud rate to 57,142
    4.) Added pinMode(2, OUTPUT) to setup.
    5.) Minimized delay values in setLED()
    6.) Miscelaneous formatting niceties
    
    Version Notes
    1.) Initial release.

*/


// Define Parameters //
int i = 0; 
byte LEDAddress = 0X19;
byte servoID = 0xFE; // 0xFE is broadcast to all servos 
byte ledOn = 0x01; 
byte ledOff = 0x00; 


// Setup //
void setup() { 
  pinMode(2, OUTPUT);
  Serial.begin(57142); //57600 nominal
} 


// Main loop //
void loop() { 
  if (i%2 ==0) { 
    setLED(servoID, ledOn); 
    delay(200); 
  }
  else { 
    setLED(servoID, ledOff); 
    delay(200); 
  } 
  
  i++;
  if (i>2) i=0;
  
} // loop() 


void setLED (byte servoID, byte ledState){ 
  unsigned int checksum_ACK; 
  byte notchecksum; 
    
  checksum_ACK =  servoID + 0x04 + 0x03 + LEDAddress + ledState; 
  notchecksum = ~checksum_ACK; 
  
  digitalWrite(2,HIGH);      // Notify max485 transciever to accept tx 
  
  Serial.write(0xFF);        // 1. First start byte 
  Serial.write(0xFF);        // 2. Second start byte
  Serial.write(servoID);     // 3. ID of target servo. 
  Serial.write(0x04);        // 4. Length of string 
  Serial.write(0x03);        // 5. Message type: ping 0x01, read 0x02, write 0x03, or syncwrite 0x83 
  Serial.write(LEDAddress);  // 6. Start address for data to be written 
  Serial.write(ledState);    // 7. Data payload 
  Serial.write(notchecksum); // 8. The check byte
  
  delayMicroseconds(1300);   // Wait for last byte to go through. The time value depends on the message length. View on a scope to minimize.
  digitalWrite(2,LOW);       // Notify max485 transciever to recieve 
} // setLED()
