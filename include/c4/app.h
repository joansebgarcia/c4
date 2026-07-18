#ifndef C4_INCLUDE_C4_APP_H_
#define C4_INCLUDE_C4_APP_H_

#include <cstdint>
#include <functional>
#include <memory>
#include <string_view>

#include "c4/request.h"
#include "c4/response.h"

namespace c4 {

using Handler = std::function<void(const Request&, Response&)>;

class App {
 public:
  App();
  ~App();

  void Get(std::string_view path, Handler handler);
  void Post(std::string_view path, Handler handler);

  void Listen(uint16_t port, std::function<void()> on_start = nullptr);

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace c4

#endif  // C4_INCLUDE_C4_APP_H_
