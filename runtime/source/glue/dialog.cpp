#include <bungeegum/glue/dialog.hpp>

#if BUNGEEGUM_USE_DIALOG

#include <nfd.h>

namespace bungeegum {
namespace detail {

    std::optional<std::filesystem::path> open_file_dialog(const std::vector<dialog_extensions_filter>& filters)
    {
        std::vector<std::string> _fused_extensions_filters = {};
        std::vector<nfdfilteritem_t> _nfd_filters = {};
        nfdchar_t* _nfd_path = nullptr;
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
        nfdresult_t result = NFD_OpenDialog(&_nfd_path, _nfd_filters.data(), (nfdfiltersize_t)_nfd_filters.size(), NULL);
        if (result == NFD_OKAY) {
            std::string _retval(_nfd_path);
            NFD_FreePath(_nfd_path);
            return std::filesystem::path(_retval);
        } else {
            return std::nullopt;
        }
    }

    std::optional<std::filesystem::path> save_file_dialog(const std::vector<dialog_extensions_filter>& filters)
    {
        (void)filters;
        return std::nullopt;
    }

}
}

#endif