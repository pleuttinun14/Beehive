#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3; // ขาเชื่อมต่อ GPS
static const int GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin); // สร้าง Serial สำหรับ GPS

void setup() {
  Serial.begin(115200);
  ss.begin(GPSBaud);
}

void loop() {
  // อ่านข้อมูลจาก GPS
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  // ถ้ามีข้อมูลพิกัด
  if (gps.location.isValid()) {
    Serial.print("Lat: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(", Lng: ");
    Serial.println(gps.location.lng(), 6);
  } else {
    Serial.println("Waiting for GPS signal...");
  }

  // ถ้ามีข้อมูลเวลา
  if (gps.time.isValid()) {
    Serial.print("Time (UTC): ");
    Serial.print(gps.time.hour());
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.println(gps.time.second());
  }

  delay(1000); // หน่วง 1 วินาที
}
