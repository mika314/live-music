#pragma once

#include <ser/macro.hpp>
#include <ser/proto.hpp>

namespace msg
{
  struct Log
  {
    std::string msg;
    SER_PROPS(msg);
  };

  struct NowReq
  {
    int32_t id;
    SER_PROPS(id);
  };

  struct NowRsp
  {
    int32_t samples; // 44100 Hz
    SER_PROPS(samples);
  };

  struct Speaker_CtorReq
  {
    int32_t id;
    SER_PROPS(id);
  };

  struct CtorRsp
  {
    int32_t id;
    SER_PROPS(id);
  };

  struct Synth_CtorReq
  {
    int32_t id;
    int32_t sinkId;
    SER_PROPS(id, sinkId);
  };
} // namespace msg

using ProtoC2S = Proto<msg::Log, msg::NowReq, msg::Speaker_CtorReq, msg::Synth_CtorReq>;
