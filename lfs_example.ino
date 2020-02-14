#include <SPI.h>
#include <SPIMemory.h>       //https://github.com/Marzogh/SPIMemory

#include "LFSWrapper.h"
#include "LFSInterface.h"

#define EXT_FLASH_SS      A5

SPIFlash flash(EXT_FLASH_SS, &SPI);
LfsInstance extFlashFs;

LfsFile radioConfig(&extFlashFs);
LfsFile MlxCalConfig(&extFlashFs);

void setup() {
  Serial.begin(115200);
  while(!Serial.available());
  pinMode(EXT_FLASH_SS, OUTPUT);
  digitalWrite(EXT_FLASH_SS, HIGH);

  flash.begin();
  Serial.println("Init Flash");
  extFlashFs.configure(SpiFlashLfsCfg);
  extFlashFs.mount();
  if (extFlashFs.errorState()) {
    Serial.print("error state:"); 
    Serial.print(extFlashFs.errorState(), HEX);
    Serial.println("; formatting drive...");
    extFlashFs.format();
    extFlashFs.mount();
  }
  // Running total of how many resets:
  LfsFile boot_count(&extFlashFs, "boot_count", LFS_O_RDWR | LFS_O_CREAT); 
  byte boot_count_num;
  boot_count.read(&boot_count_num, 1);         
  Serial.print("boot count num: ");                                                               
  Serial.println(boot_count_num);
  boot_count_num += 1;
  boot_count.rewind();
  boot_count.write(&boot_count_num, 1);
  // Your writes are not saved until you close the file!
  boot_count.close();
}

void loop(void) {
  // Gotta do some stuff here
}