#ifndef CURL_FUNCTIONS_H
#define CURL_FUNCTIONS_H

#include <curl/curl.h>
#include <stdint.h>
#include <string>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp);
std::string simple_curl(const char *target);

#endif
