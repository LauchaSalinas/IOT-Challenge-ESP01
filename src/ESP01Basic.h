#ifndef ESP01Basic_H
#define ESP01Basic_H

#include <SoftwareSerial.h>
#include <Arduino.h>

class ESP_Device {
  public:
    ESP_Device(int rxPin, int txPin);
    void init();
    bool checkConnection();
    bool connectWifi(String, String);
    bool checkWifi();
    bool disconnectWifi();
    bool establishTCP(String, int);
    bool sendTCP(String);
    bool pingTCP(String);
    bool closeTCP();
    void serialWrite();
    String serialRead(int, boolean);

// In future revisions private variables as SSID and password
// could be stored in flash memory trought EPROM Arduino
// library to avoid entering all the values once ESP01 is 
// resetted.

  private:
    SoftwareSerial mySerial;
    int _rxPin;
    int _txPin;
    String _lastSSID;
    String _lastPSW;
    String sendCommand(String, int, boolean);
};

#endif
