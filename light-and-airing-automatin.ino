#include "DHT.h"
#define PIN_HCSR501_1 2
#define PIN_HCSR501_2 3
#define DHTPIN 4
#define PIN_RELE_LIGHT 9
#define PIN_RELE_FAN 8
DHT dht(DHTPIN, DHT22);
volatile boolean flagHCSR501_1;
volatile boolean flagHCSR501_2;

void intHCSR501_1() {
  flagHCSR501_1 = digitalRead(PIN_HCSR501_1);
  if (flagHCSR501_1 == true) {
    digitalWrite(PIN_RELE_LIGHT, 1);
  }
}

void intHCSR501_2() {
  flagHCSR501_2 = digitalRead(PIN_HCSR501_2);
  if (flagHCSR501_2 == true) {
    digitalWrite(PIN_RELE_LIGHT, 1);
  }
}

void setup() {
  pinMode(PIN_HCSR501_1,INPUT);
  pinMode(PIN_HCSR501_2,INPUT);
  pinMode(PIN_RELE_LIGHT,OUTPUT);
  pinMode(PIN_RELE_FAN,OUTPUT);
  digitalWrite(PIN_RELE_LIGHT,0);
  digitalWrite(PIN_RELE_FAN,0);
  flagHCSR501_1 = digitalRead(PIN_HCSR501_1);
  flagHCSR501_2 = digitalRead(PIN_HCSR501_2);
  Serial.begin(9600);
  dht.begin();
  attachInterrupt(0,intHCSR501_1,CHANGE);
  attachInterrupt(1,intHCSR501_2,CHANGE);
  Serial.flush();
  Serial.println("Свет,ТемператураС,Влажность%");
}

void loop() {
  if (flagHCSR501_1 == true || flagHCSR501_2 == true) {
    digitalWrite(PIN_RELE_LIGHT, 1);
  }
  else {
    digitalWrite(PIN_RELE_LIGHT, 0);
  }
  float Humidity = dht.readHumidity(); //Измеряем влажность
  float Temperature = dht.readTemperature(); //Измеряем температуру
  if (isnan(Humidity) || isnan(Temperature)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
    Serial.print(flagHCSR501_1 || flagHCSR501_2);
    Serial.print(",");
    Serial.print("-100");
    Serial.print(",");
    Serial.println("-100");
  }
  else {
    if (Humidity > 55) {
      digitalWrite(PIN_RELE_FAN, 1);
    }
    else {
      digitalWrite(PIN_RELE_FAN, 0);
    }
    Serial.print(flagHCSR501_1 || flagHCSR501_2);
    Serial.print(",");
    Serial.print(Temperature);
    Serial.print(",");
    Serial.println(Humidity);
  }
 delay(10000); // 10 секунды задержки
}
