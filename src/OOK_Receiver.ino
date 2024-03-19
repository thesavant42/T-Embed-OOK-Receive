/*
 Basic rtl_433_ESP example for OOK/ASK Devices
*/

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoLog.h>
// Load custom rtl_433_ESP, needs to be on HSPI, not VSPI (default)
#include <rtl_433_ESP.h>
#include <EEPROM.h>
//added TFTHelper.h
#include <vector>
#include <TFT_eSPI.h>
#include "TFTHelper.h"
#include <OneButton.h>
// SPIFFS fs for fonts
#include <FS.h>
#include <WiFi.h>
#include "WiFiClientSecure.h"
// adafruitio
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "secrets.h"
#include <HardwareSerial.h>
#include <TinyGPSPlus.h>
#include <iostream>
#include <string>


static const uint32_t GPSBaud = 4800;
//SoftwareSerial ss(43, 44); // RX, TX
#define RXD2 44
#define TXD2 43
#define Publish FALSE

TinyGPSPlus gps;

#ifndef RF_MODULE_FREQUENCY
#  define RF_MODULE_FREQUENCY 433.92
#endif

#define JSON_MSG_BUFFER 512
#define AA_FONT_SMALL "NotoSansBold15"
#define AIO_SERVER  "io.adafruit.com"
#define AIO_SERVERPORT  8883
#define eepromsize 4096

int ledState = LOW;

OneButton button(BUTTON1, true);

// io.adafruit.com root CA
const char* adafruitio_root_ca = \
      "-----BEGIN CERTIFICATE-----\n"
      "MIIEjTCCA3WgAwIBAgIQDQd4KhM/xvmlcpbhMf/ReTANBgkqhkiG9w0BAQsFADBh\n"
      "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
      "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n"
      "MjAeFw0xNzExMDIxMjIzMzdaFw0yNzExMDIxMjIzMzdaMGAxCzAJBgNVBAYTAlVT\n"
      "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
      "b20xHzAdBgNVBAMTFkdlb1RydXN0IFRMUyBSU0EgQ0EgRzEwggEiMA0GCSqGSIb3\n"
      "DQEBAQUAA4IBDwAwggEKAoIBAQC+F+jsvikKy/65LWEx/TMkCDIuWegh1Ngwvm4Q\n"
      "yISgP7oU5d79eoySG3vOhC3w/3jEMuipoH1fBtp7m0tTpsYbAhch4XA7rfuD6whU\n"
      "gajeErLVxoiWMPkC/DnUvbgi74BJmdBiuGHQSd7LwsuXpTEGG9fYXcbTVN5SATYq\n"
      "DfbexbYxTMwVJWoVb6lrBEgM3gBBqiiAiy800xu1Nq07JdCIQkBsNpFtZbIZhsDS\n"
      "fzlGWP4wEmBQ3O67c+ZXkFr2DcrXBEtHam80Gp2SNhou2U5U7UesDL/xgLK6/0d7\n"
      "6TnEVMSUVJkZ8VeZr+IUIlvoLrtjLbqugb0T3OYXW+CQU0kBAgMBAAGjggFAMIIB\n"
      "PDAdBgNVHQ4EFgQUlE/UXYvkpOKmgP792PkA76O+AlcwHwYDVR0jBBgwFoAUTiJU\n"
      "IBiV5uNu5g/6+rkS7QYXjzkwDgYDVR0PAQH/BAQDAgGGMB0GA1UdJQQWMBQGCCsG\n"
      "AQUFBwMBBggrBgEFBQcDAjASBgNVHRMBAf8ECDAGAQH/AgEAMDQGCCsGAQUFBwEB\n"
      "BCgwJjAkBggrBgEFBQcwAYYYaHR0cDovL29jc3AuZGlnaWNlcnQuY29tMEIGA1Ud\n"
      "HwQ7MDkwN6A1oDOGMWh0dHA6Ly9jcmwzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydEds\n"
      "b2JhbFJvb3RHMi5jcmwwPQYDVR0gBDYwNDAyBgRVHSAAMCowKAYIKwYBBQUHAgEW\n"
      "HGh0dHBzOi8vd3d3LmRpZ2ljZXJ0LmNvbS9DUFMwDQYJKoZIhvcNAQELBQADggEB\n"
      "AIIcBDqC6cWpyGUSXAjjAcYwsK4iiGF7KweG97i1RJz1kwZhRoo6orU1JtBYnjzB\n"
      "c4+/sXmnHJk3mlPyL1xuIAt9sMeC7+vreRIF5wFBC0MCN5sbHwhNN1JzKbifNeP5\n"
      "ozpZdQFmkCo+neBiKR6HqIA+LMTMCMMuv2khGGuPHmtDze4GmEGZtYLyF8EQpa5Y\n"
      "jPuV6k2Cr/N3XxFpT3hRpt/3usU/Zb9wfKPtWpoznZ4/44c1p9rzFcZYrWkj3A+7\n"
      "TNBJE0GmP2fhXhP1D/XVfIW/h0yCJGEiV9Glm/uGOa3DXHlmbAcxSyCRraG+ZBkA\n"
      "7h4SeM6Y8l/7MBRpPCz6l8Y=\n"
      "-----END CERTIFICATE-----\n";


