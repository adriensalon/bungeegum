#include <iosfwd>

#include <bungeegum_widgets/widgets/StreamBuilder.hpp>

namespace bungeegum {
namespace widgets {

    namespace detail {

        StreamBuilderBuffer::StreamBuilderBuffer(std::ostream& sink, std::size_t buffer_size)
            : _sink(std::ref(sink))
            , _buffer(buffer_size + 1)
        {
            _sink.get().clear();
            char_type* _base = &_buffer.front();
            setp(_base, _base + _buffer.size() - 1);
        }

        bool1 StreamBuilderBuffer::triggerAndFlush()
        {
            std::ptrdiff_t _count = pptr() - pbase();
            std::string _str(pbase(), _count);
            if (flushCallback != nullptr) {
                flushCallback(_str);
            }
            pbump(-static_cast<int_type>(_count));
            return _sink.get().write(pbase(), _count).good();
        }

        StreamBuilderBuffer::int_type StreamBuilderBuffer::overflow(int_type character)
        {
            if (_sink.get() && character != traits_type::eof()) {
                if (!std::less_equal<char_type*>()(pptr(), epptr())) {
                    throw_error("Error in StreamBuilder.cpp : event_buffer overflow error");
                }
                *pptr() = static_cast<char_type>(character);
                pbump(1);
                if (triggerAndFlush()) {
                    return character;
                }
            }
            return traits_type::eof();
        }

        StreamBuilderBuffer::int_type StreamBuilderBuffer::sync()
        {
            return triggerAndFlush() ? 0 : -1;
        }
    }

    StreamBuilder& StreamBuilder::builder(const std::function<runtime_widget(const std::string&)>& value)
    {
        _flushCallback = value;
        processInitialData();
        return *this;
    }

    StreamBuilder& StreamBuilder::initialData(const std::string& value)
    {
        _initialData = value;
        processInitialData();
        return *this;
    }

    StreamBuilder& StreamBuilder::stream(std::ostream& stream, const uint1 buffer_size)
    {
        _customBuffer = detail::StreamBuilderBuffer(stream, buffer_size);
        _customBuffer.value().flushCallback = [this](const std::string& buffer) {
            _flushCallback.value()(buffer);
        };
        _formerStreambuf = stream.basic_ios<char>::rdbuf(&_customBuffer.value());
        return *this;
    }

    void StreamBuilder::processInitialData()
    {
        if (!_initialDataBuildDone && _flushCallback.has_value() && _initialData.has_value()) {
            _flushCallback.value()(_initialData.value());
            _initialDataBuildDone = true;
        }
    }

    void StreamBuilder::resolve(resolve_command& command)
    {
        if (_childWidget.has_value()) {
            float2 _childSize = command.resolve_child(_childWidget.value(), command.min_size(), command.max_size());
            command.position_child(_childWidget.value(), zero<float2>);
            command.resize(_childSize);
        } else {
            command.resize(command.max_size());
        }
    }

    /// WIDE

}
}