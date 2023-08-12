#include <hscpp/Hotswapper.h>
#include <hscpp/mem/MemoryManager.h>

#include <bungeegum/glue/console.hpp>
#include <bungeegum/glue/reload.hpp>

#if BUNGEEGUM_USE_HOTSWAP

namespace bungeegum {
namespace detail {

    namespace {

        void update_compilation_defines(hscpp::Hotswapper* hotswapper, std::pair<std::size_t, std::vector<std::string>>& defines)
        {
            if (defines.second.size() != defines.first) {
                hotswapper->ClearPreprocessorDefinitions();
                for (const std::string& _define : defines.second) {
                    hotswapper->AddPreprocessorDefinition(_define);
                }
                defines.first = defines.second.size();
            }
            bool _found = false;
            hotswapper->EnumeratePreprocessorDefinitions([&_found](int handle, const std::string& definition) {
                if (definition == "__HOTRELOADING__") {
                    _found = true;
                }
                (void)handle;
            });
            if (!_found) {
                hotswapper->AddPreprocessorDefinition("__HOTRELOADING__");
            }
        }

        void update_compilation_include_directories(hscpp::Hotswapper* hotswapper, std::pair<std::size_t, std::vector<std::filesystem::path>>& include_directories)
        {
            if (include_directories.second.size() != include_directories.first) {
                hotswapper->ClearIncludeDirectories();
                for (const std::filesystem::path& _include_directory : include_directories.second) {
                    hotswapper->AddIncludeDirectory(_include_directory);
                }
                include_directories.first = include_directories.second.size();
            }
        }

        void update_compilation_source_directories(hscpp::Hotswapper* hotswapper, std::pair<std::size_t, std::vector<std::filesystem::path>>& source_directories)
        {
            if (source_directories.second.size() != source_directories.first) {
                hotswapper->ClearSourceDirectories();
                for (const std::filesystem::path& _source_directory : source_directories.second) {
                    hotswapper->AddSourceDirectory(_source_directory);
                }
                source_directories.first = source_directories.second.size();
            }
        }

        void update_compilation_force_compiled_source_files(hscpp::Hotswapper* hotswapper, std::pair<std::size_t, std::vector<std::filesystem::path>>& force_compiled_source_files)
        {
            if (force_compiled_source_files.second.size() != force_compiled_source_files.first) {
                hotswapper->ClearForceCompiledSourceFiles();
                for (const std::filesystem::path& _force_compiled_source_file : force_compiled_source_files.second) {
                    hotswapper->AddForceCompiledSourceFile(_force_compiled_source_file);
                }
                force_compiled_source_files.first = force_compiled_source_files.second.size();
            }
        }

        void update_compilation_libraries(hscpp::Hotswapper* hotswapper, std::pair<std::size_t, std::vector<std::filesystem::path>>& libraries)
        {
            if (libraries.second.size() != libraries.first) {
                hotswapper->ClearLibraries();
                for (const std::filesystem::path& _force_compiled_source_file : libraries.second) {
                    hotswapper->AddLibrary(_force_compiled_source_file);
                }
                libraries.first = libraries.second.size();
            }
        }

    }

    reloader::reloader(std::wstreambuf* buffer)
    {
        (void)buffer;
        // console_redirect_wide _redirect(buffer);
        _swapper = std::make_shared<hscpp::Hotswapper>();
        _manager = std::make_shared<hscpp::mem::UniqueRef<hscpp::mem::MemoryManager>>(hscpp::mem::MemoryManager::Create());
        _swapper->SetAllocator((_manager.get()->operator->()));
        // _swapper->EnableFeature(hscpp::Feature::ManualCompilationOnly);
    }

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

    reload_state reloader::update(std::wstreambuf* buffer)
    {
        update_compilation_defines(_swapper.get(), _defines);
        update_compilation_include_directories(_swapper.get(), _include_directories);
        update_compilation_source_directories(_swapper.get(), _source_directories);
        update_compilation_force_compiled_source_files(_swapper.get(), _force_compiled_source_files);
        update_compilation_libraries(_swapper.get(), _libraries);
        // console_redirect_wide _redirect(buffer);
        (void)buffer;
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