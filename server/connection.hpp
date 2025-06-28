#pragma once
#include <functional>
#include <shared/proto.hpp>
#include <shared/uv.hpp>

class Connection
{
public:
  Connection(uv::Tcp &&, std::function<auto(Connection *)->void> destroy);
  auto operator()(msg::Log) -> void;
  auto operator()(msg::NowReq) -> void;

private:
  uv::Tcp tcp;
  std::string buf;
  int32_t sz = -1;
  auto processPack(std::string_view) -> void;
};
