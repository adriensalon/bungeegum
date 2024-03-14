#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/string.hpp>

#if BUNGEEGUM_USE_BACKTRACE
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#else
#include <backward.hpp>
#endif
#endif

#if (BUNGEEGUM_USE_BACKTRACE && TOOLCHAIN_PLATFORM_EMSCRIPTEN)
extern "C" {
void EMSCRIPTEN_KEEPALIVE bungegum_emscripten_try_from_js(int try_cpp_function)
{
    const std::function<void()>& _try_cpp_function = *(const std::function<void()>*)(try_cpp_function);
    _try_cpp_function();
}

void EMSCRIPTEN_KEEPALIVE bungegum_emscripten_catch_from_js(int catch_cpp_function, const char* what)
{
    const std::function<void(const std::string&)>& _catch_cpp_function = *(const std::function<void(const std::string&)>*)(catch_cpp_function);
    std::string _what(what);
    _catch_cpp_function(_what);
}
}
#endif

namespace bungeegum {
namespace detail {

    namespace {

#if BUNGEEGUM_USE_BACKTRACE && TOOLCHAIN_PLATFORM_EMSCRIPTEN

        EM_JS(char*, emscripten_get_stacktrace, (), {
            return Module.stringToNewUTF8(stackTrace());
        });

        EM_JS(void, emscripten_protect, (int try_callback, int catch_callback), {
            try {
                Module.ccall('bungegum_emscripten_try_from_js',
                    'void',
                    ['number'],
                    [try_callback]);
            } catch (e) {
                Module.ccall('bungegum_emscripten_catch_from_js',
                    'void',
                    [ 'number', 'string' ],
                    [ catch_callback, e.stack ]);
            }
        });

#endif

        void emplace_traces(std::vector<backtraced_step>& tracing, const std::size_t tracing_offset, const std::size_t tracing_size)
        {
#if !BUNGEEGUM_USE_BACKTRACE
            (void)tracing;
            (void)tracing_offset;
            (void)tracing_size;
#elif TOOLCHAIN_PLATFORM_EMSCRIPTEN
            char* _cstack_trace = emscripten_get_stacktrace();
            std::string _stack_trace(_cstack_trace);
            free(_cstack_trace);
            std::cout << "trace = " << _stack_trace << std::endl;
#else
            backward::StackTrace _stack_trace;
            backward::TraceResolver _trace_resolver;
            std::size_t _offset = 3u + tracing_offset; // Escape backwardcpp calls + optionnaly defined count
            _stack_trace.load_here(tracing_size + _offset);
            _trace_resolver.load_stacktrace(_stack_trace);
            tracing.resize(tracing_size);
            for (std::size_t _i = 0; _i < tracing_size; _i++) {
                backward::ResolvedTrace _trace = _trace_resolver.resolve(_stack_trace[_i + _offset]);
                
                tracing[_i].address = _trace.addr;
                tracing[_i].file = _trace.source.filename;
                tracing[_i].function = _trace.source.function;
                tracing[_i].line = _trace.source.line;
                tracing[_i].column = _trace.source.col;
            }
#endif
        }

    }

    backtraced_exception::backtraced_exception(const std::string& tag, const std::string& what, const std::size_t tracing_offset, const std::size_t tracing_size)
        : _tag(tag)
        , _what(what)
        , _key(_tag + _what)
    {
        emplace_traces(tracing, tracing_offset, tracing_size);
    }

    backtraced_exception::backtraced_exception(const std::string& tag, const std::wstring& what, const std::size_t tracing_offset, const std::size_t tracing_size)
        : _tag(tag)
        , _what(narrow(what))
        , _key(_tag + _what)
    {
        emplace_traces(tracing, tracing_offset, tracing_size);
    }

    backtraced_exception::backtraced_exception(const std::string& tag, const std::exception& existing, const std::size_t tracing_offset, const std::size_t tracing_size)
        : _tag(tag)
        , _what(existing.what())
        , _key(_tag + _what)
    {
        emplace_traces(tracing, tracing_offset, tracing_size);
    }

    const char* backtraced_exception::tag() const noexcept
    {
        return _tag.c_str();
    }

    const char* backtraced_exception::what() const noexcept
    {
        return _what.c_str();
    }

#if BUNGEEGUM_USE_OVERLAY

    const char* backtraced_exception::key() const noexcept
    {
        return _key.c_str();
    }

#endif

    void protect(
        const std::function<void()>& try_callback,
        const std::function<void(backtraced_exception&&)>& catch_callback)
    {
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        int _try_callback = (int)(&try_callback);
        int _catch_callback = (int)(&catch_callback);
        emscripten_protect(_try_callback, _catch_callback);

#elif TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP
        try {
            try_callback();
        } catch (backtraced_exception&& _exception) {
            catch_callback(std::move(_exception));

        } catch (const std::exception& _exception) {
            catch_callback(detail::backtraced_exception("unknown", _exception.what(), 0u, 0u));
        } catch (const char* _what) {
            catch_callback(detail::backtraced_exception("unknown", _what, 0u, 0u));
        } catch (const std::string& _what) {
            catch_callback(detail::backtraced_exception("unknown", _what, 0u, 0u));
        } catch (const std::wstring& _what) {
            catch_callback(detail::backtraced_exception("unknown", _what, 0u, 0u));
        } catch (...) {
            catch_callback(detail::backtraced_exception("unknown", "unknown", 0u, 0u));
        }

        // __try {
        //
        // } __except (EXCEPTION_EXECUTE_HANDLER) {
        //     catch_callback(std::string("Unknown SEH exception occured."));
        // }
#else
        _impl();
#endif
    }

}
}