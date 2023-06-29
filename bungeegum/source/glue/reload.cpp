
#include <hscpp/Hotswapper.h>
#include <hscpp/mem/MemoryManager.h>

#include <bungeegum/core/widget.hpp>
#include <bungeegum/glue/archive.fwd>
#include <bungeegum/glue/reload.fwd>

namespace bungeegum {
namespace detail {

    // reload_manager

    reload_manager::reload_manager()
    {
        _swapper = std::make_shared<hscpp::Hotswapper>();
        _manager = std::make_shared<hscpp::mem::UniqueRef<hscpp::mem::MemoryManager>>(hscpp::mem::MemoryManager::Create());
        _swapper->SetAllocator((_manager.get()->operator->()));
        // _swapper->EnableFeature(hscpp::Feature::ManualCompilationOnly);
        // _swapper->SetGlobalUserData(&(widgets_context));
    }

    uint1 reload_manager::allocated_blocks_count()
    {
        return static_cast<uint1>(_manager.get()->operator->()->GetNumBlocks());
    }

    void reload_manager::add_include_directory(const std::filesystem::path& directory)
    {
        _swapper->AddIncludeDirectory(directory);
    }

    void reload_manager::add_library(const std::filesystem::path& file)
    {
        _swapper->AddLibrary(file);
    }

    void reload_manager::add_source_directory(const std::filesystem::path& directory)
    {
        _swapper->AddSourceDirectory(directory);
    }

    void reload_manager::add_force_compiled_source_file(const std::filesystem::path& directory)
    {
        _swapper->AddForceCompiledSourceFile(directory);
    }

    void reload_manager::update()
    {
        _swapper->Update();
    }

    void reload_manager::force_update()
    {
        _swapper->TriggerManualBuild();
    }
}
}