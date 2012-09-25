/*  
    DynamixelExample_SyncWrite
    Version 1
    
    This sketch gives the framework to communicate with Dynamixel servos.
    In addition, it shows how to use syncWrite packets to communicate with several Synamixels quickly.
    
    Notes
    - This depends upon having the servo addresses be consecutive (e.g. 4, 5, 6). 
    - Set the number of servos and the address of the first servo in the #define section
    - Depends on a baud rate of 57,142
    
    Version Notes
    1) Implements writeGoalPosition via syncWrite for indefinite number of servos. 

*/

// Define Constants //

#define NUMSERVOS 4  // the total number of serovs
#define FIRSTSERVO 1 // the address of the first servo (assumes addresses are consecutive)


// Set Parameters //

// Command Values Parameters
int centerPoint = 512;
float sineCounter = 0;
int commandValue[NUMSERVOS];

// Communication Parameters
int max485pin = 2;

// Setup //
void setup() { 
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  Serial.begin(57142);
} 


// Main loop //
void loop() { 

    computeCommandValues();        
    writeGoalPosition();
      
    delay(5);

} // loop() 


// Function Definitions //

void computeCommandValues() {

  for (int i=0; i<NUMSERVOS; i++){
      commandValue[i] = (int)( centerPoint + 200*sin(sineCounter) );
  }
  
  sineCounter += 0.01;
  
} // computeCommandValues()


void writeGoalPosition(){
  
  // GoalPosition command parameters  
  byte startAddress = 0X1E; // the address of the data byte (give reference manual link)
  byte servoID = 0xFE; // 0xFE means broadcast to all servos 
  byte dataLength = 2;
  byte messageType = 0x83; // specifies syncWrite

  // start to build the checksum
  byte packetLength = (dataLength + 1) * NUMSERVOS + 4;
  byte notchecksum = 0xFE + packetLength + messageType + startAddress + dataLength;
  
  // prepare to write
  digitalWrite(max485pin,HIGH);// Notify max485 to accept tx
  
  // write
  Serial.write(0xFF);          // First start byte 
  Serial.write(0xFF);          // Second start byte 
  Serial.write(0xFE);          // ID of target servo. Use 0xFE for all servos and syncwrite 
  Serial.write(packetLength);  // Length of packet
  Serial.write(messageType);   // Message type: ping 0x01, read 0x02, write 0x03, or syncwrite 0x83 
  Serial.write(startAddress);  // Start address for data to be written 
  Serial.write(dataLength);    // Length of data to write
  
  for (int i=0; i<NUMSERVOS; i++){
    Serial.write(FIRSTSERVO+i);               // ID of servo
    Serial.write(lowByte(commandValue[i]));   // Data, low byte
    Serial.write(highByte(commandValue[i]));  // Data, high byte
    
    notchecksum += (FIRSTSERVO+i) + lowByte(commandValue[i]) + highByte(commandValue[i]); // Add to the checksum
  }
  
  notchecksum = ~ lowByte(notchecksum); // Finish the checksum
  
  Serial.write(notchecksum);   // The checksum byte
  
  // finish writing
  delayMicroseconds(1300 + NUMSERVOS*500); // allow last byte to go through. The time value depends on the message length. View on a scope to minimize.
  digitalWrite(max485pin,LOW); // Set max485 transciever to recieve 
} // writeGoalPosition
