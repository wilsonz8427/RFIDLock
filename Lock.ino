#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// Servo setup
#include <Servo.h>
Servo myservo;  // create servo object to control a servo

int potpin = 5;  // digital pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

void setup() {
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  myservo.attach(SERVO_PIN);  // Servo Setup: attaches the servo on pin 5 to the servo object
  myservo.write(180);

  Serial.println("Please scan your card...");
  Serial.println();
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  //Show UID on serial monitor and set to content
  Serial.print("\nUID: ");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "); //Print UID
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ")); //set the UID into content for future comparison
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  content.toUpperCase();
  
  if (content.substring(1) == "15 D2 D2 22" || content.substring(1) == "89 E8 06 BA") //1: Mastercard, 2: TAG: BA,
  {
    Serial.println("Access Granted. Opening...");
    delay(500);

    //Servo Run
    val = digitalRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
    val = map(val, 0, 1023, 0, 180);      // scale it to use it with the servo (value between 0 and 180)
    myservo.write(val);                   // sets the servo position according to the scaled value

    Serial.println("Closing...");
    
    delay(1000);                          // delay
    myservo.write(180);                   // closed degree:180
  }

  else {
    Serial.println("Access denied");
    delay(1000);
  }
}
