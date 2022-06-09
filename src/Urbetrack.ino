#include "Urbetrack.h"

ESP_Device ESP01(4, 5);
#define delayTime 1000
#define ssid "Upside24"
#define psw "1234223432344234"

void setup() 
{
  delay(delayTime);
  ESP01.init();
  delay(delayTime);
  ESP01.checkConnection();
  delay(delayTime);
  ESP01.connectWifi(ssid, psw);
  delay(delayTime);
  ESP01.checkWifi();
  delay(delayTime);
  ESP01.establishTCP("iot.espressif.cn",8000);
  delay(delayTime);
  ESP01.pingTCP("iot.espressif.cn");
  delay(delayTime);
  ESP01.sendTCP("RadomData");
  delay(delayTime);
  ESP01.closeTCP();
  delay(delayTime);
  ESP01.disconnectWifi();
  delay(delayTime);
}

void loop() 
{
  ESP01.serialRead(200,true);
  ESP01.serialWrite();
}
