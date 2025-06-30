#pragma once
#include "s_entity.hpp"
#include "s_speaker.hpp"
#include <functional>
#include <shared/consts.hpp>
#include <shared/proto.hpp>
#include <shared/uv.hpp>

class Conn
{
public:
  Conn(uv::Tcp &&, std::function<auto(Conn *)->void> destroy, MasterSpeaker &);
  auto operator()(msg::Log) -> void;
  auto operator()(msg::NowReq) -> void;
  auto operator()(msg::SetBpm) -> void;
  auto operator()(msg::Speaker_CtorReq) -> void;
  auto operator()(msg::Synth_CtorReq) -> void;
  auto operator()(msg::Synth_Envelope) -> void;
  auto operator()(msg::Synth_Note) -> void;

private:
  uv::Tcp tcp;
  std::reference_wrapper<MasterSpeaker> masterSpeaker;
  std::string buf;
  int32_t sz = -1;
  auto processPack(std::string_view) -> void;
  std::unordered_map<int32_t, std::unique_ptr<Entity>> entities;
  double bpm = DefaultBpm;

  auto send(auto id, auto rsp);
  template <typename Entity, typename... Args>
  auto ctor(int32_t rspId, Args &&...);
};
