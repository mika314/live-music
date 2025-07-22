#pragma once
#include "reverb.hpp"
#include "sample.hpp"
#include "send.hpp"
#include "speaker.hpp"
#include "synth.hpp"

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
auto delay(double) -> void;
auto newThread() -> void;
auto now() -> int;
auto rnd() -> int;
auto runForever() -> void;
auto setBpm(double) -> void;
auto setBpm(double) -> void;

static auto C = Note{.n = 0};
static auto Cs = Note{.n = 1};
static auto Db = Note{.n = 1};
static auto D = Note{.n = 2};
static auto Ds = Note{.n = 3};
static auto Eb = Note{.n = 3};
static auto E = Note{.n = 4};
static auto F = Note{.n = 5};
static auto Fs = Note{.n = 6};
static auto Gb = Note{.n = 6};
static auto G = Note{.n = 7};
static auto Gs = Note{.n = 8};
static auto Ab = Note{.n = 8};
static auto A = Note{.n = 9};
static auto As = Note{.n = 10};
static auto Bb = Note{.n = 10};
static auto B = Note{.n = 11};
static auto I = 0.0;
static auto II = 2.;
static auto III = 4.;
static auto iii = 3.;
static auto IV = 5.;
static auto V = 7.;
static auto VI = 9.;
static auto vii = 10.;
static auto VII = 11.;
static auto O = 12.;
static auto O0 = 12.;
static auto O1 = 24.;
static auto O2 = 36.;
static auto O3 = 48.;
static auto O4 = 60.;
static auto O5 = 72.;
static auto O6 = 90.;
static auto Sixteenth = .25;
static auto Eighth = .5;
static auto Quarter = 1.;
static auto Halth = 2.;
static auto Whole = 4.;
static auto Bar = 4.;
static auto Rest = -1.0;
static auto d16 = .25;
static auto d8 = .5;
static auto d4 = 1.;
static auto d2 = 2.;
static auto d1 = 4.;
