#pragma once
#include <filesystem>
#include <unordered_map>
#include <vector>

class SampleLib
{
public:
  static auto getInst() -> SampleLib &;
  auto operator()(std::filesystem::path) -> std::reference_wrapper<std::vector<float>>;

private:
  SampleLib() = default;
  SampleLib(SampleLib &) = delete;
  auto operator=(SampleLib &) = delete;
  std::unordered_map<std::filesystem::path, std::vector<float>> lib;
};
