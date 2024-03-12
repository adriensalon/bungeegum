#include <bungeegum/glue/console.hpp>
#include <bungeegum/glue/hotswap.hpp>

#if BUNGEEGUM_USE_HOTSWAP

#include <hscpp/Hotswapper.h>
#include <hscpp/mem/MemoryManager.h>

// #include <Objbase.h>

// Add the following line
// #pragma comment(lib, "Ole32.lib")

// #include <Windows.h>

// Add the following line
// #pragma comment(lib, "Advapi32.lib")

namespace bungeegum {
namespace detail {

    std::size_t reloader::allocated_blocks_count()
    {
        return _manager.get()->operator->()->GetNumBlocks();
    }

    std::vector<std::string>& reloader::defines()
    {
        return _defines.second;
    }

    std::vector<std::filesystem::path>& reloader::include_directories()
    {
        return _include_directories.second;
    }

    std::vector<std::filesystem::path>& reloader::source_directories()
    {
        return _source_directories.second;
    }

    std::vector<std::filesystem::path>& reloader::force_compiled_source_files()
    {
        return _force_compiled_source_files.second;
    }

    std::vector<std::filesystem::path>& reloader::libraries()
    {
        return _libraries.second;
    }

    void reloader::set_global_data(void* data_ptr)
    {
        _swapper->SetGlobalUserData(data_ptr);
    }

    reloaded_loader::reloaded_loader(const std::filesystem::path& archive_path)
        : _fstream(archive_path)
        , _archive(_fstream.value())
    {
    }

    reloaded_loader::reloaded_loader(std::stringstream& archive_stream)
        : _sstream(archive_stream)
        , _archive(_sstream.value().get())
    {
    }

    reloaded_saver::reloaded_saver(const std::filesystem::path& archive_path)
        : _fstream(archive_path)
        , _archive(_fstream.value())
    {
    }

    reloaded_saver::reloaded_saver(std::stringstream& archive_stream)
        : _sstream(archive_stream)
        , _archive(_sstream.value().get())
    {
    }
}
}

#endif