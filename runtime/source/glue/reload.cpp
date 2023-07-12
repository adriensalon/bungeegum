#include <hscpp/Hotswapper.h>
#include <hscpp/mem/MemoryManager.h>

#include <bungeegum/glue/console.hpp>
#include <bungeegum/glue/reload.hpp>

#if BUNGEEGUM_USE_HOTSWAP

namespace bungeegum {
namespace detail {

    namespace {

        void update_compilation_defines(hscpp::Hotswapper* hotswapper, std::list<std::string>& defines)
        {
            if (!defines.empty()) {
                hotswapper->ClearPreprocessorDefinitions();
                for (const std::string& _define : defines) {
                    hotswapper->AddPreprocessorDefinition(_define);
                }
            }
        }

        void update_compilation_include_directories(hscpp::Hotswapper* hotswapper, std::list<std::filesystem::path>& include_directories)
        {
            if (!include_directories.empty()) {
                hotswapper->ClearIncludeDirectories();
                for (const std::filesystem::path& _include_directory : include_directories) {
                    hotswapper->AddIncludeDirectory(_include_directory);
                }
            }
        }

        void update_compilation_source_directories(hscpp::Hotswapper* hotswapper, std::list<std::filesystem::path>& source_directories)
        {
            if (!source_directories.empty()) {
                hotswapper->ClearSourceDirectories();
                for (const std::filesystem::path& _source_directory : source_directories) {
                    hotswapper->AddSourceDirectory(_source_directory);
                }
            }
        }

        void update_compilation_force_compiled_source_files(hscpp::Hotswapper* hotswapper, std::list<std::filesystem::path>& force_compiled_source_files)
        {
            if (!force_compiled_source_files.empty()) {
                hotswapper->ClearForceCompiledSourceFiles();
                for (const std::filesystem::path& _force_compiled_source_file : force_compiled_source_files) {
                    hotswapper->AddForceCompiledSourceFile(_force_compiled_source_file);
                }
            }
        }

        void update_compilation_libraries(hscpp::Hotswapper* hotswapper, std::list<std::filesystem::path>& libraries)
        {
            if (!libraries.empty()) {
                hotswapper->ClearLibraries();
                for (const std::filesystem::path& _force_compiled_source_file : libraries) {
                    hotswapper->AddLibrary(_force_compiled_source_file);
                }
            }
        }

    }

    reloader::reloader(std::wstreambuf* buffer)
    {
        // (void)buffer;
        console_redirect_wide _redirect(buffer);
        _swapper = std::make_shared<hscpp::Hotswapper>();
        _manager = std::make_shared<hscpp::mem::UniqueRef<hscpp::mem::MemoryManager>>(hscpp::mem::MemoryManager::Create());
        _swapper->SetAllocator((_manager.get()->operator->()));
        // _swapper->EnableFeature(hscpp::Feature::ManualCompilationOnly);
        // _swapper->SetGlobalUserData(&(global_widgets_manager));
    }

    std::size_t reloader::allocated_blocks_count()
    {
        return _manager.get()->operator->()->GetNumBlocks();
    }

    std::list<std::string>& reloader::defines()
    {
        return _defines;
    }

    std::list<std::filesystem::path>& reloader::include_directories()
    {
        return _include_directories;
    }

    std::list<std::filesystem::path>& reloader::source_directories()
    {
        return _source_directories;
    }

    std::list<std::filesystem::path>& reloader::force_compiled_source_files()
    {
        return _force_compiled_source_files;
    }

    std::list<std::filesystem::path>& reloader::libraries()
    {
        return _libraries;
    }

    reload_state reloader::update(std::wstreambuf* buffer)
    {
        update_compilation_defines(_swapper.get(), _defines);
        update_compilation_include_directories(_swapper.get(), _include_directories);
        update_compilation_source_directories(_swapper.get(), _source_directories);
        update_compilation_force_compiled_source_files(_swapper.get(), _force_compiled_source_files);
        update_compilation_libraries(_swapper.get(), _libraries);
        console_redirect_wide _redirect(buffer);
        // (void)buffer;
        return static_cast<reload_state>(_swapper->Update());
    }

    void reloader::force_update(/*take 2 callbacks for save and load + buffer to redirect*/)
    {
        update_compilation_defines(_swapper.get(), _defines);
        update_compilation_include_directories(_swapper.get(), _include_directories);
        update_compilation_source_directories(_swapper.get(), _source_directories);
        update_compilation_force_compiled_source_files(_swapper.get(), _force_compiled_source_files);
        update_compilation_libraries(_swapper.get(), _libraries);
        // console_redirect_wide _redirect(buffer);
        // save widgets
        _swapper->TriggerManualBuild();
        // load widgets
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

#endif