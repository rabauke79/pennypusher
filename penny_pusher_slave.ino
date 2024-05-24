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
*/



// Initialisiere das Display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Variable zur Verfolgung der Zeit für den Interrupt
unsigned long lastInterruptTime = 0;
unsigned int anzahlMuenzen = 0;

void setup() {
  // Initialisiere das Display
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr); // Wähle eine Schriftart

  // Konfiguriere den Interrupt-Pin
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), handleInterrupt, FALLING);
}

void loop() {


  // Warte 2 Sekunden, wenn der Interrupt ausgelöst wurde
  if (millis() - lastInterruptTime < 2000) {
    // Zeige den Text "juhu" auf dem Display an
    u8g2.firstPage();
    do {
      u8g2.setCursor(0, 10);
      u8g2.print("juhu");
    } while (u8g2.nextPage());
  }
  else
  {
    // Zeige den Text "Warte" auf dem Display an
    u8g2.firstPage();
    do {
      u8g2.setDrawColor(1);
      u8g2.clearBuffer();
      drawCoinCounter();
      drawFrameWaitScreen();
      drawInsertCoins();
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

void drawCoinCounter() {
  u8g2.setFont(u8g2_font_ncenB24_tr);
  u8g2.setCursor(60,32);
  u8g2.print(anzahlMuenzen);
}

void drawInsertCoins() {
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(6, 57);
  u8g2.print("Wirf eine Münze ein!");
}
