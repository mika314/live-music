#pragma once
#include <condition_variable>
#include <deque>
#include <functional>
#include <shared/uv.hpp>
#include <string>
#include <unordered_map>

class Q
{
public:
  Q(uv::Uv &, std::function<void(std::string)>);
  auto pushToReqQ(std::string) -> void;
  auto waitForRspId(int id) -> std::string;
  auto pushToRspQ(int id, std::string) -> void;

private:
  uv::Async async;
  std::mutex reqQMutex;
  std::deque<std::string> reqQ;
  std::mutex rspQMutex;
  std::condition_variable rspQCv;
  std::unordered_map<int, std::string> rspQ;
};
