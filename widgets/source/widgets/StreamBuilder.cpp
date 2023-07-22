#include <bungeegum_widgets/widgets/StreamBuilder.hpp>

namespace bungeegum {
namespace widgets {

    // StreamBuilder

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
        _customBuffer = StreamBuilderBuffer(stream, buffer_size);
        _customBuffer.value().flushCallback = [this](const std::string& buffer) {
            if (!_flushCallback) {
                log_error("Error in StreamBuilder.cpp : _flushCallback is nullptr");
            }
            if (_childWidget.has_value())
                abandon(this, _childWidget.value());
            _childWidget = _flushCallback(buffer);
            adopt(this, _childWidget.value());
            must_resolve(this);
        };
        using _char_type = StreamBuilderBuffer::char_type;
        std::streambuf* _formerStreambuf = stream.basic_ios<_char_type>::rdbuf(&_customBuffer.value());
        _restoreFormerBuffer = [this, &stream, _formerStreambuf]() {
            stream.basic_ios<_char_type>::rdbuf(_formerStreambuf);
        };
        return *this;
    }

    void StreamBuilder::processInitialData()
    {
        if (!_initialDataBuildDone && _flushCallback && _initialData.has_value()) {
            _flushCallback(_initialData.value());
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

    StreamBuilder::~StreamBuilder()
    {
        if (_restoreFormerBuffer) {
            _restoreFormerBuffer();
        }
    }

    // StreamBuilderBuffer

    StreamBuilder::StreamBuilderBuffer::StreamBuilderBuffer(std::ostream& sink, const uint1 buffer_size)
        : _sink(std::ref(sink))
        , _buffer(static_cast<std::size_t>(buffer_size) + 1)
    {
        _sink.get().clear();
        char_type* _base = &_buffer.front();
        setp(_base, _base + _buffer.size() - 1);
    }

    bool1 StreamBuilder::StreamBuilderBuffer::triggerAndFlush()
    {
        std::ptrdiff_t _count = pptr() - pbase();
        std::string _str(pbase(), _count);
        if (flushCallback != nullptr) {
            flushCallback(_str);
        }
        pbump(-static_cast<int_type>(_count));
        return _sink.get().write(pbase(), _count).good();
    }

    StreamBuilder::StreamBuilderBuffer::int_type StreamBuilder::StreamBuilderBuffer::overflow(int_type character)
    {
        if (_sink.get() && character != traits_type::eof()) {
            if (!std::less_equal<char_type*>()(pptr(), epptr())) {
                log_error("Error in StreamBuilder.cpp : StreamBuilderBuffer overflow");
            }
            *pptr() = static_cast<char_type>(character);
            pbump(1);
            if (triggerAndFlush()) {
                return character;
            }
        }
        return traits_type::eof();
    }

    int1 StreamBuilder::StreamBuilderBuffer::sync()
    {
        return triggerAndFlush() ? 0 : -1;
    }

    // WideStreamBuilder

    WideStreamBuilder& WideStreamBuilder::builder(const std::function<runtime_widget(const std::wstring&)>& value)
    {
        _flushCallback = value;
        processInitialData();
        return *this;
    }

    WideStreamBuilder& WideStreamBuilder::initialData(const std::wstring& value)
    {
        _initialData = value;
        processInitialData();
        return *this;
    }

    WideStreamBuilder& WideStreamBuilder::stream(std::wostream& stream, const uint1 buffer_size)
    {
        _customBuffer = WideStreamBuilderBuffer(stream, buffer_size);
        _customBuffer.value().flushCallback = [this](const std::wstring& buffer) {
            if (!_flushCallback) {
                log_error("Error in StreamBuilder.cpp : _flushCallback is nullptr");
            }
            if (_childWidget.has_value())
                abandon(this, _childWidget.value());
            _childWidget = _flushCallback(buffer);
            adopt(this, _childWidget.value());
            must_resolve(this);
        };
        using _char_type = WideStreamBuilderBuffer::char_type;
        std::wstreambuf* _formerStreambuf = stream.basic_ios<_char_type>::rdbuf(&_customBuffer.value());
        _restoreFormerBuffer = [this, &stream, _formerStreambuf]() {
            if (&stream != nullptr) {
                stream.basic_ios<_char_type>::rdbuf(_formerStreambuf);
            }
        };
        return *this;
    }

    void WideStreamBuilder::processInitialData()
    {
        if (!_initialDataBuildDone && _flushCallback && _initialData.has_value()) {
            _flushCallback(_initialData.value());
            _initialDataBuildDone = true;
        }
    }

    void WideStreamBuilder::resolve(resolve_command& command)
    {
        if (_childWidget.has_value()) {
            float2 _childSize = command.resolve_child(_childWidget.value(), command.min_size(), command.max_size());
            command.position_child(_childWidget.value(), zero<float2>);
            command.resize(_childSize);
        } else {
            command.resize(command.max_size());
        }
    }

    WideStreamBuilder::~WideStreamBuilder()
    {
        if (_restoreFormerBuffer) {
            _restoreFormerBuffer();
        }
    }

    // WideStreamBuilderBuffer

    WideStreamBuilder::WideStreamBuilderBuffer::WideStreamBuilderBuffer(std::wostream& sink, const uint1 buffer_size)
        : _sink(std::ref(sink))
        , _buffer(static_cast<std::size_t>(buffer_size) + 1)
    {
        _sink.get().clear();
        char_type* _base = &_buffer.front();
        setp(_base, _base + _buffer.size() - 1);
    }

    bool1 WideStreamBuilder::WideStreamBuilderBuffer::triggerAndFlush()
    {
        std::ptrdiff_t _count = pptr() - pbase();
        std::wstring _str(pbase(), _count);
        if (flushCallback != nullptr) {
            flushCallback(_str);
        }
        pbump(-static_cast<int_type>(_count));
        return _sink.get().write(pbase(), _count).good();
    }

    WideStreamBuilder::WideStreamBuilderBuffer::int_type WideStreamBuilder::WideStreamBuilderBuffer::overflow(int_type character)
    {
        if (_sink.get() && character != traits_type::eof()) {
            if (!std::less_equal<char_type*>()(pptr(), epptr())) {
                log_error("Error in StreamBuilder.cpp : WideStreamBuilderBuffer overflow");
            }
            *pptr() = static_cast<char_type>(character);
            pbump(1);
            if (triggerAndFlush()) {
                return character;
            }
        }
        return traits_type::eof();
    }

    int1 WideStreamBuilder::WideStreamBuilderBuffer::sync()
    {
        return triggerAndFlush() ? 0 : -1;
    }
}
}