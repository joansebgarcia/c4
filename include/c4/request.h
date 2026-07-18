#ifndef C4_INCLUDE_C4_REQUEST_H_
#define C4_INCLUDE_C4_REQUEST_H_

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace c4 {

class Request {
 public:
  Request(std::string method, std::string path, std::string body,
          std::vector<std::pair<std::string, std::string>> headers);

  std::string_view Method() const;
  std::string_view Path() const;
  std::string_view Body() const;
  std::optional<std::string_view> Header(std::string_view name) const;
  std::optional<std::string_view> Param(std::string_view name) const;

 private:
  std::string method_;
  std::string path_;
  std::string body_;
  std::vector<std::pair<std::string, std::string>> headers_;
  std::vector<std::pair<std::string, std::string>> params_;
};

}  // namespace c4

#endif  // C4_INCLUDE_C4_REQUEST_H_
