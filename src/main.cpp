#include <main.h>


void setup() {
  // tulis setup kode mu di sini, untuk dijalankan sekali saja:  
  Serial.begin(115200);
  cetakIdentitasDeveloper();


  Wire.begin();

  

  urusanWiFi.konek();
  while(urusanWiFi.apakahKonek() == 0){
    Serial.print(".");
  }


  urusanIoT.konek();
  urusanIoT.penangkapPesan(penangkapPesan);


  if(urusanIoT.apakahKonek() == 1){
    subscribe();
  }


  lightMeter.begin();


  urusanLED.setWarna(100, 100, 100);
  delay(3000);
  urusanLED.setWarna(0, 0, 0);
}


static unsigned long lastUpdateMillis = 0;
void loop() {
  // tulis kode utama mu di sini, untuk dijalankan berulang-ulang :
  urusanIoT.proses();


  if(urusanIoT.apakahKonek() == 0){
    Serial.println("UrusanIoT: Koneksi terputus, mencoba untuk menyambung kembali...");
    urusanIoT.konek();
    if(urusanIoT.apakahKonek() == 1){
      subscribe();
    }
  }


  if (millis() - lastUpdateMillis >= 1000) {
    lastUpdateMillis = millis();


    lastLux = lightMeter.readLightLevel();


    String payload;
    JsonDocument doc;
    doc["lux"] = lastLux;
    serializeJson(doc, payload);


    urusanIoT.publish("syntaxg/sudarsan/status", payload);
  }


  // make the led blink if lastLux is outside bottomThreshold and upperThreshold
  if (lastLux < bottomThreshold || lastLux > upperThreshold) {
    urusanLED.setWarna(100, 100, 100); // LED nyala
  } else {
    urusanLED.setWarna(0, 0, 0); // LED padam
  }
  delay(10);
}


void subscribe() {
  urusanIoT.subscribe("syntaxg/sudarsan/command");
}


void penangkapPesan(String topic, String message){
  Serial.printf("penangkapPesan: topic: %s | message: %s\r\n", topic.c_str(), message.c_str());
  JsonDocument dataMasuk;
  DeserializationError galatParseJson = deserializeJson(dataMasuk, message);
  if(galatParseJson == DeserializationError::Ok){
    if(!dataMasuk["bottomThreshold"].isNull() &&
    !dataMasuk["upperThreshold"].isNull()){
      bottomThreshold = dataMasuk["bottomThreshold"].as<int>();
      upperThreshold = dataMasuk["upperThreshold"].as<int>();
    }
  }
  else
  {
    Serial.printf("penangkapPesan: Gagal parse JSON: %s\r\n", galatParseJson.c_str());
  }
}


// tulis definisi fungsi mu di sini:
void cetakIdentitasDeveloper() {
  Serial.printf("NIM: %d\r\n", NIM);  
  Serial.printf("Nama: %s\r\n", NAMA_LENGKAP);
  Serial.printf("Grup: %s\r\n", NAMA_KELOMPOK);
  Serial.printf("Firmware: %s %s\r\n", CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION);
}


