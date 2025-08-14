#include <TinyGPS++.h>
#include <ArduinoJson.h>
#include <LoRa.h>
#include <SPI.h>
#include <HardwareSerial.h>

// Pin definitions
#define GPS_RX 16   
#define GPS_TX 17   
#define LORA_SS 18  
#define LORA_RST 14 
#define LORA_DIO0 26
#define LORA_BAND 923E6 

// Device configuration
#define DEVICE_ID "GPS_001"
#define MIN_SATELLITES 4
#define MAX_HDOP 10.0

TinyGPSPlus gps;
HardwareSerial gpsSerial(1); 

unsigned long lastSendTime = 0;
const unsigned long interval = 20UL * 60UL * 1000UL; // 20 นาที
unsigned long totalPacketsSent = 0;

void setup() {
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  SPI.begin();
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  while (!LoRa.begin(LORA_BAND)) {
    delay(1000);
  }

  LoRa.setTxPower(20);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(8);
  LoRa.enableCrc();
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
  
  if (millis() - lastSendTime >= interval) {
    if (isValidGPSFix()) {
      String jsonString = createJson(
        gps.location.lat(),
        gps.location.lng(),
        gps.altitude.meters(),
        gps.time.hour(),
        gps.time.minute(),
        gps.time.second(),
        gps.satellites.value(),
        gps.hdop.hdop(),
        gps.speed.kmph()
      );
      sendLoRaPacket(jsonString);
      totalPacketsSent++;
    }
    lastSendTime = millis();
  }
}

bool isValidGPSFix() {
  return gps.location.isValid() && 
         gps.time.isValid() && 
         gps.satellites.isValid() &&
         gps.hdop.isValid() &&
         gps.satellites.value() >= MIN_SATELLITES && 
         gps.hdop.hdop() <= MAX_HDOP;
}

void sendLoRaPacket(String data) {
  if (data.length() <= 200) {
    LoRa.beginPacket();
    LoRa.print(data);
    LoRa.endPacket();
  }
}

String createJson(double lat, double lng, double alt, int hour, int minute, int second, 
                  int satellites, double hdop, double speed) {
  StaticJsonDocument<300> doc;
  
  doc["device_id"] = DEVICE_ID;
  doc["timestamp"] = millis();
  doc["latitude"] = serialized(String(lat, 6));
  doc["longitude"] = serialized(String(lng, 6));
  doc["altitude"] = serialized(String(alt, 1));

  char timeStr[9];
  sprintf(timeStr, "%02d:%02d:%02d", hour, minute, second);
  doc["time_utc"] = timeStr;
  
  doc["satellites"] = satellites;
  doc["hdop"] = serialized(String(hdop, 2));
  doc["speed_kmh"] = serialized(String(speed, 1));
  doc["packet_count"] = totalPacketsSent + 1;
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  return jsonString;
}
