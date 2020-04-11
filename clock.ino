#include <RTClib.h>
#include<Wire.h>

RTC_DS1307 RTC;
int buzzerClock = 7, PIR = 3;
int timer = 0;
String TIME = "";
int clockon, hourset, minuteset;
void setup() {
  pinMode(buzzerClock, OUTPUT);
  digitalWrite(buzzerClock, LOW);
  pinMode(PIR, INPUT);
  Wire.begin();
  RTC.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));
  clockon = 0;
  hourset = -1;
  minuteset = -1;
  Serial.begin(9600);
  Serial.println("clock setup success");
  Serial.println("please enter the hour setting");
}

void loop() {
  if (clockon == 0) {
    /*闹钟不启用*/
    TIME = "";
    /*通过循环读取字符串*/
    if (hourset == -1) {
      if (Serial.available() > 0) {
        while (Serial.available() > 0) {
          char k = Serial.read();
          TIME = TIME + k;
          delay(3);
        }
        hourset = TIME.toInt();
        if (hourset < 0 || hourset > 23) {
          hourset = -1;
          Serial.println("hour setting error, please redo the hour setting");
        }
        else {
          Serial.println("hour setting to be " + TIME + ",please enter the minute setting");
        }

        TIME = "";
      }
    }
    if (minuteset == -1) {
      if (Serial.available() > 0) {
        while (Serial.available() > 0) {
          char k = Serial.read();
          TIME = TIME + k;

          delay(3);
        }
        minuteset = TIME.toInt();

        if (minuteset < 0 || minuteset > 59) {
          minuteset = -1;
          Serial.println("minute setting error, please redo the minute setting");
        }
        else {

          Serial.println("minute setting to be " + TIME);
          /*Serial.println(hourset);
            Serial.println(minuteset);*/
          Serial.println("clock set success, No.3 clock is on"); clockon = 1;
        }

      }
    }
  }
  else {

    DateTime now = RTC.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    int nowhour = now.hour(), nowminute = now.minute();
    /*Serial.print(nowhour); Serial.println(nowminute);*/
    if (timer == 0) {
      if (nowhour == hourset && nowminute == minuteset) {
        timer = 90;
        digitalWrite(buzzerClock, HIGH);
      }
    }
    if (timer > 0) {
      timer = timer - 1;
      if (digitalRead(PIR) == HIGH) {
        digitalWrite(buzzerClock, LOW);
      }
    }
    if (timer == 1) {
      digitalWrite(buzzerClock, LOW);

    }

    /*if(digitalRead(PIR)==HIGH)k=0;
      while (k)
      { for (i = 0; i < 80; i++)
      {
        digitalWrite(buzzerClock, HIGH);
        delay(2000);
        digitalWrite(buzzerClock, LOW);
        delay(2000);
      }
      delay(20);
      for (i = 0; i < 80; i++)
      {
        digitalWrite(buzzerClock, HIGH);
        delay(2);
        digitalWrite(buzzerClock, LOW);
        delay(2);
      }
      }
      k=1;*/
    delay(1000);
  }
}


