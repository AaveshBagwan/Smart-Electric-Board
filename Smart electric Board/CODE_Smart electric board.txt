
// Fill-in information from your Blynk Template here

#define BLYNK_TEMPLATE_ID "TMPLI_AttCDQ"
#define BLYNK_DEVICE_NAME "NodeMCU"
#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial
#define APP_DEBUG
#include "BlynkEdgent.h"

BLYNK_WRITE(V0)
{
  if(param.asInt()==1){
    digitalWrite(D4,HIGH);
  }
  else{
    digitalWrite(D4,LOW);
  }
}

BLYNK_CONNECTED()
{
   Blynk.syncVirtual(V0);  
}

void setup()
{
  pinMode(D4,OUTPUT);
  Serial.begin(115200);
  delay(100);
  BlynkEdgent.begin();
}

void loop() {
  BlynkEdgent.run();
}
