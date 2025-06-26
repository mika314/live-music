#include <iostream>
#include <shared/uv.hpp>

#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128

int main()
{
  auto loop = uv::Uv{};
  auto server = uv::Tcp{loop.loop()};
  server.bind("0.0.0.0", DEFAULT_PORT, 0);
  std::vector<std::unique_ptr<uv::Tcp>> connections;
  server.listen(DEFAULT_BACKLOG, [&connections](int status, uv::Tcp connection) {
    if (status < 0)
    {
      fprintf(stderr, "New connection error: %s\n", uv_strerror(status));
      return;
    }

    connections.emplace_back(std::make_unique<uv::Tcp>(std::move(connection)));
    connections.back()->readStart(
      [tcp = connections.back().get(), &connections](int status, std::string buf) {
        if (status < 0)
        {
          std::cout << status << std::endl;
          if (auto it = std::find_if(std::begin(connections),
                                     std::end(connections),
                                     [tcp](const auto &x) { return tcp == x.get(); });
              it != std::end(connections))
            connections.erase(it);
          return;
        }

        std::cout << status << ": " << buf << std::endl;
        tcp->write(std::move(buf), [](int) {});
      });
  });

  printf("Listening on port %d...\n", DEFAULT_PORT);
  for (;;)
    loop.tick();
}
