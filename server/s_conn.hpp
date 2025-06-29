#pragma once
#include "s_entity.hpp"
#include <functional>
#include <shared/consts.hpp>
#include <shared/proto.hpp>
#include <shared/uv.hpp>

class Conn
{
public:
  Conn(uv::Tcp &&, std::function<auto(Conn *)->void> destroy);
  auto operator()(msg::Log) -> void;
  auto operator()(msg::NowReq) -> void;
  auto operator()(msg::SetBpm) -> void;
  auto operator()(msg::Speaker_CtorReq) -> void;
  auto operator()(msg::Synth_CtorReq) -> void;
  auto operator()(msg::Synth_Note) -> void;

private:
  uv::Tcp tcp;
  std::string buf;
  int32_t sz = -1;
  auto processPack(std::string_view) -> void;
  std::unordered_map<int32_t, std::unique_ptr<Entity>> entities;
  float bpm = DefaultBpm;

  auto send(auto id, auto rsp);
  template <typename Entity, typename... Args>
  auto ctor(int32_t rspId, Args &&...);
};
