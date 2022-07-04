#include "ESP01Basic.h"

// Defined some default timeout variables for quick editing
#define shortTimeout 800
#define mediumTimeout 1500
#define wifiTimeout 8000

// Set baud rate for both devices (Arduino and ESP)
#define baudrate 115200

//
boolean _debug = true;

ESP_Device::ESP_Device(int rxPin, int txPin)
  : mySerial(rxPin, txPin) {}

void ESP_Device::init()
{
  delay(shortTimeout);
  Serial.begin(baudrate);
  mySerial.begin(baudrate);
}

// Rest of the methods have a Serial.Print to acknowledge the result in the Serial COM.
// Since functions return a bool, code could be cleanerd and make a separate class for
// all the notification messages in a future revision. It has been done this way to
// keep the main as simply as possible.

bool ESP_Device::checkConnection()
{
  String response = sendCommand("AT", shortTimeout, _debug);
  if (response.indexOf("OK") > 0)
  {
    Serial.println("CONNECTION WITH ESP01 SUCCESFUL");
    return true;
  }
  return false;
}

bool ESP_Device::connectWifi(String ssid, String psw)
{
  String response = sendCommand("AT+CWMODE=1", shortTimeout, _debug);

  if (response.indexOf("OK") > 0)
  {
    response = sendCommand("AT+CWJAP=\"" + ssid + "\",\"" + psw + "\"", wifiTimeout, _debug);
    while (response.indexOf("OK") <= 0)
    {
      response = sendCommand("AT+CWJAP=\"" + ssid + "\",\"" + psw + "\"", wifiTimeout, _debug);
    }
  }
  if (response.indexOf("OK") > 0)
  {
    _lastSSID = ssid;
    _lastPSW = psw;
    Serial.println("ESP01 IS NOW CONNECTED TO " + ssid + " WIFI");
    return true;
  }
  return false;
}

bool ESP_Device::checkWifi() {

  String response = sendCommand("AT+CWJAP?", shortTimeout, _debug);
  if (response.indexOf("OK") > 0) {
    Serial.println("WIFI STATUS: CONNECTED");
    return true;
  }
  return false;

}

bool ESP_Device::disconnectWifi()
{
  String response = sendCommand("AT+CWQAP", shortTimeout, _debug);
  if (response.indexOf("OK") > 0)
  {
    Serial.println("WIFI DISCONNECTED");
    return true;
  }

  return false;
}

bool ESP_Device::establishTCP(String ip, int port)
{
  String response = sendCommand("AT+CIPSTART=\"TCP\",\"" + ip + "\"," + port, mediumTimeout, _debug);
  if (response.indexOf("OK") > 0)
  {
    Serial.println("TCP CONNECTION TO: " + ip + " WAS ESTABLISHED.");
    return true;
  }
  return false;
}

bool ESP_Device::sendTCP(String packet)
{
  int i = packet.length() + 2;
  String response = sendCommand("AT+CIPSEND=" + String(i), mediumTimeout, _debug);
  if (response.indexOf(">") > 0)
  {
    response = sendCommand(packet, mediumTimeout, _debug);
  }

  if (response.indexOf("SEND OK") > 0)
  {
    Serial.println("TCP PACKET WAS SENT OK");
    return true;
  }
  return false;
}

bool ESP_Device::pingTCP(String ip)
{
  String response = sendCommand("AT+PING=\"" + ip + "\"", mediumTimeout, _debug);
  if (response.indexOf("+") > 0)
  {
    Serial.println("PING TO: " + ip + " REPLIED WITH SUCCESS.");
    return true;
  }
  return false;
}

bool ESP_Device::closeTCP()
{
  String response = sendCommand("AT+CIPCLOSE", shortTimeout, _debug);
  if (response.indexOf("OK") > 0)
  {
    Serial.println("TCP CONNECTION CLOSED.");
    return true;
  }
  return false;
}

String ESP_Device::sendCommand(String command, int timeout, boolean debug)
{
  int i = 0;
  for (i = 0; command.length() > i; i++)
  {
    byte b = command[i];
    mySerial.write(b);
    delay(shortTimeout / 5);
  }
  mySerial.println("");

  String response = "";
  delay(timeout);
  while (mySerial.available())
  {
    char c = mySerial.read();
    response += c;
  }

  if (debug) Serial.print(response);
  return response;
}

// These two functions act as a debugger / tester for the void loop()
// to interact with the ESP01 via COM Serial window once all the test
// functions in void setup() were excecuted.
void ESP_Device::serialWrite()
{
  while (Serial.available())
  {
    mySerial.write(Serial.read());
    delay(shortTimeout / 5);
  }
}

String ESP_Device::serialRead(int timeout, boolean debug)
{
  String response = "";
  long int time = millis();
  delay(timeout);
  while (mySerial.available())
  {
    char c = mySerial.read();
    response += c;
  }

  if (debug) Serial.print(response);
  return response;
}
