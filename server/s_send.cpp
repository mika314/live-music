#include "s_send.hpp"
#include <cassert>
#include <cmath>
#include <shared/consts.hpp>

Send::Send(class Sink &sink, Source &aOwner, double aGain, double aPan) : Source(sink), owner(aOwner)
{
  gain = aGain;
  pan = aPan;
}

auto Send::internalPull(int samples) -> std::vector<float>
{
  if (q.empty())
    owner.get().pull(samples);
  auto r = std::move(q.back());
  q.pop_back();
  assert(r.empty() || static_cast<int>(r.size()) == ChN * samples);
  const auto a = powf(10.f, (owner.get().gain + gain) / 20.f);
  for (auto i = 0U; i < r.size(); ++i)
    r[i] *= a * ((i % 2 == 1) ? ((pan <= 0) ? 1.f : 1.f + static_cast<float>(pan))
                              : ((static_cast<float>(pan) <= 0) ? 1.f - static_cast<float>(pan) : 1.f));

  return r;
}

auto Send::isBusy() const -> bool
{
  return owner.get().isBusy();
}
