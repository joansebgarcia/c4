#include "c4/response.h"

namespace c4 {

Response::Response() : content_type_("text/plain") {}

Response& Response::Status(int code) {
  status_code_ = code;
  return *this;
}

void Response::Send(std::string_view body) {
  Text(body);
}

void Response::Text(std::string_view body) {
  content_type_ = "text/plain";
  body_ = body;
}

void Response::Json(std::string_view body) {
  content_type_ = "application/json";
  body_ = body;
}

std::string Response::Build() const {
  std::string_view status_text = "OK";
  if (status_code_ == 404) {
    status_text = "Not Found";
  }

  return "HTTP/1.1 " + std::to_string(status_code_) + " " +
         std::string(status_text) + "\r\n" + "Content-Type: " + content_type_ +
         "\r\n" + "Content-Length: " + std::to_string(body_.size()) + "\r\n" +
         "Connection: close\r\n" + "\r\n" + body_;
}

}  // namespace c4
