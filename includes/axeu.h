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

#include <asio.hpp>

namespace axeu {
  using tcp = asio::ip::tcp;
  using error_code = asio::error_code;

  class app {
  private:
    asio::io_context m_io;
    unsigned short m_port;
    tcp::acceptor m_acceptor;

  public:
    app() : m_port(9877), m_acceptor(m_io) {};

    app &port(unsigned short port) {
      m_port = port;
      return *this;
    }
  };
}; // namespace axeu
