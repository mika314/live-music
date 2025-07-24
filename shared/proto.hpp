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
    double gain;
    double pan;
    SER_PROPS(id, gain, pan);
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
    double gain;
    double pan;
    OscType oscType;
    Envelope envelope;
    SER_PROPS(id, sinkId, gain, pan, oscType, envelope);
  };

  struct SetBpm
  {
    double v;
    SER_PROPS(v);
  };

  struct Synth_Note
  {
    int32_t id;
    Note v;
    SER_PROPS(id, v);
  };

  struct Synth_Envelope
  {
    int32_t id;
    Envelope envelope;
    SER_PROPS(id, envelope);
  };

  struct Sample_CtorReq
  {
    int32_t id;
    int32_t sinkId;
    std::string path;
    double gain;
    double pan;
    Note note;
    SER_PROPS(id, sinkId, path, gain, pan, note);
  };

  struct Sample_Note
  {
    int32_t id;
    Note note;
    SER_PROPS(id, note);
  };

  struct Sample_Envelope
  {
    int32_t id;
    Envelope envelope;
    SER_PROPS(id, envelope);
  };

  struct Source_SetGain
  {
    int32_t id;
    double v;
    SER_PROPS(id, v);
  };

  struct Source_SetPan
  {
    int32_t id;
    double v;
    SER_PROPS(id, v);
  };

  struct Reverb_CtorReq
  {
    int32_t id;
    int32_t sinkId;
    double gain;
    double pan;
    SER_PROPS(id, sinkId, gain, pan);
  };

  struct Reverb_SetWet
  {
    int32_t id;
    double v;
    SER_PROPS(id, v);
  };

  struct Source_SendReq
  {
    int32_t id;
    int32_t sourceId;
    int32_t sinkId;
    double gain;
    double pan;
    SER_PROPS(id, sourceId, sinkId, gain, pan);
  };

  struct Source_SendRsp
  {
    int32_t id;
    SER_PROPS(id);
  };
} // namespace msg

using ProtoC2S = Proto<msg::Log,
                       msg::NowReq,
                       msg::SetBpm,
                       msg::Speaker_CtorReq,
                       msg::Synth_CtorReq,
                       msg::Synth_Note,
                       msg::Synth_Envelope,
                       msg::Sample_CtorReq,
                       msg::Sample_Note,
                       msg::Sample_Envelope,
                       msg::Source_SetGain,
                       msg::Source_SetPan,
                       msg::Reverb_CtorReq,
                       msg::Reverb_SetWet,
                       msg::Source_SendReq>;
