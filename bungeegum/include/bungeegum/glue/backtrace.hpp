//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <exception>
#include <filesystem>
#include <string>
#include <vector>

namespace bungeegum {

struct backtraced_source {
    std::filesystem::path file;
    std::string function;
    unsigned int line;
    unsigned int column;
};

struct backtraced_result {
    void* address;
    backtraced_source primary;
    std::vector<backtraced_source> inliners;
};

struct backtraced_exception : std::exception {
    backtraced_exception(const std::string& what, const unsigned int tracing_size);
    backtraced_exception(const std::exception& existing);
    backtraced_exception(const backtraced_exception& other) = delete;
    backtraced_exception& operator=(const backtraced_exception& other) = delete;
    backtraced_exception(backtraced_exception&& other);
    backtraced_exception& operator=(backtraced_exception&& other);

    [[nodiscard]] const char* what() const;

    std::vector<backtraced_result> tracing;

private:
    std::string _what;
};

}
