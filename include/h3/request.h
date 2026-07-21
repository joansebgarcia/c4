#ifndef H3_INCLUDE_H3_REQUEST_H_
#define H3_INCLUDE_H3_REQUEST_H_

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace h3 {

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

}  // namespace h3

#endif  // H3_INCLUDE_H3_REQUEST_H_
