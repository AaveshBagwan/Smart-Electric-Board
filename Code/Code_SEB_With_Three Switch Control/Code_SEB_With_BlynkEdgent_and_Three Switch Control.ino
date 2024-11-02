#define BLYNK_TEMPLATE_ID "TMPL3d5AULOv7"
#define BLYNK_TEMPLATE_NAME "Smart Electric Board"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"

// Define the pin for the relay
#define RELAY_PIN_D1 D1
#define RELAY_PIN_D2 D2
#define RELAY_PIN_D5 D5

// Virtual pins / datastreams
#define NUMBER_OF_VIRTUAL_PIN 3
#define VIRTUAL_PIN_V0 V0
#define VIRTUAL_PIN_V1 V1
#define VIRTUAL_PIN_V2 V2

int highVirtualPinState = 1;
const unsigned long DISABLE_DURATION = 2500;
int virtualPins[NUMBER_OF_VIRTUAL_PIN] = {VIRTUAL_PIN_V0,VIRTUAL_PIN_V1,VIRTUAL_PIN_V2};
bool isVirtualPinEnabled[NUMBER_OF_VIRTUAL_PIN] = {true,true,true};
unsigned long timeWhenDisabled[NUMBER_OF_VIRTUAL_PIN] = {0,0,0};
bool setDigitalAndVirutalPinFlag = true;

BLYNK_CONNECTED(){
  Serial.println("Connected to blynk cloud succesfully");
  if(setDigitalAndVirutalPinFlag){
    // Ensure all buttons are enabled
    Blynk.setProperty(VIRTUAL_PIN_V0, "isDisabled", "false");
    Blynk.setProperty(VIRTUAL_PIN_V1, "isDisabled", "false");
    Blynk.setProperty(VIRTUAL_PIN_V2, "isDisabled", "false");
    Serial.println("Sets virtual pins as enabled......");

    // Ensure virtual pin is ON initially
    Blynk.virtualWrite(VIRTUAL_PIN_V0, highVirtualPinState);
    Blynk.virtualWrite(VIRTUAL_PIN_V1, highVirtualPinState); 
    Blynk.virtualWrite(VIRTUAL_PIN_V2, highVirtualPinState); 
    Serial.println("Sets virtual pins as high......");

    // Ensure relay is ON initially
    digitalWrite(RELAY_PIN_D1, HIGH); 
    digitalWrite(RELAY_PIN_D2, HIGH); 
    digitalWrite(RELAY_PIN_D5, HIGH); 
    Serial.println("Sets relay pins as high......");

    setDigitalAndVirutalPinFlag=false;
  }
  else{
    Blynk.syncAll();
  }
}

BLYNK_DISCONNECTED() {
  Serial.println("Disconnected from Blynk server");
}

BLYNK_WRITE(V0) { // V0 corresponds to the Switch1 button widget in the app
  int buttonState = param.asInt(); // Get button state
  Serial.print("Received value VO for RELAY_PIN_D1: ");
  Serial.println(buttonState);
  if (buttonState == 1) {
    Serial.println("Turning RELAY_PIN_D1 ON");
    digitalWrite(RELAY_PIN_D1, HIGH); // Turn relay ON
  } else {
    Serial.println("Turning RELAY_PIN_D1 OFF");
    digitalWrite(RELAY_PIN_D1, LOW); // Turn relay OFF
  }
  Serial.println("Disabling V0");
  disableButton(0);
}

BLYNK_WRITE(V1) { // V1 corresponds to the Switch2 button widget in the app
  int buttonState = param.asInt(); // Get button state
  Serial.print("Received value V1 for RELAY_PIN_D2: ");
  Serial.println(buttonState);
  if (buttonState == 1) {
    Serial.println("Turning RELAY_PIN_D2 ON");
    digitalWrite(RELAY_PIN_D2, HIGH); // Turn relay ON
  } else {
    Serial.println("Turning RELAY_PIN_D2 OFF");
    digitalWrite(RELAY_PIN_D2, LOW); // Turn relay OFF
  }
  Serial.println("Disabling V1");
  disableButton(1);
}

BLYNK_WRITE(V2) { // V2 corresponds to the Switch3 button widget in the app
  int buttonState = param.asInt(); // Get button state
  Serial.print("Received value V2 for RELAY_PIN_D5: ");
  Serial.println(buttonState);
  if (buttonState == 1) {
    Serial.println("Turning RELAY_PIN_D5 ON");
    digitalWrite(RELAY_PIN_D5, HIGH); // Turn relay ON
  } else {
    Serial.println("Turning RELAY_PIN_D5 OFF");
    digitalWrite(RELAY_PIN_D5, LOW); // Turn relay OFF
  }
  Serial.println("Disabling V2");
  disableButton(2);
}


void setup()
{
  Serial.println("Setup function begins......");
  Serial.begin(115200);
  delay(100);

  // Set the relay pin as output
  pinMode(RELAY_PIN_D1, OUTPUT);
  pinMode(RELAY_PIN_D2, OUTPUT);
  pinMode(RELAY_PIN_D5, OUTPUT);
  Serial.println("Sets relay pins as output......");

  if(!setDigitalAndVirutalPinFlag){
    setDigitalAndVirutalPinFlag=true;
  }

  BlynkEdgent.begin();
  Serial.println("Setup function ends......");
}

void loop() {
  // Serial.println("Loop started......");
  // Run Blynk
  BlynkEdgent.run();

  // Enable Button
  for(int i=0 ; i<NUMBER_OF_VIRTUAL_PIN; i++){
    if(!isVirtualPinEnabled[i]){
      if((millis()-timeWhenDisabled[i])>=DISABLE_DURATION){
       enableButton(i);
      }
    }
  }
  // Serial.println("Loop ended......");
}

void disableButton(int virtualPin){
  Blynk.setProperty(virtualPins[virtualPin], "isDisabled", "true");
  isVirtualPinEnabled[virtualPin] = false;
  timeWhenDisabled[virtualPin] = millis();
  Serial.print(virtualPins[virtualPin]);
  Serial.print(" Button is Disabled at time ");
  Serial.println(timeWhenDisabled[virtualPin]);
}

void enableButton(int virtualPin){
   Blynk.setProperty(virtualPins[virtualPin], "isDisabled", "false");
   isVirtualPinEnabled[virtualPin] = true;
   timeWhenDisabled[virtualPin] = 0;
   Serial.print(virtualPins[virtualPin]);
   Serial.print(" Button is Enabled at time ");
   Serial.println(millis());
}

