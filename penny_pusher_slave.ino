#include <U8g2lib.h>

/* Code for slave devices - they are waiting for getting a signal and showing
   grafix / updated per coin lane.
   Attached is a 128x64 lcd (monochrome)
   Code runs on an Arduino Nano (ATMega328P)

   Initial version 0.1
   - show default message
   - wait for an interrupt and display for 2 seconds a different message

   Version 0.2
   - added counter
   - "improved" UI

   Version 0.3
   - added blinking text to insert more coins
   - improved display for up to 9999 coins

*/


// Initialisiere das Display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Variable zur Verfolgung der Zeit für den Interrupt
unsigned long lastInterruptTime = 0;
unsigned long blinkInterval = 1000;
unsigned long blinkPreviousMillis = 0;
bool blinkState = true;
unsigned int anzahlMuenzen = 0;

void setup() {
  // Initialisiere das Display
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr); // Wähle eine Schriftart


  // Konfiguriere den Interrupt-Pin
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), handleInterrupt, FALLING);

  Serial.begin(9600); // Starte die serielle Kommunikation

}

void loop() {


  // Warte 2 Sekunden, wenn der Interrupt ausgelöst wurde
  if (millis() - lastInterruptTime < 2000) {
    // Zeige den Text "juhu" auf dem Display an
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.setCursor(0, 10);
      u8g2.print("juhu");
    } while (u8g2.nextPage());
  }
  else
  {
    // Zeige den Wartezustand an
    u8g2.firstPage();
    do {
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      drawInsertCoins();
      drawCoinText();
      drawCoinCounter();
      drawFrameWaitScreen();
    } while (u8g2.nextPage());
  }

}

// Interrupt-Handler
void handleInterrupt() {
  lastInterruptTime = millis();
  anzahlMuenzen++;
}


// Funtions to handle the default Screen
void drawFrameWaitScreen() {
  u8g2.drawFrame(2, 2, 124, 60);
  u8g2.drawHLine(2, 45, 124);
}

void drawCoinText() {
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(42, 42);
  u8g2.print("Muenzen");
}

void drawCoinCounter() {
  u8g2.setFont(u8g2_font_ncenB24_tr);
  if (anzahlMuenzen < 10) {
    u8g2.setCursor(60, 32);
    u8g2.print(anzahlMuenzen);
  }
  else if (anzahlMuenzen < 100) {
    u8g2.setCursor(50, 32);
    u8g2.print(anzahlMuenzen);
  }
  else if (anzahlMuenzen < 1000) {
    u8g2.setCursor(40, 32);
    u8g2.print(anzahlMuenzen);
  }
  else
  {
    u8g2.setCursor(30, 32);
    u8g2.print(anzahlMuenzen);
  }
}

void drawInsertCoins() {
  unsigned long blinkCurrentMillis = millis();
  if (blinkCurrentMillis - blinkPreviousMillis >= blinkInterval) {

    blinkPreviousMillis = blinkCurrentMillis;
    blinkState = !blinkState;
  }

  if (blinkState == true) {     //schreibe den Text nur, falls blinkState erfüllt ist
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setCursor(8, 57);
    u8g2.print("Wirf Muenzen ein!");
  }
}
