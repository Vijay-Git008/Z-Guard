#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Helper libraries
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

//==============================
// PIN DEFINITIONS
//==============================

// RFID RC522
#define SS_PIN     5
#define RST_PIN    4
#define TAMPER_BUTTON 26
// Potentiometer
#define POT_PIN    34

// Relay
#define RELAY_PIN  27

// Buzzer
#define BUZZER_PIN 25
//==============================
// WIFI DETAILS
//==============================

#define WIFI_SSID "Kiruthika"
#define WIFI_PASSWORD "Kiruthika@007"

//==============================
// FIREBASE DETAILS
//==============================

#define API_KEY "AIzaSyBqiX3X0IP20UZrL-hAuBKbRH8jkq2HnLY"

#define DATABASE_URL "https://z-guard-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Function declarations
void checkTamper();
void checkRFID();
void checkMachineHealth();
void grantAccess();
void denyAccess();
void calculateGuardian(); 
void uploadToFirebase();
//==============================
// OBJECT CREATION
//==============================

MFRC522 rfid(SS_PIN, RST_PIN);

LiquidCrystal_I2C lcd(0x27,16,2);


//==============================
// RFID AUTHORIZED CARD UID
// CHANGE THIS AFTER READING UID
//==============================

byte allowedUID[4] = {
  0xCB,
  0xCA,
  0xF9,
  0x04
};


//==============================
// MACHINE HEALTH THRESHOLD
// ESP32 ADC RANGE : 0-4095
//==============================

int LOW_THRESHOLD  = 1500;
int HIGH_THRESHOLD = 3000;


// Variables

int healthValue = 0;
int healthPercent = 0;

bool faultStatus = false;
//==============================
// ZGUARD VARIABLES
//==============================

int healthScore = 0;

int rfidScore = 100;

int tamperScore = 100;

int trustScore = 100;

int isiScore = 100;

bool rfidValid = true;

bool tamperDetected = false;
FirebaseData fbdo;

FirebaseAuth auth;

FirebaseConfig config;

bool signupOK = false;

String guardianDecision = "";
//==============================
// SETUP
//==============================

void setup()
{

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

while (WiFi.status() != WL_CONNECTED)
{
    Serial.print("WiFi Status: ");
    Serial.println(WiFi.status());
    delay(1000);
}
Serial.println(WiFi.localIP());
config.api_key = API_KEY;
config.database_url = DATABASE_URL;

if (Firebase.signUp(&config, &auth, "", ""))
{
    Serial.println("Firebase SignUp OK");
    signupOK = true;
}
else
{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
}

Firebase.begin(&config, &auth);
Firebase.reconnectWiFi(true);

  // Output pins

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TAMPER_BUTTON, INPUT_PULLUP);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);



  // LCD

  Wire.begin(21,22);

  lcd.init();
  lcd.backlight();


  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ZGuard System");

  lcd.setCursor(0,1);
  lcd.print("Initializing");


  // RFID

  SPI.begin(18,19,23,5);

  rfid.PCD_Init();


  delay(2000);


  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("System Ready");

  delay(1000);


  lcd.clear();

}


//==============================
// MAIN LOOP
//==============================

void loop()
{


  checkMachineHealth();


  checkRFID();
  checkTamper();
  calculateGuardian();
  uploadToFirebase();
  delay(100);

}



//==============================
// MACHINE HEALTH FUNCTION
//==============================

void checkMachineHealth()
{


  healthValue = analogRead(POT_PIN);


  healthPercent = map(
    healthValue,
    0,
    4095,
    0,
    100
  );
  healthScore = healthPercent;
  Serial.print("Machine Health Value : ");
  Serial.println(healthValue);


  Serial.print("Health Percentage : ");
  Serial.print(healthPercent);
  Serial.println("%");



  if(healthValue < LOW_THRESHOLD)
  {

      faultStatus = true;


      digitalWrite(BUZZER_PIN,HIGH);


      Serial.println("STATUS : FAULT DETECTED");


      lcd.clear();

      lcd.setCursor(0,0);
      lcd.print("Health score:");
      lcd.print(healthPercent);


      lcd.setCursor(0,1);
      lcd.print("FAULT ALERT");


  }

  else
  {

      faultStatus = false;


      digitalWrite(BUZZER_PIN,LOW);


      Serial.println("STATUS : HEALTHY");


      lcd.clear();

      lcd.setCursor(0,0);
      lcd.print("Health score:");
      lcd.print(healthPercent);
      lcd.print("%");


      lcd.setCursor(0,1);
      lcd.print("Machine OK");


  }

}
//==============================
// RFID CHECK FUNCTION
//==============================

void checkRFID()
{


  // Check if new card is present

  if(!rfid.PICC_IsNewCardPresent())
  {
    return;
  }


  // Read card

  if(!rfid.PICC_ReadCardSerial())
  {
    return;
  }



  Serial.println();
  Serial.println("RFID CARD DETECTED");



  bool accessGranted = true;



  // Compare UID

  for(byte i=0;i<4;i++)
  {

    Serial.print(rfid.uid.uidByte[i],HEX);
    Serial.print(" ");


    if(rfid.uid.uidByte[i] != allowedUID[i])
    {
      accessGranted = false;
    }

  }


  Serial.println();



  if(accessGranted)
  {

    grantAccess();

  }

  else
  {

    denyAccess();

  }



  // Stop RFID communication

  rfid.PICC_HaltA();

  rfid.PCD_StopCrypto1();


}



