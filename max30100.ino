#include "MAX30100.h"
#include <Wire.h>

MAX30100 sensor;

boolean getInterruption = false;
unsigned long previousMillis = 0;
// constants won't change :
const long interval = 10;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  while (!Serial);
  sensor.reset();
  sensor.init();
  attachInterrupt(digitalPinToInterrupt(2), interruption, FALLING);
  delay(1000);
}

void loop() {
  float temperature = 0;
  char buffer[35]; // you have to be aware of how long your data can be
                 // not forgetting unprintable and null term chars
  //sprintf(buffer,"var1:%i\tvar2:%i\tvar3:%i",var1,var2,var3);
  //Serial.println(buffer);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    //sensor.GetTemp();
    sensor.readSensor();
    //Serial.println(meanDiff(sensor.IR));
    //sprintf(buffer,"%d %d",meanDiff(sensor.IR),meanDiff(sensor.RED));
    sprintf(buffer,"%d,",meanDiff(sensor.IR));
    Serial.println(buffer);
  }

  if (getInterruption) {
    getInterruption = false;
    int interruption = 0;
    interruption = sensor.GetStatus();
    //Serial.println("Interrupt!!!!");
    //Serial.println(interruption);

    if ( interruption & 0x80 ) {
      Serial.println("almost full!!!!!!!!\n");
      //sensor.readSensor();
    }
    if ( interruption & 0x40 ) {
      //Serial.println("temp ready");
      //Serial.println(sensor.ReadTemp());
    }
    if ( interruption & 0x20 ) {
      //Serial.println("heart rate ready\n");
      //sensor.readSensor();
    }
    if ( interruption & 0x10 ) {
      //Serial.println("spo2 ready\n");
      //sensor.readSensor();
    }
    if ( interruption & 0x01 ) {
      Serial.println("power ready");
      //MAX30100_Read();
    }
  }
}

void interruption()
{
  getInterruption = true;
}

long meanDiff(int M) {
#define LM_SIZE 15
  static int LM[LM_SIZE];      // LastMeasurements
  static byte index = 0;
  static long sum = 0;
  static byte count = 0;
  long avg = 0;

  // keep sum updated to improve speed.
  sum -= LM[index];
  LM[index] = M;
  sum += LM[index];
  index++;
  index = index % LM_SIZE;
  if (count < LM_SIZE) count++;

  avg = sum / count;
  return avg - M;
}
