#include "Urbetrack.h"

ESP_Device ESP01(4, 5);
#define ssid "Upside24"
#define psw "1234223432344234"

void setup() 
{
  ESP01.init();
  ESP01.checkConnection();
  ESP01.connectWifi(ssid, psw);
  ESP01.checkWifi();
  ESP01.pingTCP("iot.espressif.cn");
  ESP01.establishTCP("iot.espressif.cn",8000);
  ESP01.sendTCP("RandomData");
  ESP01.closeTCP();
  ESP01.disconnectWifi();
}

void loop() 
{
  ESP01.serialRead(500,true);
  ESP01.serialWrite();
}
