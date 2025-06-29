#pragma once
#include "q.hpp"
#include <shared/proto.hpp>
#include <string>

class Conn
{
public:
  static auto getInst() -> Conn &;
  static auto init() -> void;

  ~Conn();
  auto req(std::string) -> void;
  auto waitForRspId(int id) -> std::string;
  auto getNextId() -> int;

  template <typename Rsp>
  auto reqRsp(auto v) -> Rsp
  {
    std::array<char, 0x10000> buf;
    const auto id = getNextId();
    v.id = id;
    const auto sz =
      static_cast<int32_t>(ProtoC2S::ser(std::move(v), buf.data(), buf.data() + buf.size()));
    req(std::string{buf.data(), buf.data() + sz});
    auto payload = waitForRspId(id);
    auto st = IStrm{payload.data(), payload.data() + payload.size()};
    Rsp r;
    ::deser(st, r);
    return r;
  }

private:
  Conn();
  std::thread thread;
  uv::Tcp conn;
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

auto ctor(auto v)
{
  return Conn::getInst().reqRsp<msg::CtorRsp>(std::move(v)).id;
}

auto send(auto v)
{
  auto &inst = Conn::getInst();
  std::array<char, 0x10000> buf;
  const auto sz = static_cast<int32_t>(ProtoC2S::ser(std::move(v), buf.data(), buf.data() + buf.size()));
  inst.req(std::string{buf.data(), buf.data() + sz});
}
