#pragma once
#include "q.hpp"
#include <string>

class ConnectionThread
{
public:
  static auto getInst() -> ConnectionThread &;
  static auto init() -> void;

  ~ConnectionThread();
  auto req(std::string) -> void;
  auto waitForRspId(int id) -> std::string;
  auto getNextId() -> int;

private:
  ConnectionThread();
  std::thread thread;
  uv::Tcp connection;
  std::unique_ptr<Q> q;
  std::atomic_bool done = false;
  int nextId = 0;
  bool isReady = false;
  std::condition_variable startupCv;
  std::mutex startupMutex;
  std::string buf;
  int32_t sz = -1;

  auto onRead(int status, std::string) -> void;
};
