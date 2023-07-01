#pragma once

#include <fstream>

#include <cereal/access.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>

// pull std types here

namespace bungeegum {
namespace detail {

    struct input_archiver {
        input_archiver(const std::filesystem::path& archive_path);
        input_archiver(const input_archiver& other) = delete;
        input_archiver& other(const input_archiver& other) = delete;
        input_archiver(input_archiver&& other) = delete;
        input_archiver& other(input_archiver&& other) = delete;

        template <typename value_t>
        void load(value_t& value);

    private:
        std::ifstream _fstream;
        cereal::JSONInputArchive _archive;
    };

    struct output_archiver {
        output_archiver(const std::filesystem::path& archive_path);
        output_archiver(const output_archiver& other) = delete;
        output_archiver& other(const output_archiver& other) = delete;
        output_archiver(output_archiver&& other) = delete;
        output_archiver& other(output_archiver&& other) = delete;

        template <typename value_t>
        void save(value_t& value);

    private:
        std::ofstream _fstream;
        cereal::JSONOutputArchive _archive;
    };

}
}

#include <bungeegum/glue/archive.inl>