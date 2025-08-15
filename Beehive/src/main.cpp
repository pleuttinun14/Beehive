#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// กำหนดพินสำหรับ GPS
static const int RXPin = 4, TXPin = 3;
static const int GPSBaud = 9600;

// ออบเจกต์ TinyGPS++
TinyGPSPlus gps;

// Serial สำหรับ GPS
SoftwareSerial ss(RXPin, TXPin);

// ตัวแปรเก็บค่าที่อ่านได้
double latitude = 0.0;
double longitude = 0.0;
byte hour_gps = 0;
byte minute_gps = 0;
byte second_gps = 0;
byte day_gps = 0;
byte month_gps = 0;
int year_gps = 0;

void setup()
{
  Serial.begin(115200); // debug ถ้าจำเป็น
  ss.begin(GPSBaud);

  // รอจนกว่าจะได้ค่าจริงจาก GPS
  while (true)
  {
    while (ss.available() > 0)
    {
      gps.encode(ss.read());
    }

    // ตรวจสอบว่าค่าพร้อมครบทุกอย่างหรือยัง
    if (gps.location.isValid() &&
        gps.time.isValid() &&
        gps.date.isValid())
    {
      // อัปเดตตัวแปร
      latitude = gps.location.lat();
      longitude = gps.location.lng();
      hour_gps = gps.time.hour();
      minute_gps = gps.time.minute();
      second_gps = gps.time.second();
      day_gps = gps.date.day();
      month_gps = gps.date.month();
      year_gps = gps.date.year();

      break; // ออกจาก while(true) เมื่อได้ค่าครบ
    }
  }
}

void loop()
{
  // ใน loop() ก็สามารถอัปเดตค่าต่อได้ตามต้องการ
  while (ss.available() > 0)
  {
    gps.encode(ss.read());
  }

  if (gps.location.isValid())
  {
    latitude = gps.location.lat();
    longitude = gps.location.lng();
  }

  if (gps.time.isValid())
  {
    hour_gps = gps.time.hour();
    minute_gps = gps.time.minute();
    second_gps = gps.time.second();
  }

  if (gps.date.isValid())
  {
    day_gps = gps.date.day();
    month_gps = gps.date.month();
    year_gps = gps.date.year();
  }

  // ไม่มีการ print ออกมา
}
