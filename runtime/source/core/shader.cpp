#include <bungeegum/core/global.fwd>
#include <bungeegum/core/shader.hpp>
#include <bungeegum/core/pipeline.hpp>

namespace bungeegum {
namespace detail {

    namespace {

        Diligent::BLEND_FACTOR convert_to_diligent_blend_factor(const shader_blend_factor factor)
        {
            switch (factor)
            {
            case shader_blend_factor::zero:
                return Diligent::BLEND_FACTOR_ZERO;

            case shader_blend_factor::one:
                return Diligent::BLEND_FACTOR_ONE;

            case shader_blend_factor::src_color:
                return Diligent::BLEND_FACTOR_SRC_COLOR;

            case shader_blend_factor::inv_src_color:
                return Diligent::BLEND_FACTOR_INV_SRC_COLOR;

            case shader_blend_factor::src_alpha:
                return Diligent::BLEND_FACTOR_SRC_ALPHA;

            case shader_blend_factor::inv_src_alpha:
                return Diligent::BLEND_FACTOR_INV_SRC_ALPHA;

            case shader_blend_factor::dest_color:
                return Diligent::BLEND_FACTOR_DEST_COLOR;

            case shader_blend_factor::inv_dest_color:
                return Diligent::BLEND_FACTOR_INV_DEST_COLOR;

            case shader_blend_factor::dest_alpha:
                return Diligent::BLEND_FACTOR_DEST_ALPHA;

            case shader_blend_factor::inv_dest_alpha:
                return Diligent::BLEND_FACTOR_INV_DEST_ALPHA;
            
            default:
                throw backtraced_exception("[runtime/core/shader.cpp] invalid enum value"); 
                // return Diligent::BLEND_FACTOR_ZERO;               
            }
        }

        Diligent::BLEND_OPERATION convert_to_diligent_blend_operation(const shader_blend_operation operation)
        {
            switch (operation)
            {
            case shader_blend_operation::add:
                return Diligent::BLEND_OPERATION_ADD;

            case shader_blend_operation::subtract:
                return Diligent::BLEND_OPERATION_SUBTRACT;

            case shader_blend_operation::rev_subtract:
                return Diligent::BLEND_OPERATION_REV_SUBTRACT;

            case shader_blend_operation::min:
                return Diligent::BLEND_OPERATION_MIN;

            case shader_blend_operation::max:
                return Diligent::BLEND_OPERATION_MAX;
            
            default:
                throw backtraced_exception("[runtime/core/shader.cpp] invalid enum value");
                // return Diligent::BLEND_OPERATION_ADD;
            }
        }

        shader_blend_descriptor convert_to_blend_options(const shader_blend_options& blend)
        {
            shader_blend_descriptor _retval;
            _retval.enable = blend.enable;
            _retval.src = detail::convert_to_diligent_blend_factor(blend.src);
            _retval.dest = detail::convert_to_diligent_blend_factor(blend.dest);
            _retval.op = detail::convert_to_diligent_blend_operation(blend.op);
            _retval.src_alpha = detail::convert_to_diligent_blend_factor(blend.src_alpha);
            _retval.dest_alpha = detail::convert_to_diligent_blend_factor(blend.dest_alpha);
            _retval.alpha_op = detail::convert_to_diligent_blend_operation(blend.alpha_op);
            return _retval;
        }

        void reset_shaders(shader_data& data)
        {
            for (std::pair<const std::uintptr_t, shader_handle>& _it : data.shaders) {
                _it.second.reset();
            }
            data.shaders.clear();
        }
    }

    const shader_data& shader_access::get_data(const shader& object)
    {
        return object._data;
    }
    
    shader_data::shader_data()
    {
        raw = raw_cast(this); // create a new id
    }

    shader_data::shader_data(const shader_data& other)
    {
        *this = other;
    }

    shader_data& shader_data::operator=(const shader_data& other)
    {
        reset_shaders(*this);
        raw = raw_cast(this); // create a new id
        is_compiled = other.is_compiled;
        creation_fragment = other.creation_fragment;
        creation_blend = other.creation_blend;
        if (is_compiled) {
            detail::swapped_manager_data& _swapped = detail::get_swapped_global();
            for (std::pair<const std::uintptr_t, std::reference_wrapper<detail::rasterizer_handle>> _pipeline : _swapped.rasterizers) { 
                shaders[_pipeline.first].emplace(
                    _pipeline.second.get(),
                    creation_fragment,
                    creation_blend);
            }
        }
        return *this;
    }

    shader_data::shader_data(shader_data&& other)
    {
        *this = std::move(other);
    }

    shader_data& shader_data::operator=(shader_data&& other)
    {        
        reset_shaders(*this);
        raw = other.raw; // keep the same id
        is_compiled = std::move(other.is_compiled);
        creation_fragment = std::move(other.creation_fragment);
        creation_blend = std::move(other.creation_blend);
        shaders = std::move(other.shaders); // just move handles
        return *this;
    }

    shader_data::~shader_data()
    {
        reset_shaders(*this);
    }

}

shader::shader(const std::string& fragment, const shader_blend_options& blend)
{
    detail::reset_shaders(_data);
    _data.creation_fragment = fragment;
    _data.creation_blend = detail::convert_to_blend_options(blend);
    detail::swapped_manager_data& _swapped = detail::get_swapped_global();
    for (std::pair<const std::uintptr_t, std::reference_wrapper<detail::rasterizer_handle>> _pipeline : _swapped.rasterizers) { 
        _data.shaders[_pipeline.first].emplace(
            _pipeline.second.get(),
            _data.creation_fragment,
            _data.creation_blend);
    }
    _data.is_compiled = true;
}

template <>
shader& shader::uniform<float>(const std::string& name, const float& value)
{
    return *this;
}

}