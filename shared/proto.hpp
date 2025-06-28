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
} // namespace msg

using ProtoC2S = Proto<msg::Log, msg::NowReq>;
