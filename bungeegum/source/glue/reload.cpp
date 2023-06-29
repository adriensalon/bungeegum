
#include <hscpp/Hotswapper.h>
#include <hscpp/mem/MemoryManager.h>

#include <bungeegum/core/widget.hpp>
#include <bungeegum/glue/archive.fwd>
#include <bungeegum/glue/reload.fwd>

namespace bungeegum {
namespace detail {

    // reload_manager

    reloader::reloader()
    {
        _swapper = std::make_shared<hscpp::Hotswapper>();
        _manager = std::make_shared<hscpp::mem::UniqueRef<hscpp::mem::MemoryManager>>(hscpp::mem::MemoryManager::Create());
        _swapper->SetAllocator((_manager.get()->operator->()));
        // _swapper->EnableFeature(hscpp::Feature::ManualCompilationOnly);
        // _swapper->SetGlobalUserData(&(widgets_context));
    }

    uint1 reloader::allocated_blocks_count()
    {
        return static_cast<uint1>(_manager.get()->operator->()->GetNumBlocks());
    }

    void reloader::add_include_directory(const std::filesystem::path& directory)
    {
        _swapper->AddIncludeDirectory(directory);
    }

    void reloader::add_library(const std::filesystem::path& file)
    {
        _swapper->AddLibrary(file);
    }

    void reloader::add_source_directory(const std::filesystem::path& directory)
    {
        _swapper->AddSourceDirectory(directory);
    }

    void reloader::add_force_compiled_source_file(const std::filesystem::path& directory)
    {
        _swapper->AddForceCompiledSourceFile(directory);
    }

    void reloader::clear()
    {
        _swapper->ClearCompileOptions();
        _swapper->ClearForceCompiledSourceFiles();
        _swapper->ClearIncludeDirectories();
        _swapper->ClearLibraries();
        _swapper->ClearLibraryDirectories();
        // etc
        _swapper->ClearSourceDirectories();
    }

    void reloader::update()
    {
        _swapper->Update();
    }

    void reloader::force_update()
    {
        _swapper->TriggerManualBuild();
    }
}
}