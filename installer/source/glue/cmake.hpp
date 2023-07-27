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
        const std::string& generator,
        const std::vector<std::string>& definitions);

    void cmake_build(const std::filesystem::path& build_directory);
}
}