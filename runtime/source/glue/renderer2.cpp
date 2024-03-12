#include <bungeegum/glue/renderer.hpp>

namespace bungeegum { 
namespace detail {


    

    bool texture_handle::has_value() const
    {
        return _has_value;
    } 

    void texture_handle::reset()
    {
        if (_has_value) {
            _diligent_texture.Release();
            _diligent_texture_view.Release();
            _has_value = false;
        }
    }

}
}