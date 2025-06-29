#pragma once
#include "entity.hpp"
#include <functional>
#include <shared/proto.hpp>
#include <shared/uv.hpp>

class Connection
{
public:
  Connection(uv::Tcp &&, std::function<auto(Connection *)->void> destroy);
  auto operator()(msg::Log) -> void;
  auto operator()(msg::NowReq) -> void;
  auto operator()(msg::Speaker_CtorReq) -> void;
  auto operator()(msg::Synth_CtorReq) -> void;

private:
  uv::Tcp tcp;
  std::string buf;
  int32_t sz = -1;
  auto processPack(std::string_view) -> void;
  std::unordered_map<int32_t, std::unique_ptr<Entity>> entities;
  auto send(auto id, auto rsp);
  template <typename Entity, typename... Args>
  auto ctor(int32_t rspId, Args &&...);
};
