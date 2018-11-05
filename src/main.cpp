#include <Arduino.h>
#include "ExtDueFlashStorage.h"

#define SerialMonitor Serial
#define __ADDRESS_WRITE_FLASH__ 0
#define __MAX_LENGTH_DATA__ 15
#define __MAX_FLASH_STORAGE__ 20

ExtDueFlashStorage extDueFlashStorage(__ADDRESS_WRITE_FLASH__, __MAX_LENGTH_DATA__, __MAX_FLASH_STORAGE__);

void setup() {
  SerialMonitor.begin(115200);
  extDueFlashStorage.init();

  for (unsigned int i = 0; i < __MAX_FLASH_STORAGE__ + 20; i++) {
    extDueFlashStorage.writeString("Write Data " + String(i));
  }

  String data[__MAX_FLASH_STORAGE__];
  extDueFlashStorage.readString(data);

  unsigned char count = extDueFlashStorage.getCountWriteData();
  SerialMonitor.println("Count = " + String(count));
  for (unsigned char i = 0; i < count; i++) {
    SerialMonitor.println(data[i]);
  }

  SerialMonitor.println("Finish!");

  extDueFlashStorage.reset();
}

void loop() {
  // put your main code here, to run repeatedly:
}
