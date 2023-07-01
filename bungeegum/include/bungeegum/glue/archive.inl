#pragma once

namespace bungeegum {
namespace detail {

    inline input_archiver::input_archiver(const std::filesystem::path& archive_path)
        : _fstream(archive_path)
        , _archive(_fstream)
    {
    }

    template <typename value_t>
    void input_archiver::load(value_t& value)
    {
        // value._bungeegum_load(_archive);
        _archive(value);
    }

    inline output_archiver::output_archiver(const std::filesystem::path& archive_path)
        : _fstream(archive_path)
        , _archive(_fstream)
    {
    }

    template <typename value_t>
    void output_archiver::save(value_t& value)
    {
        // _archive(value);
        value.get()._bungeegum_save(_archive);
    }
}
}