String gpsLat, gpsLng, payload, myLocation, Newpayload;



void EnterSleep();


// WiFiFlientSecure for SSL/TLS support
WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Using port 8883 for MQTTS
#define AIO_SERVERPORT  8883

/****************************** Feeds ***************************************/

// Setup a feed called 'test' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>

Adafruit_MQTT_Publish tembed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/tembed");


//SemaphoreHandle_t radioLock;

char messageBuffer[JSON_MSG_BUFFER];

rtl_433_ESP rf; // use -1 to disable transmitter

int count = 0;

// Handler function for a single click:
static void handleClick() {
  Serial.println("Clicked!");
}

void buttonLoop(void *)
{
    while (1) {
        button.tick();
        delay(5);
    }
}

void rtl_433_Callback(char* message) {
  DynamicJsonBuffer jsonBuffer2(JSON_MSG_BUFFER);
  JsonObject& RFrtl_433_ESPdata = jsonBuffer2.parseObject(message);
  logJson(RFrtl_433_ESPdata);
  count++;
}

void logJson(JsonObject& jsondata) {
  gpsLat = gps.location.lat(), 12;
  gpsLng = gps.location.lng(), 12;
  myLocation = gpsLat; myLocation += ","; myLocation += gpsLng;
  /// myLocation; 
#if defined(ESP8266) || defined(ESP32) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)
  char JSONmessageBuffer[jsondata.measureLength() + 1];
#else
  char JSONmessageBuffer[JSON_MSG_BUFFER];
#endif
  jsondata.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
#if defined(setBitrate) || defined(setFreqDev) || defined(setRxBW)
  Log.setShowLevel(false);
  Log.notice(F("."));
  Log.setShowLevel(true);
#else
  Log.notice(F("Received message: %s" CR), JSONmessageBuffer);
  //GPS Logs
  Log.notice(F("Location: %s" CR), myLocation);
  // clear the screen and prepare the tft for text
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  // print message and location
  tft.println(JSONmessageBuffer);
  //tft.print(gps.location.lat(), 6); tft.print(F(",")); tft.print(gps.location.lng(), 6);
  tft.print(myLocation);
  
  //// Begin ADAIO Logging Function
  //Serial.print(F("\nSending val "));
  //Serial.print(JSONmessageBuffer);
  //Serial.print(F(" to tembed feed..."));
  //if (! tembed.publish(JSONmessageBuffer)) {
  //  Serial.println(F("Failed"));
  //} else {
  //  Serial.println(F("OK!"));
  //}

   //wait a couple seconds to avoid rate limit
  delay(5000);
#endif
}

void setup() {
  Serial.begin(921600);
  power_management();
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(1000);
   Serial.println("Loopback program started");
  //ss.begin(4800);
  if (!SPIFFS.begin()) {
      Serial.println("SPIFFS initialisation failed!");
      while (1) yield(); // Stay here twiddling thumbs waiting
    }
  Serial.println("\r\nSPIFFS available!");
  
  // ESP32 will crash if any of the fonts are missing
  bool font_missing = false;
  if (SPIFFS.exists("/NotoSansBold15.vlw")    == false) font_missing = true;
 
  if (font_missing)
    {
      Serial.println("\r\nFont missing in SPIFFS, did you upload it?");
      while(1) yield();
    }
    else Serial.println("\r\nFonts found OK."); 
  // Initialize display screen
  initTFT();
  pinMode(BOARD_TFT_BL, OUTPUT);
  digitalWrite(BOARD_TFT_BL, HIGH);
  tft.setRotation(1);
  tft.setCursor(0, 0, 4);
  tft.loadFont(AA_FONT_SMALL); // Must load the font first
  // Set the font colour to be white with a black background
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  // We can now plot text on screen using the "print" class
  //tft.println("Intialized default\n");
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0, 4);
  // init radios
  initHSPI();
  // new changes
  //Serial.begin(921600);
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  tft.print("Connecting to: ");tft.println(WLAN_SSID);
  delay(1000);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  delay(2000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  tft.print("Wifi connected :"); tft.println(WiFi.localIP());
  // Set Adafruit IO's root CA
  client.setCACert(adafruitio_root_ca);
  delay(3000);
  #ifndef LOG_LEVEL
    LOG_LEVEL_SILENT
  #endif
    Log.begin(LOG_LEVEL, &Serial);

  Log.notice(F(" " CR));
  Log.notice(F("****** setup ******" CR));
  rf.initReceiver(RF_MODULE_RECEIVER_GPIO, RF_MODULE_FREQUENCY);
  rf.setCallback(rtl_433_Callback, messageBuffer, JSON_MSG_BUFFER);
  rf.enableReceiver();
  Log.notice(F("****** setup complete ******" CR));
  rf.getModuleStatus();
  button.attachDoubleClick(doubleClick);
  esp_sleep_enable_ext1_wakeup(((uint64_t)(((uint64_t)1) << BUTTON_1)), ESP_EXT1_WAKEUP_ANY_LOW);
  }

