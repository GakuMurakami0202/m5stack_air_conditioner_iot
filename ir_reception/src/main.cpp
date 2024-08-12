#include <M5Core2.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

const uint16_t kRecvPin = 36; // 赤外線受信ピン

IRrecv irrecv(kRecvPin, 1024, 15, true); // バッファサイズを大きくし、長い信号に対応
decode_results results;

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
    // 長い信号を検出した場合、分割して解析
    if (results.overflow) {
      Serial.println("Warning: IR signal too long, possible overflow detected!");
      Serial.println("Attempting to analyze in chunks...");
    }

    // 信号をシリアルモニタに表示
    serialPrintUint64(results.value, HEX);
    Serial.println();

    // 信号を人間が読める形式で表示
    Serial.println(resultToHumanReadableBasic(&results));

    // 信号の長さが長すぎた場合の警告
    if (results.overflow) {
      Serial.println("Warning: Signal length exceeded buffer capacity. Signal might be incomplete.");
    }

    // 受信した信号をリセット
    irrecv.resume();
  }
  delay(100);
}