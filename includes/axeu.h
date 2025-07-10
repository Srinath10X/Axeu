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
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace axeu {
  using tcp = asio::ip::tcp;

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
    std::vector<std::any> params;
    std::string method, path, body;
    std::unordered_map<std::string, std::string> headers;

    template <typename T> T get_param(size_t idx) {
      return std::any_cast<T>(params[idx]);
    }
  };

  class response {
    unsigned short status_code;

    std::string body;
    std::unordered_map<std::string, std::string> headers;

    std::string generate_response() const {
      std::ostringstream response;

      response << "HTTP/1.1 " << status_code << " "
               << axeu::detail::get_status_phrase(status_code) << "\r\n";
      for (const auto &[name, value] : headers)
        response << name << ": " << value << "\r\n";

      if (headers.find("Content-Length") == headers.end())
        response << "Content-Length: " << body.size() << "\r\n";
      response << "\r\n" << body;
      return response.str();
    }
  };

  class app {
  private:
    unsigned short m_port {9877};

    asio::io_context m_io;
    tcp::acceptor m_acceptor {m_io};

    struct Route {
      std::string path;
      std::regex path_regex;
      std::vector<std::string> param_types;
      std::function<axeu::request(const axeu::response &)> handler;
    };

    std::vector<Route> routes;

    std::pair<std::string, std::vector<std::string>>
    compile_path(const std::string &path) {
      if (path == "/") return {R"(^/$)", {}};

      std::string part;
      std::istringstream ss(path);
      std::vector<std::string> types;
      std::string regex_string = "^";

      while (std::getline(ss, part, '/')) {
        if (part.empty()) continue;
        if (part.front() == '<' && part.back() == '>') {
          auto type = part.substr(1, part.size() - 2);
          regex_string += type == "int" ? "/(\\d+)" : "/([^/]+)";
          types.push_back(type);
        } else {
          regex_string += "/" + part;
        }
      }

      return {regex_string + "$", types};
    }

  public:
    app &port(unsigned short port) {
      m_port = port;
      return *this;
    }

    void run() {
      asio::error_code ec;
      tcp::endpoint endpoint(tcp::v4(), m_port);
      ec = m_acceptor.open(endpoint.protocol(), ec);
      ec = m_acceptor.bind(endpoint, ec);
      ec = m_acceptor.listen(asio::socket_base::max_listen_connections, ec);

      ///  TODO: implement accepting logic and handling request and response
      m_io.run();
    }
  };
}; // namespace axeu
