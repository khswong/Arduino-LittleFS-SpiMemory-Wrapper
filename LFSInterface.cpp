#include <SPI.h>
#include <SPIMemory.h>
#include <lfs.h>

#include "LFSWrapper.h"

#include "LFSInterface.h"

extern SPIFlash flash;
/*
enum lfs_error {
    LFS_ERR_OK          = 0,    // No error
    LFS_ERR_IO          = -5,   // Error during device operation
    LFS_ERR_CORRUPT     = -84,  // Corrupted
    LFS_ERR_NOENT       = -2,   // No directory entry
    LFS_ERR_EXIST       = -17,  // Entry already exists
    LFS_ERR_NOTDIR      = -20,  // Entry is not a dir
    LFS_ERR_ISDIR       = -21,  // Entry is a dir
    LFS_ERR_NOTEMPTY    = -39,  // Dir is not empty
    LFS_ERR_BADF        = -9,   // Bad file number
    LFS_ERR_FBIG        = -27,  // File too large
    LFS_ERR_INVAL       = -22,  // Invalid parameter
    LFS_ERR_NOSPC       = -28,  // No space left on device
    LFS_ERR_NOMEM       = -12,  // No more memory available
    LFS_ERR_NOATTR      = -61,  // No data/attr available
    LFS_ERR_NAMETOOLONG = -36,  // File name too long
};
 */

enum SpiMemoryError {
  SM_ERR_SUCCESS = 0x00,
  SM_ERR_CALLBEGIN = 0x01,
  SM_ERR_UNKNOWNCHIP = 0x02,
  SM_ERR_UNKNOWNCAP = 0x03,
  SM_ERR_CHIPBUSY = 0x04,
  SM_ERR_OUTOFBOUNDS = 0x05,
  SM_ERR_CANTENWRITE = 0x06,
  SM_ERR_PREVWRITTEN = 0x07,
  SM_ERR_LOWRAM = 0x08,
  SM_ERR_SYSSUSPEND = 0x09,
  SM_ERR_ERRORCHKFAIL = 0x0A,
  SM_ERR_NORESPONSE = 0x0B,
  SM_ERR_UNSUPPORTEDFUNC = 0x0C,
  SM_ERR_UNABLETO4BYTE = 0x0D,
  SM_ERR_UNABLETO3BYTE = 0x0E,
  SM_ERR_CHIPISPOWEREDDOWN = 0x0F,
  SM_ERR_NOSFDP = 0x10,
  SM_ERR_NOSFDPERASEPARAM = 0x11,
  SM_ERR_NOSFDPERASETIME = 0x12,
  SM_ERR_NOSFDPPROGRAMTIMEPARAM = 0x13,
  SM_ERR_NOCHIPSELECTDECLARED = 0x14,
  SM_ERR_UNKNOWNERROR = 0xFE,
};

int mapSpiMemorytoLFSError(int SP_Error) {
  if (SP_Error) 
  {
    Serial.print("spi mem error: ");
    Serial.println(SP_Error, HEX);
  }
  switch (SP_Error) {
  case SM_ERR_SUCCESS:
    return LFS_ERR_OK;
    break;
  case SM_ERR_ERRORCHKFAIL:
    return LFS_ERR_CORRUPT;
    break;
  default:
    return LFS_ERR_IO;
    break;
  }
}

int SpiFlashRead(const struct lfs_config *c, lfs_block_t block,
                          lfs_off_t off, void *buffer, lfs_size_t size) {
  // Not great, we should be passing in SpiFlashLfsCfg as its .context pointer
  // But we'll live
  uint32_t address = (block * SpiFlashLfsCfg.block_size) + off;
  flash.readByteArray(address, (byte *)buffer, size);
  return mapSpiMemorytoLFSError(flash.error());
}

int SpiFlashProg(const struct lfs_config *c, lfs_block_t block,
                          lfs_off_t off, const void *buffer, lfs_size_t size) {
  uint32_t address = (block * SpiFlashLfsCfg.block_size) + off;
  flash.writeByteArray(address, (byte *)buffer, size);
  return mapSpiMemorytoLFSError(flash.error());
}

int SpiFlashErase(const struct lfs_config *c, lfs_block_t block) {
  uint32_t address = (block * SpiFlashLfsCfg.block_size);
  flash.eraseSection(address, SpiFlashLfsCfg.block_size);
  return mapSpiMemorytoLFSError(flash.error());
}

int SpiFlashSync(const struct lfs_config *c) {
  // nop nop nop
  return 0;
}