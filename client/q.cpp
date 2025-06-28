#include "q.hpp"
#include <cassert>
#include <optional>

Q::Q(uv::Uv &uv, std::function<void(std::string)> cb)
  : async(uv.createAsync([this, cb = std::move(cb)]() {
      for (;;)
      {
        auto payload = [this]() -> std::optional<std::string> {
          auto lock = std::lock_guard(reqQMutex);
          if (reqQ.empty())
            return std::nullopt;
          auto r = std::move(reqQ.back());
          reqQ.pop_back();
          return r;
        }();
        if (!payload)
          break;
        cb(std::move(*payload));
      }
    }))
{
}
auto Q::pushToReqQ(std::string v) -> void
{
  auto lock = std::lock_guard(reqQMutex);
  reqQ.push_front(std::move(v));
  async.send();
}

auto Q::waitForRspId(int id) -> std::string
{
  auto lock = std::unique_lock<std::mutex>(rspQMutex);
  rspQCv.wait(lock, [id, this]() { return rspQ.find(id) != std::end(rspQ); });
  auto it = rspQ.find(id);
  assert(it != std::end(rspQ));
  auto r = std::move(it->second);
  rspQ.erase(id);
  return r;
}

auto Q::pushToRspQ(int id, std::string v) -> void
{
  std::unique_lock<std::mutex> lock(rspQMutex);
  rspQ.emplace(id, std::move(v));
  rspQCv.notify_all();
}
