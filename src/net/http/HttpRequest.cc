/**
 * lhqvq
 *
 *
 */
#include "src/net/http/HttpRequest.h"

using lhqvq::HttpRequest;

const char* HttpRequest::MethodNames[] = {
  "INVALID",
  "GET",
  "POST",
  "HEAD",
  "PUT",
  "DELETE"
};

const char* HttpRequest::VersionNames[] = {
  "UNKNOWN",
  "HTTP/1.0",
  "HTTP/1.1"
};