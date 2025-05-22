#include <main.h>

unsigned long lastSent = 0;

void setup() {
  // Setup awal, dijalankan sekali saat boot
  urusanLED.setWarna(100, 0, 0);

  Wire.begin();
  Serial.begin(SERIAL_BAUD_RATE);
  i2cScanner();

  oled.mulai();
  oled.bersihkan();

  cetakIdentitasDeveloper();

  urusanWiFi.konek();
  while (urusanWiFi.apakahKonek() == 0) {
    Serial.print(".");
    delay(1000);
  }

  urusanLED.setWarna(0, 100, 0);
  buzzer.beep(50, 3);
  buzzer.beep(100, 2);

  // Koneksi MQTT
  urusanIoT.konek();
  urusanIoT.penangkapPesan(penangkapPesan);

  if (urusanIoT.apakahKonek() == 1) {
    subscribe();
  }
}

void loop() {
  // Jalankan proses MQTT
  urusanIoT.proses();

  // Kirim pesan dan tampilkan di OLED setiap 1 detik
  if (millis() - lastSent > 1000) {
    String pesan = String(NAMA_LENGKAP) + " - " + String(NIM) + " - HADIR";

    // Kirim ke MQTT
    urusanIoT.publish("undiknas/psti/display/update", pesan);

    // Tampilkan ke OLED
    oled.bersihkan();
    oled.tambahTeks(0, 0, "%s", pesan.c_str());
    oled.tampilkan();

    lastSent = millis();
  }
}

void penangkapPesan(String topic, String message) {
  Serial.printf("penangkapPesan: topic: %s | message: %s\r\n", topic.c_str(), message.c_str());
  oled.bersihkan();
  oled.tambahTeks(0, 0, "%s", message.c_str());
  oled.tampilkan();
}

// Menampilkan identitas developer di awal
void cetakIdentitasDeveloper() {
  oled.tambahTeks(0, 0, "%d", NIM);
  oled.tampilkan();
  oled.tambahTeks(0, 10, "%s", NAMA_LENGKAP);
  oled.tampilkan();
  oled.tambahTeks(0, 20, "%s", NAMA_KELOMPOK);
  oled.tampilkan();
  oled.tambahTeks(0, 30, "%s %s", CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION);
  oled.tampilkan();
}

// Subscribe ke topik MQTT
void subscribe() {
  urusanIoT.subscribe("undiknas/psti/display");
  urusanIoT.subscribe("undiknas/psti/display/update");
}

// Scanner I2C untuk mendeteksi perangkat
void i2cScanner() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
}
