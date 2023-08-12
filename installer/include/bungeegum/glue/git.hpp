#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace bungeegum {
namespace detail {

    std::optional<std::string> git_version();

    void git_clone(const std::string& url, const std::filesystem::path& install_directory, const std::string& branch);

    bool is_tag_newer(const std::string& found_tag, const std::string& new_tag);
}
}