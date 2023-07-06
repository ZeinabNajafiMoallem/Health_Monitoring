#include <SoftwareSerial.h>   
#define USE_ARDUINO_INTERRUPTS true    
#include <PulseSensorPlayground.h>     
#include <LiquidCrystal_I2C.h>

SoftwareSerial GSM(10, 11); 
LiquidCrystal_I2C lcd(0x27, 16, 2);  

char phone_no[] = "+989029765170";

byte Heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};


byte Speaker[8] = {
  0b00001,
  0b00011,
  0b01111,
  0b01111,
  0b01111,
  0b00011,
  0b00001,
  0b00000
};

byte Skull[8] = {
  0b00000,
  0b01110,
  0b10101,
  0b11011,
  0b01110,
  0b01110,
  0b00000,
  0b00000
};

byte Degree[8] = {
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte Multi[8] = {
  0b10001,
  0b01010,
  0b01010,
  0b00100,
  0b00100,
  0b01010,
  0b01010,
  0b10001
};

#define bt_M  A5 
#define bt_C  A6 

char inchar; 

int LED_BLUE = 26;
int LED_GREEN = 24;
int LED_ORANGE = 22;

int BUZZER = 28;

int TMP36 = A2 ;
int x;

const int PulseWire = 3;
int Threshold = 550;           
PulseSensorPlayground pulseSensor;  

int MIC = A4;
int value =  0;


void setup() { 

  Serial.begin(9600);
  GSM.begin(9600);   

  pinMode(bt_M, INPUT_PULLUP); 
  pinMode(bt_C, INPUT_PULLUP); 


  Serial.println("Initializing....");
  initModule("AT", "OK", 1000);              
  initModule("ATE1", "OK", 1000);            
  initModule("AT+CPIN?", "READY", 1000);    
  initModule("AT+CMGF=1", "OK", 1000);       
  initModule("AT+CNMI=2,2,0,0,0", "OK", 1000); 
  Serial.println("Initialized Successfully");

  pinMode(22, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(26, OUTPUT);

  pinMode(28, OUTPUT);

  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);
  if (pulseSensor.begin()) {
    //Serial.println("PulseSensor object created!");
  }

  pinMode(MIC, INPUT);

  lcd.init();

  lcd.backlight();

  lcd.createChar(0, Heart);
  lcd.createChar(1, Speaker);
  lcd.createChar(2, Skull);
  lcd.createChar(3, Degree);
  lcd.createChar(4, Multi);

  lcd.clear();

}

void loop() {

  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_ORANGE, LOW);
  

  int MQ7_VALUE = analogRead(A0);
  int MQ135_VALUE = analogRead(A1);
  
  
  lcd.setCursor(6, 0);
  lcd.write(2);
  lcd.print(MQ135_VALUE, 1);
  
  
  lcd.setCursor(0, 1);
  lcd.write(2);
  lcd.print(MQ7_VALUE, 1);
  

  x = analogRead(TMP36);
  float voltage = x * 5.0;
  voltage /= 1024.0;
  float TMP36_VALUE = (voltage - 0.5) * 100.0;
  lcd.setCursor(0, 0);
  lcd.print(TMP36_VALUE, 0);
  lcd.write(3);
  lcd.print("C");
  

  int myBPM = pulseSensor.getBeatsPerMinute();
  if (pulseSensor.sawStartOfBeat()) {
    lcd.setCursor(6, 1);
    lcd.write(0);
    lcd.print(myBPM, 1);
  }
  

  value = analogRead(MIC);
  lcd.setCursor(12, 0);
  lcd.write(1);
  lcd.print(value, 1);
  

  if (MQ7_VALUE <= 5000) {
  }
  else if (MQ7_VALUE >= 6650) {
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_ORANGE, HIGH);
    digitalWrite(BUZZER, HIGH);
    lcd.setCursor(12, 1);
    lcd.write(4);
    lcd.print("CO");
  }
  else {
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_ORANGE, LOW);
    for (int n = 1; n <= 3; n++) {
      digitalWrite(BUZZER, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
      delay(100);
    }
  }


  if (MQ135_VALUE <= 6300) {
  }
  else if (MQ135_VALUE >= 7000) {
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_ORANGE, HIGH);
    digitalWrite(BUZZER, HIGH);
    lcd.setCursor(12, 1);
    lcd.write(4);
    lcd.print("CO2");
  }
  else {
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_ORANGE, LOW);
    for (int n = 1; n <= 3; n++) {
      digitalWrite(BUZZER, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
      delay(100);
    }
  }


  if (TMP36_VALUE <= 70) {
  }
  else if (TMP36_VALUE >= 80) {
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_ORANGE, HIGH);
    digitalWrite(BUZZER, HIGH);
    lcd.setCursor(12, 1);
    lcd.write(4);
    lcd.print("TMP");
  }
  else {
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_ORANGE, LOW);
    for (int n = 1; n <= 3; n++) {
      digitalWrite(BUZZER, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
      delay(100);
    }
  }


  if (myBPM <= 50) {
  }
  else if (myBPM >= 100) {
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_ORANGE, HIGH);
    digitalWrite(BUZZER, HIGH);
    lcd.setCursor(12, 1);
    lcd.write(4);
    lcd.print("BPM");
  }
  else {
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_ORANGE, LOW);
    for (int n = 1; n <= 3; n++) {
      digitalWrite(BUZZER, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
      delay(100);
    }
  }


  if (value <= 350) {
  }
  else if (value >= 450) {
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_ORANGE, HIGH);
    digitalWrite(BUZZER, HIGH);
    lcd.setCursor(12, 1);
    lcd.write(4);
    lcd.print("MIC");
  }
  else {
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_ORANGE, LOW);
    for (int n = 1; n <= 3; n++) {
      digitalWrite(BUZZER, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
      delay(100);
    }
  }

  if (GSM.available() > 0) {
    inchar = GSM.read(); Serial.print(inchar);
    if (inchar == 'R') {
      inchar = GSM.read();
      if (inchar == 'I') {
        inchar = GSM.read();
        if (inchar == 'N') {
          inchar = GSM.read();
          if (inchar == 'G') {
            initModule("ATH", "OK", 1000); 
          }
        }
      }
    }


    else if (inchar == 'T') {
      delay(10); inchar = GSM.read();
      if (inchar == 'M') {
        delay(10); inchar = GSM.read();
        if (inchar == 'P') {
          delay(10);
            sendSMS(phone_no, TMP36_VALUE);
        }
      }
    }


    else if (inchar == 'C') {
      delay(10); inchar = GSM.read();
      if (inchar == 'O') {
        delay(10); inchar = GSM.read();
        if (inchar == '2') {
          delay(10);
            sendSMS(phone_no, MQ135_VALUE);
        }
      }
    }


    else if (inchar == 'C') {
      delay(10); inchar = GSM.read();
        if (inchar == 'O') {
          delay(10);
            sendSMS(phone_no, MQ7_VALUE);
        }
      }


    else if (inchar == 'B') {
      delay(10); inchar = GSM.read();
      if (inchar == 'P') {
        delay(10); inchar = GSM.read();
        if (inchar == 'M') {
          delay(10);
            sendSMS(phone_no, myBPM);
        }
      }
    }


    else if (inchar == 'M') {
      delay(10); inchar = GSM.read();
      if (inchar == 'I') {
        delay(10); inchar = GSM.read();
        if (inchar == 'C') {
          delay(10);
            sendSMS(phone_no, value);
        }
      }
    }


  }


  if (digitalRead (bt_M) == 0) {
    sendSMS(phone_no, (char)"Enter CO2 or CO or TMP or BPM or MIC");
  }
  if (digitalRead (bt_C) == 0) {
    callUp(phone_no);
  }

  delay(5000);
  lcd.clear();
}



void sendSMS(char *number, float msg) {
  GSM.print("AT+CMGS=\""); GSM.print(number); GSM.println("\"\r\n"); 
  delay(500);
  GSM.println(msg); 
  delay(500);
  GSM.write(byte(26)); 
  delay(3000);
}


void callUp(char *number) {
  GSM.print("ATD + "); GSM.print(number); GSM.println(";"); 
  delay(20000);       
  GSM.println("ATH"); 
  delay(100);
}


void initModule(String cmd, char *res, int t) {
  while (1) {
    Serial.println(cmd);
    GSM.println(cmd);
    delay(100);
    while (GSM.available() > 0) {
      if (GSM.find(res)) {
        Serial.println(res);
        delay(t);
        return;
      } else {
        Serial.println("Error");
      }
    }
    delay(t);
  }
}
