![AxeuBanner](https://u.cubeupload.com/Srinath10X/Screenshot20250606at.png)

**Axeu** is a lightweight, header-only C++ framework for building fast REST APIs with minimal overhead and modern C++ syntax.

## ✨ Features

- ⚙️ **Modern C++** (C++17+) API design
- ✅ **Header-only**: Just include and go
- 📦 **No external dependencies** beyond Asio
- 🚀 **Blazing-fast routing** with zero runtime overhead


## 🚀 Quickstart

### 🔗 Define a REST API

```cpp
#include "axeu.hpp"

int main() {
  axeu::app app;

  app.get("/", [](const axeu::request &) {
    return axeu::response(200, "Welcome to Axeu!");
  });

  app.get("/hello/<str>", [](const axeu::request &req) {
    auto name = req.get_param<std::string>(0);

    return axeu::response(200, "Hello, " + name + "!");
  });

  app.get("/add/<int>/<int>", [](const axeu::request &req) {
    int a = req.get_param<int>(0);
    int b = req.get_param<int>(1);

    return axeu::response::json(200, {{"sum", std::to_string(a + b)}});
  });

  app.port(8000).run();
}
```

## 📦 Building

You need a C++17+ compiler and Asio (standalone version or via Boost).

```sh
g++ main.cpp -std=c++17 -pthread -o server
./server
```

To use with standalone Asio:
- Install standalone Asio (`apt install libasio-dev` or from source).
- Ensure it’s in your include path.
- Or define `ASIO_STANDALONE` before including.


## 📚 API

### `App` methods
```cpp
app.get(path, handler);
app.post(path, handler);
app.put(path, handler);
app.del(path, handler);
app.port(port_number).run();
```

### Path parameters

Use `<int>` or `<str>` in route paths:

- `/user/<int>` → casts to `int`
- `/hello/<str>` → casts to `std::string`

## 📜 License

Axeu is licensed under the **Apache License 2.0**.  
Feel free to use it in personal, academic, or commercial projects.

## 🙏 Acknowledgements

Inspired by:
- [Express.js](https://expressjs.com/)
- [Crow](https://github.com/CrowCpp/Crow)

Happy hacking! 🧠⚙️
