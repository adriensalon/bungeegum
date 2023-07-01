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
        wrapper<value_t> _wrapper(value);
        _archive(_wrapper);
    }

    inline output_archiver::output_archiver(const std::filesystem::path& archive_path)
        : _fstream(archive_path)
        , _archive(_fstream)
    {
    }

    template <typename value_t>
    struct wrapper {
        wrapper(value_t& value)
            : _ref(value)
        {
        }

        template <typename archive_t>
        void load(archive_t& archive)
        {
            _ref.get()._bungeegum_load(archive);
        }

        template <typename archive_t>
        void save(archive_t& archive) const
        {
            _ref.get()._bungeegum_save(archive);
        }

        value_t& _ref;
    };

    template <typename value_t>
    void output_archiver::save(value_t& value)
    {
        wrapper<value_t> _wrapper(value);
        _archive(_wrapper);
    }
}
}