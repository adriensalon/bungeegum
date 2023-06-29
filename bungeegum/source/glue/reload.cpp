
#include <hscpp/Hotswapper.h>
#include <hscpp/mem/MemoryManager.h>

#include <bungeegum/glue/archive.fwd>
#include <bungeegum/glue/reload.fwd>

namespace bungeegum {
namespace detail {

    // reload_manager

    reload_manager::reload_manager()
    {
        _swapper.SetAllocator(&(_manager.get()));
    }

    uint1 reload_manager::allocated_blocks_count()
    {
        return static_cast<uint1>(_manager.get().GetNumBlocks());
    }

    void reload_manager::add_include_directory(const std::filesystem::path& directory)
    {
        _swapper.AddIncludeDirectory(directory);
    }

    void reload_manager::add_source_directory(const std::filesystem::path& directory)
    {
        _swapper.AddSourceDirectory(directory);
    }

    void reload_manager::update()
    {
        _swapper.Update();
    }
}
}