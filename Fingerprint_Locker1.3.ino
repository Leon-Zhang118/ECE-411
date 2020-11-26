/***************************************************
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/
//Most serial print messages are purely there for testing to make sure everything functions properly
//Not necessary for final product, can move some serial print messages to LED display 

#include <Adafruit_Fingerprint.h> //library for fingerprint sensor
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(2,3 );
#else
#define mySerial Serial //establish connection with serial pins of chip ; hardware serial
#endif
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial); //set up variable for fingerprint sensor

uint8_t id = 1; //set up variable for id to store fingerprints,may be for testing only and implemented differently
int FingerprintCheck; //set up variable for a check if the fingerprint has a match
int Solenoid = 5; //set up pin for solenoid lock
int RegisterButton = 4; //set up pin for button to register new fingerprints

void setup() {
  pinMode(Solenoid, OUTPUT); //configure solenoid pin as an output
  pinMode(RegisterButton, INPUT_PULLUP); //configure button pin as an input
  pinMode(LED_BUILTIN, OUTPUT);
  
  //Sets data rate for serial monitor (for testing purposes)
//  Serial.begin(9600);
//  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  finger.begin(57600); //set data rate for sensor serial port

  //(Testing) checks if the sensor is active and communicating with the microcontroller
//  if (finger.verifyPassword()) {
//    Serial.println("Found fingerprint sensor!");
//  } else {
//    Serial.println("Did not find fingerprint sensor :(");
//    while (1) { delay(1); }
//  }

  //(Testing) checks various sensor parameters
//  Serial.println(F("Reading sensor parameters"));
//  finger.getParameters();
//  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
//  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
//  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
//  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
//  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
//  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
//  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

//  //(Testing) Asks user to input id# to save fingerprint data on
//  Serial.println("Ready to enroll a fingerprint!");
//  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
//  id = readnumber();
//  if (id == 0) {// ID #0 not allowed, try again!
//     return;
//  }
//  Serial.print("Enrolling ID #");
//  Serial.println(id);
//
//  while (!  getFingerprintEnroll() );
}


//(Testing) Ask user to input a number through the serial monitor
//uint8_t readnumber(void) {
//  uint8_t num = 0;
//
//  while (num == 0) {
//    while (! Serial.available());
//    num = Serial.parseInt();
//  }
//  return num;
//}


void loop() {
  digitalWrite(LED_BUILTIN, HIGH); 
  getFingerprintID();
  if ( digitalRead(RegisterButton) == LOW ) getFingerprintEnroll();
}


//Takes and records the user's fingerprint(s)
uint8_t getFingerprintEnroll() {
  
  int p = -1;
  digitalWrite(LED_BUILTIN, LOW);
  // If the first fingerprint has already been set, prevents new registrations from being set 
  // Unless allowed by the first registered user. Prevents others from simply registering and breaking in
  if (id != 1) {
    FingerprintCheck = getFingerprintID();
    if (FingerprintCheck == -1) return p;
  }
  
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);

  //waits until finger is placed and then takes a picture
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
//      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
//      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
//      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
//      Serial.println("Imaging error");
      break;
    default:
//      Serial.println("Unknown error");
      break;
    }
  }

  digitalWrite(LED_BUILTIN, HIGH);
  //Converts image taken into a template in slot 1
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
//      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
//      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
//      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
//      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
//      Serial.println("Could not find fingerprint features");
      return p;
    default:
//      Serial.println("Unknown error");
      return p;
  }
  
//  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
//  Serial.print("ID "); Serial.println(id);
  p = -1;
  digitalWrite(LED_BUILTIN, LOW);
  //waits until finger is placed and takes a picture (should be the same finger used earlier)
//  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
//      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
//      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
//      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
//      Serial.println("Imaging error");
      break;
    default:
//      Serial.println("Unknown error");
      break;
    }
  }
  digitalWrite(LED_BUILTIN, HIGH);
  //Converts image taken into a template in slot 2
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
//      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
//      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
//      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
//      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
//      Serial.println("Could not find fingerprint features");
      return p;
    default:
//      Serial.println("Unknown error");
      return p;
  }

//  Serial.print("Creating model for #");  Serial.println(id);

  //Takes the two created templates and generates a model
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
//    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
//    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
//    Serial.println("Fingerprints did not match");
    return p;
  } else {
//    Serial.println("Unknown error");
    return p;
  }

  //Store generated model into sensor's internal memory
//  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
//    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
//    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
//    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
//    Serial.println("Error writing to flash");
    return p;
  } else {
//    Serial.println("Unknown error");
    return p;
  }
  digitalWrite(LED_BUILTIN, LOW);
  ++id; //Increment id to allow new fingerprints to be saved (can be changed to allow users to manually save)
  return true;
}


//Take user's fingerprint and attempts to find matches to saved templates
//(p != FINGERPRINT_OK) used for errors if function could not be performed
int getFingerprintID() {
  
  //Take image of user's fingerprint
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) {
//    Serial.println("An error occurred");
    return -1;
  }
  
  //Converts the image into a template
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  {
//    Serial.println("An error occurred");
    return -1;
  }

  //Search for a match between the feature template and saved templates
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  {
//    Serial.println("No match found");
    return -1;
  }

  //(Testing) If a match is found state which id # the match template was saved in and confidence of match
//  Serial.print("Found ID #"); Serial.print(finger.fingerID);
//  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  //Opens lock if the fingerprint matches with high enough confidence, better match is greater confidence
  if (finger.confidence > 0) {
    digitalWrite(Solenoid, HIGH);
    delay(800);
    digitalWrite(Solenoid, LOW);
    delay(800);
  }
  else {
//    Serial.print("Match does not fit\n"); //(Testing) match found was not storng enough
  }
  
  return finger.fingerID;
}
