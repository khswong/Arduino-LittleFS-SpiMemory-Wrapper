#ifndef LFS_WRAPPER_H
#define LFS_WRAPPER_H

#include <lfs.h>
#include <lfs_util.h>

//namespace LFS {

class LfsInstance;
class LfsFile;
class LfsDir;

class LfsInstance {
public:
  LfsInstance() {};
  LfsInstance(struct lfs_config _cfg);

  // General operations
  void configure(lfs_config _cfg);
  void mount();
  void unmount();
  void format();
  void remove(const char *path);
  // void remove_recursive(const char *path);
  void rename(const char *path, const char *newpath);
  LfsDir mkdir(const char *path);
  lfs_ssize_t size() { return lfs_fs_size(&lfs); };
  // return the error state of the last operation
  int errorState() { return err; }

private:
  void stat(lfs_info* info, const char *path);
  lfs_t lfs;
  lfs_config cfg;
  int err;
  friend class LfsFile;
  friend class LfsDir;
};

class LfsFile {
public:
  LfsFile(LfsInstance *fs);
  LfsFile(LfsInstance *fs, const char *path,
          int flags,
          const struct lfs_file_config *config = nullptr);
  // File operations
  void open(const char *path, int flags);
  void open(const char *path, int flags, const struct lfs_file_config *config);
  void close();
  void sync();
  lfs_ssize_t read(void *buffer, lfs_size_t size);
  lfs_ssize_t write(const void *buffer, lfs_size_t size);
  lfs_soff_t seek(lfs_soff_t off, int whence);
  void truncate(lfs_off_t size);
  lfs_soff_t tell();
  void rewind();
  lfs_soff_t size();

private:
  LfsInstance *lfsInst;
  lfs_file_t file;
  lfs_info info;
};

class LfsDir {
public:
  LfsDir(LfsInstance *fs = nullptr, const char *path = nullptr);
  // Directory operations
  void open(const char *path);
  void close();
  void read();
  void seek(lfs_off_t off);
  lfs_soff_t tell();

private:
  LfsInstance *lfsInst;
  lfs_dir_t dir;
  lfs_info info;
};

//} // namespace LFS
#endif
