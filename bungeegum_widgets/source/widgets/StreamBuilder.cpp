#include <cstdlib>
#include <iosfwd>

#include <bungeegum_widgets/widgets/StreamBuilder.hpp>

namespace bungeegum {
namespace widgets {

    namespace detail {

        event_buffer::event_buffer(std::ostream& sink, std::size_t buffer_size)
            : _sink(std::ref(sink))
            , _buffer(buffer_size + 1)
        {
            _sink.get().clear();
            char_type* _base = &_buffer.front();
            setp(_base, _base + _buffer.size() - 1);
        }

        // event_buffer::event_buffer(const event_buffer& other)
        //     : _sink(other._sink)
        // {
        //     *this = other;
        // }

        // event_buffer& event_buffer::operator=(const event_buffer& other)
        // {
        //     _cap_next = other._cap_next;
        //     _sink = other._sink;
        //     _buffer = other._buffer;
        //     return *this;
        // }

        bool event_buffer::trigger_and_flush()
        {
            // TODO
            for (char_type *p = pbase(), *e = pptr(); p != e; ++p) {
                std::cout << *p << std::endl;
            }
            std::ptrdiff_t _n = pptr() - pbase();
            pbump(-static_cast<int_type>(_n));
            return _sink.get().write(pbase(), _n).good();
        }

        event_buffer::int_type event_buffer::overflow(int_type ch)
        {
            if (_sink.get() && ch != traits_type::eof()) {
                if (!std::less_equal<char_type*>()(pptr(), epptr())) {
                    throw_error("Error in StreamBuilder.cpp : event_buffer overflow error");
                }
                *pptr() = static_cast<char_type>(ch);
                pbump(1);
                if (trigger_and_flush()) {
                    return ch;
                }
            }
            return traits_type::eof();
        }

        event_buffer::int_type event_buffer::sync()
        {
            return trigger_and_flush() ? 0 : -1;
        }

    }

    StreamBuilder& StreamBuilder::builder(const std::function<runtime_widget(const std::string&)>& value)
    {
        // TODO
        (void)value;
        return *this;
    }

    StreamBuilder& StreamBuilder::initialData(const std::string& value)
    {
        if (_builderFunction.has_value()) {
            _builderFunction.value()(value);
        } else {
            _initialData = value;
        }
        return *this;
    }

    StreamBuilder& StreamBuilder::stream(std::ostream& value)
    {
        _customBuffer = detail::event_buffer(value, 256u);
        _formerStreambuf = value.rdbuf(&_customBuffer.value());
        return *this;
    }

    void StreamBuilder::resolve(resolve_command& command)
    {
        (void)command;
    }

    /// WIDE

}
}