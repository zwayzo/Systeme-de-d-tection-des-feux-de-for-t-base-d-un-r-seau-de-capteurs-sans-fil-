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
//BluetoothSerial SerialBT;
DHTesp dht;
CO2Sensor co2Sensor(34, 0.99, 100);
int co;
void setup() {
    Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
   Serial.begin(115200);
   Serial.println("=== Initialized ===");
//WIFI Kit series V1 not support Vext control
  dht.setup(12, DHTesp::DHT22);
  co2Sensor.calibrate();
  Serial2.begin(9600, SERIAL_8N1, 13, 15);//Start Serial2, mode, RX, TX
 }
void loop() {
  Serial.print("Sending packet: ");
  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  //digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);                       // wait for a second
  //digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  //delay(1000);                       // wait for a second
  int co2 = co2Sensor.read();
  int temperature = dht.getTemperature();
  int humidity = dht.getHumidity();
  Serial2.readBytes(readBuffer, sizeof(readBuffer));//Q&A read
      byte Check = FucCheckSum(readBuffer, sizeof(readBuffer));//Checksum
     if (Check == readBuffer[8]) {//Checksum compare
        co = readBuffer[2] * 256 + readBuffer[3]*0.1;//gas concentration
      }
//      incoming += (char)LoRa.read();
 char buffer_txt[40];
  sprintf(buffer_txt, "co2:%d",co2);
  Serial.println(buffer_txt);
  LoRa.print(buffer_txt);
  sprintf(buffer_txt, "Temp:%d",temperature);
  Serial.println(buffer_txt);
  LoRa.print(buffer_txt);
  sprintf(buffer_txt, "Hum:%d",humidity);
  Serial.println(buffer_txt);
  LoRa.print(buffer_txt);
  sprintf(buffer_txt, "co:%d",co);
  Serial.println(buffer_txt);
  LoRa.print(buffer_txt);
  LoRa.endPacket();
       delay(4000);
}
