
#include <hscpp/Hotswapper.h>
#include <hscpp/mem/MemoryManager.h>

#include <bungeegum/glue/reload.fwd>

namespace bungeegum {
namespace detail {

    namespace {

        struct singleton_data {
            std::unique_ptr<hscpp::Hotswapper> swapper = nullptr;
            std::unique_ptr<hscpp::mem::MemoryManager> manager = nullptr;
        };

    }

    uint1 allocated_blocks_count()
    {
        return 0;
    }

}
}