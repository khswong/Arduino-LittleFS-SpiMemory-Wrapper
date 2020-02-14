#ifndef LFS_INTERFACE_H
#define LFS_INTERFACE_H

// Read a region in a block. Negative error codes are propogated
// to the user.
int SpiFlashRead(const struct lfs_config *c, lfs_block_t block,
                          lfs_off_t off, void *buffer, lfs_size_t size);

// Program a region in a block. The block must have previously
// been erased. Negative error codes are propogated to the user.
// May return LFS_ERR_CORRUPT if the block should be considered bad.
int SpiFlashProg(const struct lfs_config *c, lfs_block_t block,
                          lfs_off_t off, const void *buffer, lfs_size_t size);

// Erase a block. A block must be erased before being programmed.
// The state of an erased block is undefined. Negative error codes
// are propogated to the user.
// May return LFS_ERR_CORRUPT if the block should be considered bad.
int SpiFlashErase(const struct lfs_config *c, lfs_block_t block);

// Sync the state of the underlying block device. Negative error codes
// are propogated to the user.
int SpiFlashSync(const struct lfs_config *c);

// Configure these buffers for the cache
static byte SpiFlashReadBuffer[256];
static byte SpiFlashProgBuffer[256];
static byte SpiFlashLookaheadBuffer[256];
const struct lfs_config SpiFlashLfsCfg = {
    // Opaque user provided context that can be used to pass
    // information to the block device operations
    .context = nullptr,
    .read = &SpiFlashRead,
    .prog = &SpiFlashProg,
    .erase = &SpiFlashErase,
    .sync = &SpiFlashSync,
    .read_size = 8,
    .prog_size = 8,
    .block_size = 256,
    .block_count = 1000,
    .block_cycles = 1000,
    .cache_size = 256,
    .lookahead_size = 64,
    .read_buffer = SpiFlashReadBuffer,
    .prog_buffer = SpiFlashProgBuffer,
    .lookahead_buffer = SpiFlashLookaheadBuffer,
    .name_max = 0,
    .file_max = 0,
    .attr_max = 0,
};


#endif