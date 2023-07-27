#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace bungeegum {
namespace detail {

    std::optional<std::string> git_version();

    std::string git_local_branch(const std::filesystem::path& dot_git_directory);

    void git_clone(const std::string& url, const std::string& branch);
}
}