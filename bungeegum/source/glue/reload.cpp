
#include <hscpp/Hotswapper.h>
#include <hscpp/mem/MemoryManager.h>

#include <bungeegum/glue/reload.hpp>

namespace bungeegum {
namespace detail {

    // reload_manager

    reloader::reloader()
    {
        _swapper = std::make_shared<hscpp::Hotswapper>();
        _manager = std::make_shared<hscpp::mem::UniqueRef<hscpp::mem::MemoryManager>>(hscpp::mem::MemoryManager::Create());
        _swapper->SetAllocator((_manager.get()->operator->()));
        // _swapper->EnableFeature(hscpp::Feature::ManualCompilationOnly);
        // _swapper->SetGlobalUserData(&(global_widgets_manager));
    }

    std::size_t reloader::allocated_blocks_count()
    {
        return (_manager.get()->operator->()->GetNumBlocks());
    }

    void reloader::add_define(const std::string& define)
    {
        _swapper->AddPreprocessorDefinition(define);
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
        _swapper->ClearPreprocessorDefinitions();
        _swapper->ClearLibraries();
        _swapper->ClearLibraryDirectories();
        // etc
        _swapper->ClearSourceDirectories();
    }

    reload_state reloader::update()
    {
        return (static_cast<reload_state>(_swapper->Update()));
    }

    void reloader::force_update()
    {
        _swapper->TriggerManualBuild();
    }

    reloaded_loader::reloaded_loader(const std::filesystem::path& archive_path)
        : _fstream(archive_path)
        , _archive(_fstream)
    {
    }

    reloaded_saver::reloaded_saver(const std::filesystem::path& archive_path)
        : _fstream(archive_path)
        , _archive(_fstream)
    {
    }
}
}