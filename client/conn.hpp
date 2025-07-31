#pragma once
#include "q.hpp"
#include <msgpack/msgpack-ser.hpp>
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
    const auto id = getNextId();
    {
      auto st = std::ostringstream{};
      v.id = id;
      msgpackSer(st, ProtoC2S{std::move(v)});
      req(st.str());
    }
    {
      auto st = std::istringstream{waitForRspId(id)};
      Rsp r;
      msgpackDeser(st, r);
      return r;
    }
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
  auto msg = ProtoC2S{.v = std::move(v)};
  auto st = std::ostringstream{};
  msgpackSer(st, msg);
  Conn::getInst().req(st.str());
}
