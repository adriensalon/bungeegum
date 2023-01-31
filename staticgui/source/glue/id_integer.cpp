//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <staticgui/glue/id_integer.hpp>

namespace staticgui {
namespace glue {

    id_integer id_generator::create()
    {
        id_integer _retval;
        if (!_deleted.empty()) {
            _retval = _deleted.back();
            _deleted.pop_back();
        } else
            _retval = _count++;
        return _retval;
    }

    id_integer id_generator::next()
    {
        id_integer _retval;
        if (!_deleted.empty()) {
            _retval = _deleted.back();
        } else
            _retval = _count + 1;
        return _retval;
    }

    void id_generator::destroy(const id_integer id)
    {
        _deleted.emplace_back(id);
    }

}
}