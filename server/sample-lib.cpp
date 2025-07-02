#include "sample-lib.hpp"
#include "load-audio.hpp"
#include <cassert>

auto SampleLib::operator()(std::filesystem::path v) -> std::reference_wrapper<std::vector<float>>
{
  auto it = lib.find(v);
  if (it == std::end(lib))
  {
    auto tmp = lib.emplace(v, loadAudio(v));
    assert(tmp.second);
    it = tmp.first;
  }
  return it->second;
}

auto SampleLib::getInst() -> SampleLib &
{
  static SampleLib inst;
  return inst;
}
