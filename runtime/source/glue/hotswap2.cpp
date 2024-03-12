

#include <bungeegum/glue/hotswap.hpp>

#include <hscpp/Hotswapper.h>

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

    reloader::reloader(std::wstreambuf* buffer)
    {
        (void)buffer;
        // redirect_wide_guard _redirect(buffer);
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
    }

    reload_state reloader::update(std::wstreambuf* buffer)
    {
        update_compilation_defines(_swapper.get(), _defines);
        update_compilation_include_directories(_swapper.get(), _include_directories);
        update_compilation_source_directories(_swapper.get(), _source_directories);
        update_compilation_force_compiled_source_files(_swapper.get(), _force_compiled_source_files);
        update_compilation_libraries(_swapper.get(), _libraries);
        // redirect_wide_guard _redirect(buffer);
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
        // redirect_wide_guard _redirect(buffer);
        // save widgets
        _swapper->TriggerManualBuild();
        // load widgets
    }

}
}