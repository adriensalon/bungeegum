#include <bungeegum/glue/dialog.fwd>

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif
#if (BUNGEEGUM_USE_OVERLAY && !TOOLCHAIN_PLATFORM_EMSCRIPTEN)
#include <nfd.h>
#endif

namespace bungeegum {
namespace detail {

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
    EM_JS(void, emscripten_download_text_file, (const char* content, const char* file_name, const char* mime_type), {
		var element = document.createElement('a');
		element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(UTF8ToString(content)));
		element.setAttribute('download', UTF8ToString(file_name));
		element.style.display = 'none';
		document.body.appendChild(element);
		element.click();
		document.body.removeChild(element);
    });

    EM_JS(void, emscripten_download_file, (const void* buffer, const std::size_t buffer_size, const char* file_name, const char* mime_type), {
        var element = document.createElement('a');
        element.download = UTF8ToString(file_name);
        element.href = URL.createObjectURL(new Blob([new Uint8Array(Module.HEAPU8.buffer, buffer, buffer_size)], {type: UTF8ToString(mime_type) }));
        element.click();
		document.body.removeChild(element);
    });

	// void download_file_from_disk(const std::filesystem::path& file_path, const std::string& file_name, const std::string& mime_type)
	// {		
		
	// }

    void download_file_from_string(const std::string& content, const std::string& file_name, const std::string& mime_type)
    {
		const char* _content_cstr = content.c_str();
		const char* _file_name_cstr = file_name.c_str();
		const char* _mime_type_cstr = mime_type.c_str();
		emscripten_download_text_file(_content_cstr, _file_name_cstr, _mime_type_cstr);
    }

	template <typename value_t>
    void download_file_from_memory(const value_t& content, const std::string& file_name, const std::string& mime_type)
    {
		const void* _buffer = static_cast<const void*>(std::addressof(content));
		const std::size_t _buffer_size = sizeof(value_t);
		const char* _file_name_cstr = file_name.c_str();
		const char* _mime_type_cstr = mime_type.c_str();
		emscripten_download_file(_buffer, _buffer_size, _file_name_cstr, _mime_type_cstr);
    }
#endif

#if (BUNGEEGUM_USE_OVERLAY && !TOOLCHAIN_PLATFORM_EMSCRIPTEN)
    namespace {

        struct nfd_filters {
            nfd_filters() = delete;
            nfd_filters(const std::vector<dialog_extensions_filter>& filters)
            {
                const std::size_t _filters_size = filters.size();
                _fused_filters_str.reserve(_filters_size);
                _nfd_filters.reserve(_filters_size);
                for (const dialog_extensions_filter& _filter : filters) {
                    std::string& _fused_str = _fused_filters_str.emplace_back();
                    if (!_filter.extensions.empty()) {
                        _fused_str += _filter.extensions.front();
                    }
                    for (std::size_t _k = 1u; _k < _filter.extensions.size(); ++_k) {
                        _fused_str += ", " + _filter.extensions[_k];
                    }
                    _nfd_filters.push_back(nfdfilteritem_t { _filter.description.c_str(), _fused_str.c_str() });
                }
            }
            nfd_filters(const nfd_filters& other) = delete;
            nfd_filters& operator=(const nfd_filters& other) = delete;
            nfd_filters(nfd_filters&& other) = delete;
            nfd_filters& operator=(nfd_filters&& other) = delete;

            const nfdfilteritem_t* data() const
            {
                return _nfd_filters.data();
            }

            nfdfiltersize_t size() const
            {
                return static_cast<nfdfiltersize_t>(_nfd_filters.size());
            }

        private:
            std::vector<std::string> _fused_filters_str = {};
            std::vector<nfdfilteritem_t> _nfd_filters = {};
        };

        struct nfd_default_path {
            nfd_default_path() = delete;
            nfd_default_path(const std::optional<std::filesystem::path>& path)
            {
                if (path.has_value()) {
                    _path_str = path.value().generic_string();
                }
            }
            nfd_default_path(const nfd_default_path& other) = delete;
            nfd_default_path& operator=(const nfd_default_path& other) = delete;
            nfd_default_path(nfd_default_path&& other) = delete;
            nfd_default_path& operator=(nfd_default_path&& other) = delete;

            const nfdchar_t* data() const
            {
                return _path_str.empty() ? NULL : _path_str.c_str();
            }

        private:
            std::string _path_str = {};
        };

        struct nfd_default_name {
            nfd_default_name() = delete;
            nfd_default_name(const std::optional<std::string>& name)
            {
                if (name.has_value()) {
                    _name_str = name.value();
                }
            }
            nfd_default_name(const nfd_default_name& other) = delete;
            nfd_default_name& operator=(const nfd_default_name& other) = delete;
            nfd_default_name(nfd_default_name&& other) = delete;
            nfd_default_name& operator=(nfd_default_name&& other) = delete;

            const nfdchar_t* data() const
            {
                return _name_str.empty() ? NULL : _name_str.c_str();
            }

        private:
            std::string _name_str = {};
        };

        struct nfd_result {
            nfd_result() = default;
            nfd_result(const nfd_result& other) = delete;
            nfd_result& operator=(const nfd_result& other) = delete;
            nfd_result(nfd_result&& other) = delete;
            nfd_result& operator=(nfd_result&& other) = delete;

            nfdchar_t** data()
            {
                return &(_nfd_output);
            }

            std::optional<std::filesystem::path> process(const nfdresult_t result) const
            {
                if (result != NFD_OKAY) {
                    return std::nullopt;
                }
                std::string _result_str(_nfd_output);
                NFD_FreePath(_nfd_output);
                return std::filesystem::path(_result_str);
            }

        private:
            nfdchar_t* _nfd_output = nullptr;
        };
    }

    std::optional<std::filesystem::path> open_file_dialog(
        const std::vector<dialog_extensions_filter>& filters,
        const std::optional<std::filesystem::path>& default_path)
    {
        nfd_result _result;
        nfd_filters _filters(filters);
        nfd_default_path _default_path(default_path);
        return _result.process(NFD_OpenDialog(_result.data(), _filters.data(), _filters.size(), _default_path.data()));
    }

    std::optional<std::filesystem::path> save_file_dialog(
        const std::vector<dialog_extensions_filter>& filters,
        const std::optional<std::filesystem::path>& default_path,
        const std::optional<std::string>& default_name)
    {
        nfd_result _result;
        nfd_filters _filters(filters);
        nfd_default_path _default_path(default_path);
        nfd_default_name _default_name(default_name);
        return _result.process(NFD_SaveDialog(_result.data(), _filters.data(), _filters.size(), _default_path.data(), _default_name.data()));
    }

    std::optional<std::filesystem::path> pick_folder_dialog(
        const std::optional<std::filesystem::path>& default_path)
    {
        nfd_result _result;
        nfd_default_path _default_path(default_path);
        return _result.process(NFD_PickFolder(_result.data(), _default_path.data()));
    }
#endif
}
}