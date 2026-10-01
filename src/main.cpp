#include <Arduino.h>

#include <SPI.h>
#include <LoRa.h>

//Tx
// Heltec V3 LoRa 핀 매핑 (보드 하드웨어 버전에 따라 다를 수 있음)
#define SS      8
#define RST     12
#define DIO0    14

int counter = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa TX Setup...");
  LoRa.setPins(SS, RST, DIO0);

  // 주파수 915 MHz 설정 (미국 대역)
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // 강의에서 다룬 트레이드오프 파라미터 적용
  LoRa.setSpreadingFactor(7);       // SF 7 (가장 빠른 속도, 짧은 거리)
  LoRa.setSignalBandwidth(125E3);   // 대역폭 125kHz
  LoRa.setTxPower(14);              // 송신 출력 설정

  Serial.println("LoRa TX Started.");
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // 문자 메시지 패킷 송신
  LoRa.beginPacket();
  LoRa.print("Hello CS640 - Packet #");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
  
  // 5초 대기 후 다음 패킷 전송
  delay(5000); 
}

//Rx
/*
#include <SPI.h>
#include <LoRa.h>

#define SS      8
#define RST     12
#define DIO0    14

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa RX Setup...");
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // TX와 동일한 물리 계층 파라미터 설정
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);

  Serial.println("LoRa RX Started. Waiting for messages...");
}

void loop() {
  // 수신된 패킷의 크기 확인
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet '");

    // 버퍼에 있는 문자열 디코딩 및 출력
    while (LoRa.available()) {
      String rxData = LoRa.readString();
      Serial.print(rxData);
    }

    // 통신 신호 강도(RSSI) 확인
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
*/