#ifndef _RI_2015_1_UTIL_STATUS_H_
#define _RI_2015_1_UTIL_STATUS_H_

#include <cstdlib>
#include <iostream>
#include <string>

namespace util {

class Status {
 public:
  enum class StatusCode {
    OK,           // if the status is good to go, return this value.
    UNAVAILABLE,  // The requested operation could not be completed due to a
                  // service denial.
    INVALID_ARGUMENTS,  // The supplied parameters are not valid for the given
                        // method.
    NOT_FOUND           // THe requested object could not be found / retrieved.
  };

  Status(StatusCode status_code, const std::string& error)
      : status_code_(status_code), error_(error){};

  Status(StatusCode status_code) : Status(status_code, ""){};

  bool ok() const {
    return (this->status_code_ == StatusCode::OK);
  }

  const std::string& error() const {
    return this->error_;
  }

  static const Status& OK;

 private:
  StatusCode status_code_;

  std::string error_;
};

const Status& Status::OK = Status(Status::StatusCode::OK);
}  // namespace util

#endif
