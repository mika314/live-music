#include "master_speaker.hpp"
#include "s_conn.hpp"
#include <iostream>
#include <log/log.hpp>
#include <sdlpp/sdlpp.hpp>
#include <shared/uv.hpp>
#include <thread>

#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128

int main()
{
  auto init = sdl::Init{SDL_INIT_AUDIO};
  auto loop = uv::Uv{};
  auto server = uv::Tcp{loop.loop()};
  server.bind("0.0.0.0", DEFAULT_PORT, 0);
  std::vector<std::unique_ptr<Conn>> conns;
  MasterSpeaker masterSpeaker;
  server.listen(DEFAULT_BACKLOG, [&conns, &masterSpeaker](int status, uv::Tcp conn) {
    if (status < 0)
    {
      LOG("New connection error:", uv_strerror(status));
      return;
    }

    conns.emplace_back(std::make_unique<Conn>(
      std::move(conn),
      [&conns, &masterSpeaker](Conn *self) {
        masterSpeaker.lock();
        if (auto it = std::find_if(
              std::begin(conns), std::end(conns), [self](const auto &x) { return self == x.get(); });
            it != std::end(conns))
          conns.erase(it);
        masterSpeaker.unlock();
      },
      masterSpeaker));
  });

  auto t = std::thread([]() { system("cd ../client; ./client.sh"); });

  LOG("Server is running");
  for (;;)
    loop.tick();
}
