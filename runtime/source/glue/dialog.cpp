#include <bungeegum/glue/dialog.hpp>

// f*ck C ! (although this code sucks too)

#if BUNGEEGUM_USE_DIALOG

#include <nfd.h>

namespace bungeegum {
namespace detail {

    std::optional<std::filesystem::path> open_file_dialog(
        const std::vector<dialog_extensions_filter>& filters,
        const std::optional<std::filesystem::path>& default_path)
    {
        // filters
        std::vector<std::string> _fused_extensions_filters = {};
        std::vector<nfdfilteritem_t> _nfd_filters = {};
        for (const dialog_extensions_filter& _filter : filters) {
            std::string& _fused_extensions = _fused_extensions_filters.emplace_back();
            if (!_filter.extensions.empty()) {
                _fused_extensions += _filter.extensions.front();
            }
            for (std::size_t _k = 1u; _k < _filter.extensions.size(); ++_k) {
                _fused_extensions += ", " + _filter.extensions[_k];
            }
            _nfd_filters.emplace_back(nfdfilteritem_t { _filter.description.c_str(), _fused_extensions.c_str() });
        }

        // default path
        std::string _default_path_str;
        const nfdchar_t* _nfd_default_path = NULL;
        if (default_path.has_value()) {
            _default_path_str = default_path.value().generic_string();
            _nfd_default_path = _default_path_str.c_str();
        }

        // NFD call
        nfdchar_t* _nfd_path = NULL;
        nfdresult_t _nfd_result = NFD_OpenDialog(&_nfd_path, _nfd_filters.data(), (nfdfiltersize_t)_nfd_filters.size(), _nfd_default_path);

        // result
        if (_nfd_result == NFD_OKAY) {
            std::string _result_str(_nfd_path);
            NFD_FreePath(_nfd_path);
            return std::filesystem::path(_result_str);
        } else {
            return std::nullopt;
        }
    }

    std::optional<std::filesystem::path> save_file_dialog(
        const std::vector<dialog_extensions_filter>& filters,
        const std::optional<std::filesystem::path>& default_path,
        const std::optional<std::string>& default_name)
    {
        // filters
        std::vector<std::string> _fused_extensions_filters = {};
        std::vector<nfdfilteritem_t> _nfd_filters = {};
        for (const dialog_extensions_filter& _filter : filters) {
            std::string& _fused_extensions = _fused_extensions_filters.emplace_back();
            if (!_filter.extensions.empty()) {
                _fused_extensions += _filter.extensions.front();
            }
            for (std::size_t _k = 1u; _k < _filter.extensions.size(); ++_k) {
                _fused_extensions += ", " + _filter.extensions[_k];
            }
            _nfd_filters.emplace_back(nfdfilteritem_t { _filter.description.c_str(), _fused_extensions.c_str() });
        }

        // default path
        std::string _default_path_str;
        const nfdchar_t* _nfd_default_path = NULL;
        if (default_path.has_value()) {
            _default_path_str = default_path.value().generic_string();
            _nfd_default_path = _default_path_str.c_str();
        }

        // default name
        const nfdchar_t* _nfd_default_name = NULL;
        if (default_name.has_value()) {
            _nfd_default_name = default_name.value().c_str();
        }

        // NFD call
        nfdchar_t* _nfd_path = NULL;
        nfdresult_t _nfd_result = NFD_SaveDialog(&_nfd_path, _nfd_filters.data(), (nfdfiltersize_t)_nfd_filters.size(), _nfd_default_path, _nfd_default_name);

        // result
        if (_nfd_result == NFD_OKAY) {
            std::string _result_str(_nfd_path);
            NFD_FreePath(_nfd_path);
            return std::filesystem::path(_result_str);
        } else {
            return std::nullopt;
        }
    }

    std::optional<std::filesystem::path> pick_folder_dialog(
        const std::optional<std::filesystem::path>& default_path)
    {
        // default path
        std::string _default_path_str;
        const nfdchar_t* _nfd_default_path = NULL;
        if (default_path.has_value()) {
            _default_path_str = default_path.value().generic_string();
            _nfd_default_path = _default_path_str.c_str();
        }

        // NFD call
        nfdchar_t* _nfd_path = NULL;
        nfdresult_t _nfd_result = NFD_PickFolder(&_nfd_path, _nfd_default_path);

        // result
        if (_nfd_result == NFD_OKAY) {
            std::string _result_str(_nfd_path);
            NFD_FreePath(_nfd_path);
            return std::filesystem::path(_result_str);
        } else {
            return std::nullopt;
        }
    }
}
}

#endif