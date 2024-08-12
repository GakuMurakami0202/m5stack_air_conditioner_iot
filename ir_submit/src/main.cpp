#include <M5Core2.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

// 赤外線送信ピンを26に設定
const uint16_t kIrLedPin = 26; 
const uint16_t kFrequency = 38000; // 38kHzの周波数

IRsend irsend(kIrLedPin); // 赤外線送信機の初期化

// 関数プロトタイプの宣言
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
    for (int i = 0; i < 5; i++) { // 信号を5回送信
      sendCoolingOn();
      delay(500); // 信号の間に遅延を入れる
    }
  }

  // ボタンBを押すとエアコンをオフにする
  if (M5.BtnB.wasPressed()) {
    M5.Lcd.println("AC Off");
    for (int i = 0; i < 5; i++) { // 信号を5回送信
      sendAcOff();
      delay(500); // 信号の間に遅延を入れる
    }
  }

  delay(100);
}

// 冷房をオンにする信号を送信する関数
void sendCoolingOn() {
  uint16_t coolingOnSignal[] = {0xCC00, 0xFFBF, 0x4000, 0x1001, 0x0110, 0x0040, 0xBFFF, 0x00CC, 0x3392, 0x6D13, 0xEC74, 0x8B00, 0xFF00, 0xFF00, 0xFF00, 0xFF00, 0xFF00, 0x25DA, 0xD12E, 0x00FF, 0x00FF};
  irsend.sendRaw(coolingOnSignal, sizeof(coolingOnSignal) / sizeof(coolingOnSignal[0]), kFrequency);
}

// エアコンをオフにする信号を送信する関数
void sendAcOff() {
  uint16_t acOffSignal[] = {0xCC00, 0xFFBF, 0x4000, 0x1001, 0x0110, 0x0040, 0xBFFF, 0x00CC, 0x3392, 0x6D13, 0xEC74, 0x8B00, 0xFF00, 0xFF00, 0xFF00, 0xFF00, 0xFF00, 0x25DA, 0xC13E, 0x00FF, 0x00FF};
  irsend.sendRaw(acOffSignal, sizeof(acOffSignal) / sizeof(acOffSignal[0]), kFrequency);
}