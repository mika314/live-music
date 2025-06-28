#include "connection.hpp"
#include <iostream>
#include <shared/uv.hpp>

#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128

int main()
{
  auto loop = uv::Uv{};
  auto server = uv::Tcp{loop.loop()};
  server.bind("0.0.0.0", DEFAULT_PORT, 0);
  std::vector<std::unique_ptr<Connection>> connections;
  server.listen(DEFAULT_BACKLOG, [&connections](int status, uv::Tcp connection) {
    if (status < 0)
    {
      fprintf(stderr, "New connection error: %s\n", uv_strerror(status));
      return;
    }

    connections.emplace_back(std::make_unique<Connection>(std::move(connection), [&](Connection *self) {
      if (auto it = std::find_if(std::begin(connections),
                                 std::end(connections),
                                 [self](const auto &x) { return self == x.get(); });
          it != std::end(connections))
        connections.erase(it);
    }));
  });

  printf("Listening on port %d...\n", DEFAULT_PORT);
  for (;;)
    loop.tick();
}
