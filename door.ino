#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include "HomeSpan.h"
#include "LED.h"
#include <WiFi.h>


LiquidCrystal_PCF8574 lcd(0x27);


#define SS_PIN 5
#define RST_PIN 0
 



MFRC522 mfrc522;   // 建立MFRC522實體

char *reference;

byte uid[]={0xDA, 0xB5, 0x2C, 0x47};  //這是我們指定的卡片UID，可由讀取UID的程式取得特定卡片的UID，再修改這行


void setup()
{
  
  ////////////////////////////////////////////////////////////////
  lcd.begin(16,2);
  lcd.setBacklight(255);
  Serial.begin(9600);   
  
  SPI.begin();
  
  mfrc522.PCD_Init(SS_PIN, RST_PIN); // 初始化MFRC522卡
  Serial.print(F("Reader "));
  Serial.print(F(": "));
  mfrc522.PCD_DumpVersionToSerial(); // 顯示讀卡設備的版本

}

void loop() {
  //////////////////////////////////////////////////////////////////////
  //Serial.print("reading...");
  // 檢查是不是偵測到新的卡
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      // 顯示卡片的UID
      Serial.print(F("Card UID:"));
      dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size); // 顯示卡片的UID
      Serial.println();
      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
      Serial.println(mfrc522.PICC_GetTypeName(piccType));  //顯示卡片的類型
      
      //把取得的UID，拿來比對我們指定好的UID
      bool they_match = true; // 初始值是假設為真 
      for ( int i = 0; i < 4; i++ ) { // 卡片UID為4段，分別做比對
        if ( uid[i] != mfrc522.uid.uidByte[i] ) { 
          they_match = false; // 如果任何一個比對不正確，they_match就為false，然後就結束比對
          break; 
        }
      }
      
      //在監控視窗中顯示比對的結果
      if(they_match){        
        Serial.print(F("Access Granted!"));
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Access Granted!");
        delay(2000);
        lcd.clear();
  
      }else{
        Serial.print(F("Access Denied!"));
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Access Denied!");
        delay(2000);
        lcd.clear();
      }
      mfrc522.PICC_HaltA();  // 卡片進入停止模式
    }
}

/**
 * 這個副程式把讀取到的UID，用16進位顯示出來
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}