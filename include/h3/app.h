#ifndef H3_INCLUDE_H3_APP_H_
#define H3_INCLUDE_H3_APP_H_

#include <cstdint>
#include <functional>
#include <memory>
#include <string_view>

#include "h3/request.h"
#include "h3/response.h"

namespace h3 {

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

}  // namespace h3

#endif  // H3_INCLUDE_H3_APP_H_
