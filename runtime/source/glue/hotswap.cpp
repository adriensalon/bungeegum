#include <bungeegum/glue/console.hpp>
#include <bungeegum/glue/hotswap.hpp>

#if BUNGEEGUM_USE_HOTSWAP

#include <hscpp/Hotswapper.h>
#include <hscpp/mem/MemoryManager.h>

namespace bungeegum {
namespace detail {

    namespace {

        void update_compilation_defines(hscpp::Hotswapper* hotswapper, std::pair<std::size_t, std::vector<std::string>>& defines)
        {
            if (defines.second.size() != defines.first) {
                // hotswapper->ClearPreprocessorDefinitions();
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
                // hotswapper->ClearIncludeDirectories();
                for (const std::filesystem::path& _include_directory : include_directories.second) {
                    hotswapper->AddIncludeDirectory(_include_directory);
                }
                include_directories.first = include_directories.second.size();
            }
        }

        void update_compilation_source_directories(hscpp::Hotswapper* hotswapper, std::pair<std::size_t, std::vector<std::filesystem::path>>& source_directories)
        {
            if (source_directories.second.size() != source_directories.first) {
                // hotswapper->ClearSourceDirectories();
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
                // hotswapper->ClearLibraries();
                for (const std::filesystem::path& _force_compiled_source_file : libraries.second) {
                    hotswapper->AddLibrary(_force_compiled_source_file);
                }
                libraries.first = libraries.second.size();
            }
        }

    }

    void swapper_handle::emplace(std::wstreambuf* buffer)
    {
        reset();
        redirect_wide_guard _redirect(buffer);
        _swapper = std::make_shared<hscpp::Hotswapper>();
        _manager = std::make_shared<hscpp::mem::UniqueRef<hscpp::mem::MemoryManager>>(hscpp::mem::MemoryManager::Create());
        _swapper->SetAllocator((_manager.get()->operator->()));
        // _swapper->ClearCompileOptions();
        // _swapper->AddCompileOption("/nologo");
        // _swapper->AddCompileOption("/Z7");
        // _swapper->AddCompileOption("/FC");
        // _swapper->AddCompileOption("/EHsc");
        // _swapper->AddCompileOption("/MP");
        // _swapper->AddCompileOption("/MDd");
        // _swapper->AddCompileOption("/LDd");
        // _swapper->AddCompileOption("/std:c++17");
        // // _swapper->AddLinkOption("/VERBOSE");
        // // _swapper->AddLinkOption("/LTCG:OFF");
        // _swapper->AddLinkOption("/OPT:NOREF");
        // _swapper->AddLinkOption("/OPT:ICF");
        _has_value = true;
    }
    
    bool swapper_handle::has_value() const
    {
        return _has_value;
    }

    void swapper_handle::reset()
    {
        if (_has_value) {

        }
    }

    swapper_state swapper_handle::update(std::wstreambuf* buffer)
    {
        update_compilation_defines(_swapper.get(), _defines);
        update_compilation_include_directories(_swapper.get(), _include_directories);
        update_compilation_source_directories(_swapper.get(), _source_directories);
        update_compilation_force_compiled_source_files(_swapper.get(), _force_compiled_source_files);
        update_compilation_libraries(_swapper.get(), _libraries);
        redirect_wide_guard _redirect(buffer);
        return static_cast<swapper_state>(_swapper->Update());
    }

    void swapper_handle::force_update(std::wstreambuf* buffer)
    {
        update_compilation_defines(_swapper.get(), _defines);
        update_compilation_include_directories(_swapper.get(), _include_directories);
        update_compilation_source_directories(_swapper.get(), _source_directories);
        update_compilation_force_compiled_source_files(_swapper.get(), _force_compiled_source_files);
        update_compilation_libraries(_swapper.get(), _libraries);
        redirect_wide_guard _redirect(buffer);
        _swapper->TriggerManualBuild();
    }

    std::size_t swapper_handle::allocated_blocks_count()
    {
        return _manager.get()->operator->()->GetNumBlocks();
    }

    std::vector<std::string>& swapper_handle::defines()
    {
        return _defines.second;
    }

    std::vector<std::filesystem::path>& swapper_handle::include_directories()
    {
        return _include_directories.second;
    }

    std::vector<std::filesystem::path>& swapper_handle::source_directories()
    {
        return _source_directories.second;
    }

    std::vector<std::filesystem::path>& swapper_handle::force_compiled_source_files()
    {
        return _force_compiled_source_files.second;
    }

    std::vector<std::filesystem::path>& swapper_handle::libraries()
    {
        return _libraries.second;
    }

    void swapper_handle::set_global_data(void* data_ptr)
    {
        _swapper->SetGlobalUserData(data_ptr);
    }

    swapped_load_guard::swapped_load_guard(const std::filesystem::path& archive_path)
        : _fstream(archive_path)
        , _archive(_fstream.value())
    {
    }

    swapped_load_guard::swapped_load_guard(std::stringstream& archive_stream)
        : _sstream(archive_stream)
        , _archive(_sstream.value().get())
    {
    }

    swapped_save_guard::swapped_save_guard(const std::filesystem::path& archive_path)
        : _fstream(archive_path)
        , _archive(_fstream.value())
    {
    }

    swapped_save_guard::swapped_save_guard(std::stringstream& archive_stream)
        : _sstream(archive_stream)
        , _archive(_sstream.value().get())
    {
    }
}
}

#endif