#include <TinyGPS++.h>
#include <HardwareSerial.h>

TinyGPSPlus gps;
HardwareSerial SerialGPS(1);

// ตัวแปรเก็บพิกัด
double latitude = 0.0;
double longitude = 0.0;

void setup() {
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17); 
  // (baudrate, protocol, RX pin, TX pin)
  // RX=16, TX=17 (ปรับตามที่คุณต่อจริง)
}

void loop() {
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read())) {
      if (gps.location.isValid()) {
        latitude  = gps.location.lat();
        longitude = gps.location.lng();
      }
    }
  }

  // ตอนนี้ latitude และ longitude จะอัปเดตตลอด
  // คุณสามารถเอาไปใช้งานในโค้ดส่วนอื่นได้เลย
}
