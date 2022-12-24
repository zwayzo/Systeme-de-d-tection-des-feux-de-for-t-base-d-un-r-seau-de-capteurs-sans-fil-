/* 
  Basic test program, send date at the BAND you seted.
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn
  
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/
#include "heltec.h"
#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6
#include "DHTesp.h"   // library DHT
//#include "BluetoothSerial.h"   // library bluethooth
#include "CO2Sensor.h"   //library detecteur co2
#include "heltec.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#endif
#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6
byte command[] = {0xFF, 0x04, 0x03, 0x01, 0x00, 0x25, 0x13, 0x88, 0x25};//upload mode
byte QnA[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};//To read the concentration value
byte readBuffer[9] = {};
int counter = 0;
int co;

CO2Sensor co2Sensor(34, 0.99, 100);
unsigned char FucCheckSum(unsigned char *i, unsigned char ln)//CheckSum
{
  unsigned char j, tempq = 0;
  i += 1;
  for (j = 0; j < (ln - 2); j++)
  {
    tempq += *i;
    i++;
  }
  tempq = (~tempq) + 1;
  return (tempq);
}
DHTesp dht;
void setup() {
    co2Sensor.calibrate();
      Serial2.begin(9600, SERIAL_8N1, 13, 15);//Start Serial2, mode, RX, TX
      Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
    dht.setup(12, DHTesp::DHT22);


  //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  
}

void loop() {
 // Serial.print("Sending packet: ");
  //Serial.println(counter);
  // send packet
  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  int co2 = co2Sensor.read();
  int temperature = dht.getTemperature();
  int humidity = dht.getHumidity();
  Serial2.readBytes(readBuffer, sizeof(readBuffer));//Q&A read
     byte Check = FucCheckSum(readBuffer, sizeof(readBuffer));//Checksum
     if (Check == readBuffer[8]) {//Checksum compare
        co = readBuffer[2] * 256 + readBuffer[3]*0.1;//gas concentration
      }
/*
* LoRa.setTxPower(txPower,RFOUT_pin);
* txPower -- 0 ~ 20
* RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
*   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
*   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
*/
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
char buffer_txt[10];
  sprintf(buffer_txt, "%d:%d:%d:%d",co2,temperature,humidity,co);
    Serial.println(buffer_txt);

 /* sprintf(buffer_txt, "co2:%d",co2);
  Serial.println(buffer_txt);
  LoRa.print(buffer_txt);
  sprintf(buffer_txt, "Temp:%d",temperature);
  Serial.println(buffer_txt);
  LoRa.print(buffer_txt);
  sprintf(buffer_txt, "Hum:%d",humidity);
  Serial.println(buffer_txt);*/
  LoRa.print(buffer_txt);
  LoRa.endPacket();
  
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
