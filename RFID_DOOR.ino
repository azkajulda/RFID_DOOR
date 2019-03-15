#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SERVO_PIN 3
Servo myservo;
bool isOpen = false;

String RFID[] = {"2F-48-C5-15","26-A7-7-85","6F-F8-D2-15","F-24-CE-14","F-C5-D2-15","82-50-98-5E","62-57-3-5E","4F-94-59-52","AF-81-D2-15","2F-E1-D2-15","7A-3A-E5-DF","F-9-D3-15","AF-BB-3C-1D","72-CF-8C-5E","92-FB-94-5E","92-F-B3-5E","4F-3D-D2-15","D2-C1-1-31","83-90-B8-0","AF-2D-D3-15",""};
byte count = sizeof RFID / sizeof RFID[0] ;

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.


void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Arduino RFID door lock");

  myservo.attach(3);
  myservo.write(5);
  //  myservo.write( 0 );

  pinMode(7, INPUT);
  digitalWrite(7, LOW);

}
void push()
{

  if (digitalRead(7) == HIGH)
  {
    myservo.write(0);
    delay(15);
  }
}

void loop() {
  //Look for new cards
  if ( !mfrc522.PICC_IsNewCardPresent() ) {
    return;
  }
  //Select one of the cards
  if ( !mfrc522.PICC_ReadCardSerial() ) {
    return;
  }

  String content = "";
  byte letter;
  byte found=0;
  for ( byte i = 0; i < mfrc522.uid.size; i++ ) {
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
    if ( i < mfrc522.uid.size - 1 ) content += "-";
  }
  content.toUpperCase();
  Serial.println();
  Serial.println("UID tag :'" + content + "'");

  for(byte j=0; j<=count;j++){
      if(content == RFID[j]){
        found++;
        }
    }
  if ( found > 0) {
    if(isOpen){
      Serial.println("Authorized access | Closed");
      Serial.print(found);
      myservo.write(5);
      //digitalWrite(3, 0);
      delay(1000);
      push();      
    }else{
      Serial.println("Authorized access | Opened");
      myservo.write(30);
      //digitalWrite(3, 180);
      delay(1000);
      push();      
    }
    isOpen = !isOpen;
  }
  else
  {
    Serial.println("Access denied");
  }

  delay(1000);
}
