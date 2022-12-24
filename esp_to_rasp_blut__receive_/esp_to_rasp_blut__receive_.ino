/*
  Check the new incoming messages, and print via serialin 115200 baud rate.
 
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn
 
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/
#include "BluetoothSerial.h"   // library bluethooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
#include "heltec.h"
#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6
BluetoothSerial SerialBT;

void setup() {
    //WIFI Kit series V1 not support Vext control
      Serial.begin(9600);
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  SerialBT.begin("ESP32test");
}
void loop() {
      Serial.print("Received packet ");
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet ");
    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read()); 
    }
    // print RSSI of packet
    Serial.print(" with RSSI ");
    Serial.println(LoRa.packetRssi());
    Serial.print(LoRa.packetRssi());

  }
}
