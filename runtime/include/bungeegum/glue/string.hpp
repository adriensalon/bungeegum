#pragma once

#include <regex>
#include <string>

namespace bungeegum {
namespace detail {
    
    /// @brief
    /// @param wstr
    [[nodiscard]] std::wstring widen(const std::string& str);

    /// @brief
    /// @param wstr
    [[nodiscard]] std::string narrow(const std::wstring& wstr);

    /// @brief
    /// @param str
    /// @param regex_str
    [[nodiscard]] bool regex_search(const std::string& str, const std::string& regex_str);

    /// @brief
    /// @param str
    /// @param regex
    [[nodiscard]] bool regex_search(const std::string& str, const std::regex& regex);

}
}