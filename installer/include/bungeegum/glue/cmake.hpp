#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace bungeegum {
namespace detail {

    std::optional<std::string> cmake_version();

    std::vector<std::string> cmake_generators();

    void cmake_configure(
        const std::filesystem::path& source_directory,
        const std::filesystem::path& build_directory,
        const std::optional<std::filesystem::path> install_directory,
        const std::optional<std::string>& generator,
        const std::vector<std::pair<std::string, bool>>& definitions,
        const bool debug = false);

    void cmake_build(const std::filesystem::path& build_directory, const std::optional<std::string>& target = std::nullopt);

    void cmake_install(const std::filesystem::path& build_directory, const std::filesystem::path& prefix, const std::optional<std::string>& target = std::nullopt);
}
}