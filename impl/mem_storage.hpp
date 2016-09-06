#pragma once
#include <raft.hpp>
#include <map>
#include <list>
#include <iostream>

#ifdef _WIN32 
#include <Windows.h>
#define JOURNAL_FD HANDLE
#define JOURNAL_OPEN(file) CreateFile(filename, GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL)
#define JOURNAL_WRITE(file, buffer, size) WriteFile(file, buffer, size, NULL, NULL)
#define JOURNAL_CLOSE(file) CloseHandle(file)
#define JOURNAL_BAD_WRITE 0
#else
#define JOURNAL_FD int
#define JOURNAL_OPEN(filename) open(filename,O_WRONLY | O_CREAT | O_DIRECT | O_DSYNC, S_IWUSR | S_IRUSR)
#define JOURNAL_WRITE(file, buffer, size) write(file, buffer, size)
#define JOURNAL_CLOSE(file) close(file)
#define JOURNAL_BAD_WRITE -1
#endif

class JournalStream {
public:
  JournalStream(const char* filename) {
    file = JOURNAL_OPEN(filename);
  }

  ~JournalStream() {
    JOURNAL_CLOSE(file);
  }
  bool write(const char* data, size_t length) {
    if ( JOURNAL_WRITE(file, data, length) == JOURNAL_BAD_WRITE ) {
        std::cout << "can't write " <<  GetLastError();
        return false;
    }
    return true;
  }

private:
  JOURNAL_FD file;
};

namespace avery {
class MemStorage : public raft::Storage {
 public:
  MemStorage(const char * filename);
  uint64_t append(const std::vector<raft::Entry> &entries);
  void voted_for(std::string id);
  void current_term(uint64_t current_term);
  uint64_t commit_until(uint64_t commit_index);
  std::string voted_for();
  uint64_t current_term();
  raft::EntryInfo last_commit();
  std::vector<raft::Entry> entries_since(uint64_t index);
  raft::EntryInfo get_last_entry_info();
  raft::EntryInfo get_entry_info(uint64_t index);

 private:
  typedef std::vector<raft::Entry> entries_t;
  entries_t entries_;
  JournalStream log_file_;
  std::string voted_for_;
  raft::EntryInfo last_entry_info_;
  raft::EntryInfo commit_info_;
  uint64_t current_term_;
};
}