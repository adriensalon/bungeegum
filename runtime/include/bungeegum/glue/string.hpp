#pragma once

#include <regex>
#include <string>
#include <vector>

namespace bungeegum {
namespace detail {

    /// @brief
    /// @param str
    /// @param regex_str
    [[nodiscard]] bool contains(const std::string& str, const std::string& regex_str);

    /// @brief
    /// @param str
    /// @param regex
    [[nodiscard]] bool contains(const std::string& str, const std::regex& regex);

    /// @brief
    /// @param wstr
    [[nodiscard]] std::string narrow(const std::wstring& wstr);

    /// @brief 
    /// @param str 
    /// @param to_replace 
    /// @param replace_with 
    [[nodiscard]] std::string replace(const std::string& str, const std::string& to_replace, const std::string& replace_with);

    /// @brief
    /// @param str
    /// @param sep
    [[nodiscard]] std::vector<std::string> split(const std::string& str, const char sep = ',');
    
    /// @brief
    /// @param str
    [[nodiscard]] std::wstring widen(const std::string& str);

}
}