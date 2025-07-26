#pragma once
#include "reverb.hpp"
#include "sample.hpp"
#include "send.hpp"
#include "speaker.hpp"
#include "synth.hpp"
#include <shared/note.hpp>

template <typename _Tp>
struct remove_reference
{
  typedef _Tp type;
};

template <typename _Tp>
struct remove_reference<_Tp &>
{
  typedef _Tp type;
};
template <typename _Tp>
struct remove_reference<_Tp &&>
{
  typedef _Tp type;
};

template <typename _Tp>
using remove_reference_t = typename remove_reference<_Tp>::type;

template <class T>
constexpr remove_reference_t<T> &&move(T &&arg) noexcept
{
  return static_cast<remove_reference_t<T> &&>(arg);
}

class BaseFunc
{
public:
  virtual auto invoke() -> void = 0;
};

template <typename Functor>
class Func final : public BaseFunc
{
public:
  Func(Functor aFunc) : func(move(aFunc)) {}
  auto invoke() -> void final { func(); }

private:
  Functor func;
};

namespace internal
{
  auto thread(BaseFunc &) -> void;
}

auto thread(auto func)
{
  auto f = new Func<decltype(func)>(move(func));
  internal::thread(*f);
}

auto createBass(Sink &, double gain = 0, double pan = 0) -> Synth;
auto createPad(Sink &, double gain = 0, double pan = 0) -> Synth;
auto createPluck(Sink &, double gain = 0, double pan = 0) -> Synth;
auto createSynth(Sink &, double gain = 0, double pan = 0) -> Synth;
auto delay(double) -> void;
auto delay(const dur &) -> void;
auto newThread() -> void;
auto now() -> int;
auto rnd() -> int;
auto runForever() -> void;
auto setBpm(double) -> void;
