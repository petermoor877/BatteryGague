#include "MAX17043.h"
#include "Wire.h"
#include "SSD1306Wire.h"

SSD1306Wire  display(0x3c,4,5);
MAX17043 batteryMonitor;
char tmp[8];

void setup() {
  
 
Serial.begin(115200);
Serial.println();
Serial.println();

display.init();
display.flipScreenVertically();
display.setFont(ArialMT_Plain_16);
display.setTextAlignment(TEXT_ALIGN_LEFT);

Wire.begin();
batteryMonitor.reset();
batteryMonitor.quickStart();
delay(1000);

}

void loop() {
  
  display.clear();
  
  float cellVoltage = batteryMonitor.getVCell();
  float stateOfCharge = batteryMonitor.getSoC();
  display.drawString(0, 0, "DSTIKE ");
  display.drawString(0, 16, "LiPo: ");
  display.drawString(40, 16, dtostrf(stateOfCharge,4,1,tmp));
  display.drawString(85, 16, "%");

  

  display.drawString(0, 32, dtostrf(cellVoltage,5,2,tmp));
  display.drawString(40, 32, "V");
  display.drawString(0, 48, "dstike.com");

  
  display.display();
  delay(500);

  
}
