/**
 *  Axeu: A minimalist framework for building REST APIs
 *
 *  Copyright 2025 Srinath10X
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#pragma once

#include <any>
#include <asio.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace axeu {
  using tcp = asio::ip::tcp;
  using error_code = asio::error_code;

  namespace detail {
    const std::unordered_map<unsigned short, std::string> status_phrases = {
        {200, "OK"},           {201, "Created"},
        {204, "No Content"},   {301, "Moved Permanently"},
        {302, "Found"},        {400, "Bad Request"},
        {401, "Unauthorized"}, {403, "Forbidden"},
        {404, "Not Found"},    {500, "Internal Server Error"},
        {502, "Bad Gateway"},  {503, "Service Unavailable"},
    };

    inline const std::string get_status_phrase(unsigned short status_code) {
      auto it = status_phrases.find(status_code);
      return it != status_phrases.end() ? it->second : "Unknown";
    }
  } // namespace detail

  class request {
    std::string path;
    std::string body;
    std::string method;
    std::vector<std::any> params;
    std::unordered_map<std::string, std::string> headers;

    template <typename T> T get_param(size_t idx) {
      if (idx >= params.size()) throw std::out_of_range("Parameter out of range");
      return std::any_cast<T>(params[idx]);
    }
  };

  class response {
    unsigned short status_code;

    std::string body;
    std::unordered_map<std::string, std::string> headers;

    std::string generate_response() const {
      std::ostringstream response;
      response << "HTTP/1.1 " << status_code << " " << axeu::detail::get_status_phrase(status_code) << "\r\n";

      for (const auto &[name, value] : headers) {
        response << name << ": " << value << "\r\n";
      }

      if (headers.find("Content-Length") == headers.end()) response << "Content-Length: " << body.size() << "\r\n";
      response << "\r\n" << body;
      return response.str();
    }
  };
} // namespace axeu

namespace axeu {

  class App {
  private:
    asio::io_context m_io;
    tcp::acceptor m_acceptor;
    unsigned short m_port {9877};

  public:
    App() : m_acceptor(m_io) {};

    App &port(unsigned short port) {
      m_port = port;
      return *this;
    }
  };
}; // namespace axeu
