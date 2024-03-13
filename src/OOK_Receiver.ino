/*
 Basic rtl_433_ESP example for OOK/ASK Devices
*/

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoLog.h>
// rtl_433_ESP needs to be on HSPI, not VSPI (default)
#include <rtl_433_ESP.h>
//added TFTHelper.h
#include "TFTHelper.h"
#include <OneButton.h>

//#include <images.h>

#ifndef RF_MODULE_FREQUENCY
#  define RF_MODULE_FREQUENCY 433.92
#endif

#define JSON_MSG_BUFFER 512

TFT_eSprite spr = TFT_eSprite(&tft);
TFT_eSprite sprTitle = TFT_eSprite(&tft);
TFT_eSprite nfcSpr = TFT_eSprite(&tft);
OneButton button(BOARD_ENCODE_CENTEN_PIN, true);

int spr_w = 0;
int spr_h = 0;
int spr_start_x = 0;
int spr_start_y = 0;
int start_x ;
int title_h ;
SemaphoreHandle_t radioLock;

char messageBuffer[JSON_MSG_BUFFER];

rtl_433_ESP rf; // use -1 to disable transmitter

int count = 0;

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
  Log.notice(F("Received message : %s" CR), JSONmessageBuffer);
  tft.print("Received message : %s" CR), JSONmessageBuffer;
#endif
}

void setup() {
    // BEGIN SAVANT CHANGE BLOCK
    // Initialize display screen
    initTFT();
    spr_w = tft.width();
    spr_h = tft.width() / 3;
    // init radios
    initHSPI();
    // END SAVANT CHANGE BLOCK
  Serial.begin(921600);
  delay(1000);
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
}

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
  rf.loop();
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
