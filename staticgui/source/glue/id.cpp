//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <staticgui/glue/id.hpp>

namespace staticgui {
namespace glue {
    namespace id {

        integer generator::create()
        {
            integer _retval;
            if (!_deleted.empty()) {
                _retval = _deleted.back();
                _deleted.pop_back();
            } else
                _retval = _count++;
            return _retval;
        }

        integer generator::next()
        {
            integer _retval;
            if (!_deleted.empty()) {
                _retval = _deleted.back();
            } else
                _retval = _count + 1;
            return _retval;
        }

        void generator::destroy(const integer id)
        {
            _deleted.emplace_back(id);
        }

    }
}
}