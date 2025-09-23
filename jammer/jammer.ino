#include <SPI.h>
#include "RF24.h"
#include <ezButton.h>
#include <string>

//Im using nodemcu esp8266
RF24 radio(2, 4);//CE, CSN 1st Radio
RF24 radio2(0, 5);//CE, CSN 2nd Radio
byte i = 45;
ezButton button(3);

const int wifiFrequencies[] = {
  2412,
  2417,
  2422,
  2427,
  2432,
  2437,
  2442,
  2447,
  2452,
  2457,
  2462
};


void displayMessage(const char* line)
{
  Serial.println(line);
}

void addvertising() {
  displayMessage("Jammer got up. Click the button and discover all modes!");
}

void setup() {
  Serial.begin(9600);
  button.setDebounceTime(100);
  pinMode(3, INPUT_PULLUP);
  pinMode(LED_BUILTIN,OUTPUT);
 
  delay(900);
  if (radio.begin()) {
    delay(200);
    radio.setAutoAck(false); 
    radio.stopListening();
    radio.setRetries(0, 0);
    radio.setPayloadSize(5);
    radio.setAddressWidth(3);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);
    radio.setCRCLength(RF24_CRC_DISABLED);
    radio.printPrettyDetails();
    radio.startConstCarrier(RF24_PA_MAX, i);
    Serial.println("1st Radio started!");
    
  } else {
    Serial.println("1st Radio Could not be started");
    displayMessage("Jammer Error!");
  }
  delay(900);
  if (radio2.begin()) {
    delay(200);
    radio2.setAutoAck(false); 
    radio2.stopListening();
    radio2.setRetries(0, 0);
    radio2.setPayloadSize(5);
    radio2.setAddressWidth(3);
    radio2.setPALevel(RF24_PA_MAX);
    radio2.setDataRate(RF24_2MBPS);
    radio2.setCRCLength(RF24_CRC_DISABLED);
    radio2.printPrettyDetails();
    radio2.startConstCarrier(RF24_PA_MAX, i);
    Serial.println("2nd Radio started!");
  } else {
    Serial.println("2nd Radio Could not be started");
    displayMessage("Jammer Error!");
  }
  addvertising();
}

void fullAttack() {
  for (size_t i = 0; i < 80; i++) {
    radio.setChannel(i);
    i++;
    radio2.setChannel(i);
    
  }
}
void wifiAttack() {
  for (int i = 0; i < sizeof(wifiFrequencies) / sizeof(wifiFrequencies[0]); i++) {
    radio.setChannel(wifiFrequencies[i] - 2400);
    
  }
}

const char* modes[] = {
  "BLE & All 2.4 GHz",
  "Just Wi-Fi",
  "Waiting Idly :("
};
uint8_t attack_type = 2;
void loop() {
  button.loop();
  if (button.isPressed()) {
    attack_type = (attack_type + 1) % 3;
    displayMessage("Button Pressed!");
    displayMessage((String(modes[attack_type])+" Mode").c_str());
    for(int h=0;h<=attack_type;h++)
    {
      digitalWrite(LED_BUILTIN, LOW); // Turn the LED on
      delay(500); 
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500); 
    }
  }
  
  switch (attack_type) {
    case 0:
      fullAttack();
      break;
    case 1:
      wifiAttack();
      break;
    case 2:
      break;
  }
}