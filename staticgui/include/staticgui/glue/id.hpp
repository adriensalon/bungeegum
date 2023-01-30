//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <vector>

namespace staticgui {
namespace glue {
    namespace id {

        using integer = unsigned long long;

        struct generator {
            static integer create();

            static integer next();

            static void destroy(const integer id);

        private:
            inline static integer _count = 0;
            inline static std::vector<integer> _deleted;
        };

    }
}
}