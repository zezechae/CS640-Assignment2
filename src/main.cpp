#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>

// Heltec V3 전용 SX1262 핀 매핑 (매우 중요)
#define LORA_CS    8
#define LORA_RST   12
#define LORA_DIO1  14
#define LORA_BUSY  13
#define LORA_SCK   9
#define LORA_MISO  11
#define LORA_MOSI  10

// SX1262 모듈 생성
SX1262 radio = new Module(LORA_CS, LORA_DIO1, LORA_RST, LORA_BUSY);

int counter = 0;

void setup() {
  Serial.begin(115200);
  delay(2000); // 부팅 후 시리얼 모니터 안정화 대기
  Serial.println("\n--- Heltec V3 LoRa TX Setup ---");

  // 1. 내부 SPI 핀을 명시적으로 연결 (이 줄이 없으면 칩을 찾지 못함)
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);

  // 2. SX1262 초기화 (915 MHz 대역)
  Serial.print("[SX1262] Initializing ... ");
  int state = radio.begin(915.0);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("Success!");
  } else {
    Serial.print("Failed, code ");
    Serial.println(state);
    while (true); // 에러 나면 여기서 정지
  }

  // 3. LoRa 물리 계층 파라미터 설정
  radio.setSpreadingFactor(7);
  radio.setBandwidth(125.0);
  radio.setOutputPower(14);
}

void loop() {
  String msg = "Hello CS640 - Packet #" + String(counter);
  Serial.print("Sending: ");
  Serial.print(msg);

  // 무선으로 문자열 전송
  int state = radio.transmit(msg);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("  --> TX Complete!");
  } else {
    Serial.print("  --> TX Failed, code ");
    Serial.println(state);
  }

  counter++;
  delay(5000); // 5초 대기
}

//Rx
/*
#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>

// Heltec V3 전용 SX1262 핀 매핑
#define LORA_CS    8
#define LORA_RST   12
#define LORA_DIO1  14
#define LORA_BUSY  13
#define LORA_SCK   9
#define LORA_MISO  11
#define LORA_MOSI  10

// SX1262 모듈 생성
SX1262 radio = new Module(LORA_CS, LORA_DIO1, LORA_RST, LORA_BUSY);

void setup() {
  Serial.begin(115200);
  delay(2000); // 부팅 후 시리얼 모니터 안정화 대기
  Serial.println("\n--- Heltec V3 LoRa RX Setup ---");

  // 1. 내부 SPI 핀 연결 (TX 코드와 동일)
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);

  // 2. SX1262 초기화 (915 MHz 대역)
  Serial.print("[SX1262] Initializing ... ");
  int state = radio.begin(915.0);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("Success!");
  } else {
    Serial.print("Failed, code ");
    Serial.println(state);
    while (true); 
  }

  // 3. 물리 계층 설정 (TX와 완벽히 똑같아야 수신 가능)
  radio.setSpreadingFactor(7);
  radio.setBandwidth(125.0);
  
  Serial.println("Waiting for incoming packets...");
}

void loop() {
  String rxData;
  
  // 패킷이 수신될 때까지 대기했다가 읽어오는 블로킹 함수
  int state = radio.receive(rxData);

  if (state == RADIOLIB_ERR_NONE) {
    // 수신 성공 시 데이터와 신호 강도 출력
    Serial.print("Received: '");
    Serial.print(rxData);
    Serial.print("' | RSSI: ");
    Serial.print(radio.getRSSI());
    Serial.print(" dBm | SNR: ");
    Serial.print(radio.getSNR());
    Serial.println(" dB");
    
  } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    // 수신 대기 타임아웃 (정상적인 현상, 루프 재시작)
    
  } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
    // 전파 간섭으로 패킷이 공중에서 깨진 경우
    Serial.println("Error: Packet corrupted (CRC Mismatch)");
    
  } else {
    // 기타 에러
    Serial.print("Receive failed, code ");
    Serial.println(state);
  }
}
*/