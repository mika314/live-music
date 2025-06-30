#pragma once

#include "envelope.hpp"
#include "note.hpp"
#include "osc_type.hpp"
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
    int32_t samples;
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
    OscType oscType;
    Envelope envelope;
    SER_PROPS(id, sinkId, oscType, envelope);
  };

  struct SetBpm
  {
    float v;
    SER_PROPS(v);
  };

  struct Synth_Note
  {
    int32_t id;
    Note note;
    SER_PROPS(id, note);
  };

  struct Synth_Envelope
  {
    int32_t id;
    Envelope envelope;
    SER_PROPS(id, envelope);
  };
} // namespace msg

using ProtoC2S = Proto<msg::Log,
                       msg::NowReq,
                       msg::SetBpm,
                       msg::Speaker_CtorReq,
                       msg::Synth_CtorReq,
                       msg::Synth_Note,
                       msg::Synth_Envelope>;