//==============================
// ACCESS GRANTED FUNCTION
//==============================

void grantAccess()
{


  Serial.println("----------------");
  Serial.println("ACCESS ALLOWED");
  Serial.println("----------------");



  lcd.clear();


  lcd.setCursor(0,0);
  lcd.print("Access Allowed");


  lcd.setCursor(0,1);
  lcd.print("Welcome User");



  // Activate relay

  digitalWrite(RELAY_PIN,HIGH);



  delay(1000);



  digitalWrite(RELAY_PIN,LOW);



  delay(500);



  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Machine Ready");
  rfidValid = true;
  rfidScore = 100;

}



//==============================
// ACCESS DENIED FUNCTION
//==============================

void denyAccess()
{


  Serial.println("----------------");
  Serial.println("ACCESS DENIED");
  Serial.println("----------------");



  lcd.clear();


  lcd.setCursor(0,0);
  lcd.print("Access Denied");


  lcd.setCursor(0,1);
  lcd.print("Invalid Card");



  // Short buzzer alert

  digitalWrite(BUZZER_PIN,HIGH);


  delay(1500);


  digitalWrite(BUZZER_PIN,LOW);



  delay(1000);



  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Machine Ready");
  rfidValid = false;
  rfidScore = 0;

}

//==============================
// PRINT RFID UID FUNCTION
//==============================

void printUID()
{

  Serial.print("Card UID : ");


  for(byte i=0; i<rfid.uid.size; i++)
  {

    if(rfid.uid.uidByte[i] < 0x10)
    {
      Serial.print("0");
    }


    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");

  }


  Serial.println();

}



//==============================
// RFID CARD INFORMATION
//==============================

void showCardInfo()
{

  Serial.println("----------------------");

  Serial.println("RFID CARD INFORMATION");


  Serial.print("UID Size : ");

  Serial.println(rfid.uid.size);



  printUID();


  Serial.println("----------------------");

}



//==============================
// SYSTEM HEALTH REPORT
//==============================

void systemReport()
{


  Serial.println();
  Serial.println("======================");


  Serial.println("ZGuard System Report");


  Serial.print("Health Value : ");

  Serial.println(healthValue);



  Serial.print("Health Level : ");

  Serial.print(healthPercent);

  Serial.println("%");



  if(faultStatus)
  {

    Serial.println("Machine State : FAULT");

  }

  else
  {

    Serial.println("Machine State : NORMAL");

  }



  Serial.println("======================");

}
//==============================
// TAMPER DETECTION
//==============================

void checkTamper()
{

  if(digitalRead(TAMPER_BUTTON) == LOW && tamperDetected == false)
  {

    Serial.println("================");
    Serial.println("TAMPER DETECTED");
    Serial.println("================");


    tamperDetected = true;
    tamperScore = 20;


    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("TAMPER");

    lcd.setCursor(0,1);
    lcd.print("DETECTED");


    digitalWrite(BUZZER_PIN,HIGH);

    delay(1000);

    digitalWrite(BUZZER_PIN,LOW);


  }


}
void calculateGuardian()
{

    //--------------------------
    // TRUST SCORE
    //--------------------------

    if(rfidScore == 0)
    {
        trustScore = 0;
    }
    else
    {
        trustScore = (rfidScore + tamperScore)/2;
    }


    //--------------------------
    // ISI
    //--------------------------

    isiScore = (healthScore*60 + trustScore*40)/100;


    //--------------------------
    // DECISION
    //--------------------------

    if(rfidScore == 0)
    {
        guardianDecision = "ACCESS DENIED";
    }

    else if(healthScore < 50)
    {
        guardianDecision = "FAULT DETECTED";
    }

    else if(tamperDetected)
    {
        guardianDecision = "WARNING";
    }

    else
    {
        guardianDecision = "SAFE TO OPERATE";
    }


    Serial.println("--------------");

    Serial.print("Health : ");
    Serial.println(healthScore);

    Serial.print("Trust : ");
    Serial.println(trustScore);

    Serial.print("ISI : ");
    Serial.println(isiScore);

    Serial.print("Decision : ");
    Serial.println(guardianDecision);

    Serial.println("--------------");

}
void uploadToFirebase()
{
    if (Firebase.ready() && signupOK)
    {
        Firebase.RTDB.setInt(&fbdo, "zguard/healthScore", healthScore);

        Firebase.RTDB.setInt(&fbdo, "zguard/trustScore", trustScore);

        Firebase.RTDB.setInt(&fbdo, "zguard/isi", isiScore);

        Firebase.RTDB.setString(&fbdo, "zguard/rfidStatus",
                                rfidValid ? "VALID" : "INVALID");

        Firebase.RTDB.setString(&fbdo, "zguard/tamperStatus",
                                tamperDetected ? "DETECTED" : "SAFE");

        Firebase.RTDB.setString(&fbdo, "zguard/decision",
                                guardianDecision);

        Serial.println("Firebase Updated");
    }
}
