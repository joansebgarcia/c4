#ifndef H3_INCLUDE_H3_RESPONSE_H_
#define H3_INCLUDE_H3_RESPONSE_H_

#include <string>
#include <string_view>

namespace h3 {

class Response {
 public:
  Response();

  Response& Status(int code);
  void Send(std::string_view body);
  void Text(std::string_view body);
  void Json(std::string_view body);

  // Internal: used by App to serialize the finished response.
  std::string Build() const;

 private:
  int status_code_ = 200;
  std::string content_type_;
  std::string body_;
};

}  // namespace h3

#endif  // H3_INCLUDE_H3_RESPONSE_H_
