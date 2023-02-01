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

#include <entt/entt.hpp>

namespace staticgui {
namespace glue {

    using id_integer = ENTT_ID_TYPE;

    struct id_generator {
        static id_integer create();

        static id_integer next();

        static void destroy(const id_integer id);

    private:
        inline static id_integer _count = 0;
        inline static std::vector<id_integer> _deleted;
    };

}
}