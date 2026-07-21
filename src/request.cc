#include "c4/request.h"

namespace c4 {
Request::Request(std::string method, std::string path, std::string body,
                 std::vector<std::pair<std::string, std::string>> headers)
    : method_(std::move(method)),
      path_(std::move(path)),
      body_(std::move(body)),
      headers_(std::move(headers)) {}

std::string_view Request::Method() const {
  return method_;
}

std::string_view Request::Path() const {
  return path_;
}

std::string_view Request::Body() const {
  return body_;
}

std::optional<std::string_view> Request::Header(std::string_view name) const {
  for (const auto& [key, value] : headers_) {
    if (key == name) {
      return value;
    }
  }

  return std::nullopt;
}

std::optional<std::string_view> Request::Param(std::string_view name) const {
  for (const auto& [key, value] : params_) {
    if (key == name) {
      return value;
    }
  }

  return std::nullopt;
}
}  // namespace c4
