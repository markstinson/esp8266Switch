#include <Bounce2.h>
#include <Homie.h>
#include <Button.h>

const int PIN_RELAY = 15;
const int PIN_LED = 2;
const int PIN_BUTTON = 13;


#define FW_NAME "ecoplug"
#define FW_VERSION "1.0.2"

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

HomieNode switchNode("plug", "switch");
Button button1(PIN_BUTTON); // Connect your button between pin 2 and GND

static uint8_t lightState = LOW;

void setLightState(uint8_t newLightState) {
  if(newLightState == HIGH) {
    digitalWrite(PIN_RELAY, HIGH);
    switchNode.setProperty("on").send("true");
    Serial.println("Light is on");
    lightState = HIGH;
  } else if(newLightState == LOW) {
    digitalWrite(PIN_RELAY, LOW);
    switchNode.setProperty("on").send("false");
    Serial.println("Light is off");    
    lightState = LOW;
  }
}

void invertLightState() {
  if(lightState == LOW) {
    setLightState(HIGH);
  } else if(lightState == HIGH) {
    setLightState(LOW);
  }
}

bool lightOnHandler(HomieRange range, String value) {
  if (value == "true") {
    setLightState(HIGH);
  } else if (value == "false") {
    setLightState(LOW);
  } else {
    Serial.print("Error Got: ");
    Serial.println(value);
    return false;
  }

  return true;
}



void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println("ecoplug");
  pinMode(PIN_BUTTON,INPUT_PULLUP);
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);
  Homie.setLedPin(PIN_LED, LOW);
  Homie.setResetTrigger(PIN_BUTTON, LOW, 5000);
  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie_setBrand("ecoplug");
  switchNode.advertise("on").settable(lightOnHandler);
  button1.begin();
  Homie.setup();
}

void loop() {
  uint8_t digitalReadTemp;
  
  Homie.loop();
  if (button1.pressed())
  {
    invertLightState();
  }
  

}