uint32_t x=0;

unsigned long uptime() {
  static unsigned long lastUptime = 0;
  static unsigned long uptimeAdd = 0;
  unsigned long uptime = millis() / 1000 + uptimeAdd;
  if (uptime < lastUptime) {
    uptime += 4294967;
    uptimeAdd += 4294967;
  }
  lastUptime = uptime;
  return uptime;
}

int next = uptime() + 30;

#if defined(setBitrate) || defined(setFreqDev) || defined(setRxBW)

#  ifdef setBitrate
#    define TEST    "setBitrate" // 17.24 was suggested
#    define STEP    2
#    define stepMin 1
#    define stepMax 300
// #    define STEP    1
// #    define stepMin 133
// #    define stepMax 138
#  elif defined(setFreqDev) // 40 kHz was suggested
#    define TEST    "setFrequencyDeviation"
#    define STEP    1
#    define stepMin 5
#    define stepMax 200
#  elif defined(setRxBW)
#    define TEST "setRxBandwidth"

#    ifdef defined(RF_SX1276) || defined(RF_SX1278)
#      define STEP    5
#      define stepMin 5
#      define stepMax 250
#    else
#      define STEP    5
#      define stepMin 58
#      define stepMax 812
// #      define STEP    0.01
// #      define stepMin 202.00
// #      define stepMax 205.00
#    endif
#  endif
float step = stepMin;
#endif

void loop() {
  button.tick();
  // This loop displays information every time a new sentence is correctly encoded.
  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      //displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  MQTT_connect();
  rf.loop();
  //printDateTime(gps.date, gps.time);
#if defined(setBitrate) || defined(setFreqDev) || defined(setRxBW)
  char stepPrint[8];
  if (uptime() > next) {
    next = uptime() + 120; // 60 seconds
    dtostrf(step, 7, 2, stepPrint);
    Log.notice(F(CR "Finished %s: %s, count: %d" CR), TEST, stepPrint, count);
    step += STEP;
    if (step > stepMax) {
      step = stepMin;
    }
    dtostrf(step, 7, 2, stepPrint);
    Log.notice(F("Starting %s with %s" CR), TEST, stepPrint);
    count = 0;

    int16_t state = 0;
#  ifdef setBitrate
    state = rf.setBitRate(step);
    RADIOLIB_STATE(state, TEST);
#  elif defined(setFreqDev)
    state = rf.setFrequencyDeviation(step);
    RADIOLIB_STATE(state, TEST);
#  elif defined(setRxBW)
    state = rf.setRxBandwidth(step);
    if ((state) != RADIOLIB_ERR_NONE) {
      Log.notice(F(CR "Setting  %s: to %s, failed" CR), TEST, stepPrint);
      next = uptime() - 1;
    }
#  endif

    rf.receiveDirect();
    // rf.getModuleStatus();
  }
#endif
 
}
//}

void EnterSleep()
{
    //tft.setRotation(2);
    delay(1000);
    Serial.println("EnterSleep");
    //tft.print("EnterSleep"); 
    //tft.update();
    delay(2000);
    //
    esp_deep_sleep_start();
    /*Turn on power control*/
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }

  Serial.println("MQTT Connected!");
}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

void power_management() {
  EEPROM.begin(eepromsize);
  byte z = EEPROM.read(eepromsize-2);

  if (digitalRead(BUTTON1) != LOW) {
    if (z == 1){
      go_deep_sleep();
    }
  } else {
    if (z == 0) {
      EEPROM.write(eepromsize-2, 1);
      EEPROM.commit();
      go_deep_sleep();
    } else {
      EEPROM.write(eepromsize-2, 0);
      EEPROM.commit();
    }
  }
}

void go_deep_sleep() {
  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
}

//void force_reset() {
//  esp_task_wdt_init(1,true);
//  esp_task_wdt_add(NULL);
//  while(true);
//}

void doubleClick()
{
  Serial.println("x2 Enter Sleep");
  EnterSleep();
} // doubleClick