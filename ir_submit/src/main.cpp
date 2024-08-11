#include <M5Core2.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>

// 赤外線送信ピン
const uint16_t kIrLedPin = 9;  // M5Stack Core2の場合、通常は9ピンを使用

IRsend irsend(kIrLedPin);

// 関数のプロトタイプ宣言
void sendCoolingOn();
void sendAcOff();

void setup() {
  // M5Stack Core2の初期化
  M5.begin();
  M5.Lcd.setTextSize(3);
  M5.Lcd.println("AC Control Ready");

  // 赤外線送信機の初期化
  irsend.begin();
}

void loop() {
  M5.update();

  // ボタンAを押すと冷房をオンにする
  if (M5.BtnA.wasPressed()) {
    M5.Lcd.println("Cooling ON");
    sendCoolingOn();
  }

  // ボタンBを押すとエアコンをオフにする
  if (M5.BtnB.wasPressed()) {
    M5.Lcd.println("AC Off");
    sendAcOff();
  }
  
  delay(100);
}

void sendCoolingOn() {
  uint64_t coolingOnSignal = 0x80080002FDFF;
  uint64_t unknownSignal1 = 0x30324473;
  uint64_t unknownSignal2 = 0xDADB4F40;

  irsend.sendPanasonic(coolingOnSignal, 48);
  delay(100);  // 遅延を少し長くしてみる
  irsend.sendNEC(unknownSignal1, 50);  
  delay(100);
  irsend.sendNEC(unknownSignal2, 50);
}

void sendAcOff() {
  uint64_t acOffSignal = 0x80080002FDFF;
  uint64_t unknownSignal1 = 0x930060D7;
  uint64_t unknownSignal2 = 0xEE0896F6;

  irsend.sendPanasonic(acOffSignal, 48);
  delay(300);  // 遅延を少し長くしてみる
  irsend.sendNEC(unknownSignal1, 50);  
  delay(300);
  irsend.sendNEC(unknownSignal2, 50);
}