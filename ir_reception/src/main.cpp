#include <M5Core2.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

// 赤外線受信ピン
const uint16_t kRecvPin = 36;

IRrecv irrecv(kRecvPin);
decode_results results;

unsigned long lastTime = 0; // 前回の信号を受信した時間を記録する変数
bool delayExceeded = false; // 2秒超過フラグ

void setup() {
  // M5Stack Core2の初期化
  M5.begin();
  M5.Lcd.setTextSize(3);
  M5.Lcd.println("IR Receiver Ready");

  // 赤外線受信機の初期化
  irrecv.enableIRIn();

  // シリアル通信の初期化
  Serial.begin(115200);
}

void loop() {
  if (irrecv.decode(&results)) {
    unsigned long currentTime = millis();
    unsigned long delayTime = currentTime - lastTime; // 前回の信号からの時間差を計算

    if (delayTime > 2000) { // 遅延が2秒を超えた場合
      Serial.println("----------------------------");
      delayExceeded = true;
    }

    if (!delayExceeded) {
      Serial.print("IR Signal Received after ");
      Serial.print(delayTime);
      Serial.println(" ms");
    } else {
      delayExceeded = false; // フラグをリセット
    }

    Serial.println("IR Signal Received:");
    // 赤外線信号の詳細をシリアルモニタに表示
    serialPrintUint64(results.value, HEX);
    Serial.println();
    
    // 受信した信号の復号結果を表示
    Serial.println(resultToHumanReadableBasic(&results));

    // 現在の時間をlastTimeに保存
    lastTime = currentTime;

    // 受信した信号をリセット
    irrecv.resume();
  }
  delay(100);
}