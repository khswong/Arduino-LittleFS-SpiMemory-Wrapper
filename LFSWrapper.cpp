#include <lfs.h>

#include "LFSWrapper.h"

//using namespace LFS;

/*
  LfsDir mkdir(const char *path);
  lfs_info stat(const char *path);
  lfs_ssize_t size() { return lfs_fs_size(&lfs); };
  // return the error state of the last operation
  lfs_error errorState() { return err; }

private:
  lfs_t lfs;
  lfs_config cfg;
  lfs_error err;
  friend class LfsFile;
  friend class LfsDir;
};
*/

/**
 * The default constructor won't attempt to mount
 * unless given a configuration, in which it'll try
 * to mount w/ the given config
 */

LfsInstance::LfsInstance(lfs_config _cfg) {
  cfg = _cfg;
  this->mount();
  if (err) {
    // //Do error handling
    // // we can do this if necessary:
     format();
     mount();
  }
}

void LfsInstance::configure(lfs_config _cfg) { cfg = _cfg; }

void LfsInstance::mount() { err = lfs_mount(&lfs, &cfg); }

void LfsInstance::unmount() { err = lfs_unmount(&lfs); }

void LfsInstance::format() { err = lfs_format(&lfs, &cfg); }

void LfsInstance::remove(const char *path) { err = lfs_remove(&lfs, path); }

void LfsInstance::rename(const char *path, const char *newpath) {
  err = lfs_rename(&lfs, path, newpath);
}

void LfsInstance::stat(lfs_info* info, const char *path){
  err = lfs_stat(&lfs, path, info);
}

// File operations

LfsFile::LfsFile(LfsInstance *fs) {
  if (fs) {
    lfsInst = fs;
  }
}

LfsFile::LfsFile(LfsInstance *fs, const char *path, int flags,
                 const struct lfs_file_config *config) {
  if (fs) {
    lfsInst = fs;
    if (path) {
      if (config) {
        open(path, flags, config);
      } else {
        open(path, flags);
      }
    }
  }
}


void LfsFile::open(const char *path, int flags) {
  lfsInst->err = lfs_file_open(&(lfsInst->lfs), &file, path, flags);
  if (lfsInst->err == LFS_ERR_OK) {
    lfsInst->stat(&info, path);
  }
}

void LfsFile::open(const char *path, int flags,
                   const struct lfs_file_config *config) {
  lfsInst->err = lfs_file_opencfg(&(lfsInst->lfs), &file, path, flags, config);
}

void LfsFile::close() { lfsInst->err = lfs_file_close(&(lfsInst->lfs), &file); }

void LfsFile::sync() { lfsInst->err = lfs_file_sync(&(lfsInst->lfs), &file); }

lfs_ssize_t LfsFile::read(void *buffer, lfs_size_t size) {
  lfs_ssize_t ret = lfs_file_read(&(lfsInst->lfs), &file, buffer, size);
  if (ret < 0) {
    lfsInst->err = ret;
  } // negative return value indicates error
  return ret;
}

lfs_ssize_t LfsFile::write(const void *buffer, lfs_size_t size) {
  lfs_ssize_t ret = lfs_file_write(&(lfsInst->lfs), &file, buffer, size);
  if (ret < 0) {
    lfsInst->err = ret;
  } // negative return value indicates error
  return ret;
}

lfs_soff_t LfsFile::seek(lfs_soff_t off, int whence) {
  lfs_soff_t ret = lfs_file_seek(&(lfsInst->lfs), &file, off, whence);
  if (ret < 0) {
    lfsInst->err = ret;
  } // negative return value indicates error
  return ret;
}

void LfsFile::truncate(lfs_off_t size) {
  lfsInst->err = lfs_file_truncate(&(lfsInst->lfs), &file, size);
}

lfs_soff_t LfsFile::tell() {
  lfs_soff_t ret = lfs_file_tell(&(lfsInst->lfs), &file);
  if (ret < 0) {
    lfsInst->err = ret;
  } // negative return value indicates error
  return ret;
}

void LfsFile::rewind() {
  lfsInst->err = lfs_file_rewind(&(lfsInst->lfs), &file);
}

lfs_soff_t LfsFile::size() {
  lfs_soff_t ret = lfs_file_size(&(lfsInst->lfs), &file);
  if (ret < 0) {
    lfsInst->err = ret;
  } // negative return value indicates error
  return ret;
}

/*
class LfsDir {
public:
  LfsDir(LfsInstance *fs = nullptr, const char *path = nullptr);
  // Directory operations
  lfs_dir_t open(const char *path);
  void close(lfs_dir_t *dir);
  void read(lfs_dir_t *dir, lfs_info *info);
  void seek(lfs_dir_t *dir, lfs_off_t off);
  lfs_soff_t tell(lfs_t *lfs, lfs_dir_t *dir);

private:
  LfsInstance *lfs;
  lfs_dir_t dir;
}
*/

LfsDir::LfsDir(LfsInstance *fs, const char *path) {
  if (fs) {
    lfsInst = fs;
    if (path) {
      open(path);
    }
  }
}
// Directory operations
void LfsDir::open(const char *path) {
  lfsInst->err = lfs_dir_open(&(lfsInst->lfs), &dir, path);
}

void LfsDir::close() { lfsInst->err = lfs_dir_close(&(lfsInst->lfs), &dir); }

void LfsDir::read() { lfsInst->err = lfs_dir_read(&(lfsInst->lfs), &dir, &info);}
void LfsDir::seek(lfs_off_t off) {lfsInst->err = lfs_dir_seek(&(lfsInst->lfs), &dir, off);}

lfs_soff_t LfsDir::tell() {  
  lfs_soff_t ret = lfs_dir_tell(&(lfsInst->lfs), &dir);
  if (ret < 0) {
    lfsInst->err = ret;
  } // negative return value indicates error
  return ret;
}
