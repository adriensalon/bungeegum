#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace bungeegum {
namespace detail {

    void checkout(const std::filesystem::path install_directory, const std::string& branch);

    void configure(const std::string& generator, const std::vector<std::string>& definitions);

    void build();
}
}