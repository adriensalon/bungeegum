#include <bungeegum/glue/dialog.hpp>

#if BUNGEEGUM_USE_DIALOG

#include <nfd.h>

namespace bungeegum {
namespace detail {

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
}
}

#endif