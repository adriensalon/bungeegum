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

#include <iostream>
#include <thread>

#if TOOLCHAIN_BUILD_DEBUG && TOOLCHAIN_PLATFORM_EMSCRIPTEN

extern "C" {
void EMSCRIPTEN_KEEPALIVE bungegum_emscripten_try_from_js(int try_cpp_function)
{
    const std::function<void()>& _try_cpp_function = *(const std::function<void()>*)(try_cpp_function);
    _try_cpp_function();
}

void EMSCRIPTEN_KEEPALIVE bungegum_emscripten_catch_from_js(int catch_cpp_function, const char* what)
{
    const std::function<void(bungeegum::detail::backtraced_exception&&)>& _catch_cpp_function = *(const std::function<void(bungeegum::detail::backtraced_exception&&)>*)(catch_cpp_function);
    std::vector<std::string> _separated = bungeegum::detail::split(what, '!');
    std::string _tag = _separated[0];
    // filter 
    std::string _what = _separated[1];
    std::string _trace = _separated[2];
    bungeegum::detail::backtraced_exception _exception(_tag, _what, _trace);
    _catch_cpp_function(std::move(_exception));
}
}

#endif

namespace bungeegum {
namespace detail {

    namespace {

#if BUNGEEGUM_USE_BACKTRACE && TOOLCHAIN_PLATFORM_EMSCRIPTEN

        EM_JS(char*, emscripten_get_stacktrace_str_impl, (), {
            return Module.stringToNewUTF8(stackTrace());
        });
        
        std::string emscripten_get_stracktrace_str()
        {
            char* _cstack_trace = emscripten_get_stacktrace_str_impl();
            std::string _stack_trace(_cstack_trace);
            free(_cstack_trace);
            return _stack_trace;
        }

        std::string emscripten_get_tracing(
            const std::size_t tracing_offset, 
            const std::size_t tracing_size)
        {
            std::string _js_tracing = emscripten_get_stracktrace_str();
            // todo
            return _js_tracing;
        }

        void emscripten_emplace_tracing(
            std::vector<backtraced_step>& tracing, 
            const std::string& js_tracing)
        {
            // todo
        }
        
#endif

#if BUNGEEGUM_USE_BACKTRACE && TOOLCHAIN_PLATFORM_DESKTOP
        
        void desktop_emplace_tracing(
            std::vector<backtraced_step>& tracing, 
            const std::size_t tracing_offset, 
            const std::size_t tracing_size)
        {
            backward::StackTrace _stack_trace;
            backward::TraceResolver _trace_resolver;
            std::size_t _offset = 4u + tracing_offset; // Escape backwardcpp calls + optionnaly defined count
            _stack_trace.load_here(tracing_size + _offset);
            _trace_resolver.load_stacktrace(_stack_trace);
            tracing.resize(tracing_size); // plutot go stop on bungeegum:: boundary
            for (std::size_t _i = 0; _i < tracing_size; _i++) {
                backward::ResolvedTrace _trace = _trace_resolver.resolve(_stack_trace[_i + _offset]);
                tracing[_i].address = _trace.addr;
                tracing[_i].file = _trace.source.filename;
                tracing[_i].function = _trace.source.function;
                tracing[_i].line = _trace.source.line;
                tracing[_i].column = _trace.source.col;
            }
        }

#endif

#if TOOLCHAIN_BUILD_DEBUG && TOOLCHAIN_PLATFORM_EMSCRIPTEN
        
        EM_JS(void, emscripten_protect_impl, (int try_callback, int catch_callback), {
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

        void emscripten_protect(
            const std::function<void()>& try_callback,
            const std::function<void(backtraced_exception&&)>& catch_callback)
        {
            int _try_callback = (int)(&try_callback);
            int _catch_callback = (int)(&catch_callback);
            emscripten_protect_impl(_try_callback, _catch_callback);
        }

#endif

#if TOOLCHAIN_BUILD_DEBUG && !TOOLCHAIN_PLATFORM_EMSCRIPTEN
        
        void native_protect(
            const std::function<void()>& try_callback,
            const std::function<void(backtraced_exception&&)>& catch_callback)
        {
            try {
                try_callback();
            } catch (backtraced_exception&& _exception) {
                catch_callback(std::move(_exception));
            } catch (const std::exception& _exception) {
                catch_callback(detail::backtraced_exception("C++", _exception.what(), 0u, 0u));
            } catch (const char* _what) {
                catch_callback(detail::backtraced_exception("C++", _what, 0u, 0u));
            } catch (const std::string& _what) {
                catch_callback(detail::backtraced_exception("C++", _what, 0u, 0u));
            } catch (const std::wstring& _what) {
                catch_callback(detail::backtraced_exception("C++", _what, 0u, 0u));
            } catch (...) {
                catch_callback(detail::backtraced_exception("C++", "Unknown error.", 0u, 0u));
            }
        }

#endif

#if TOOLCHAIN_BUILD_DEBUG && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP)

        thread_local static std::optional<backtraced_exception> _global_win32_exception = std::nullopt;

        void win32_protect_impl(
            const std::function<void()>& try_callback,
            const std::function<void(const char*, const char*)>& catch_callback)
        {
            __try {
                try_callback();
            } __except (EXCEPTION_EXECUTE_HANDLER) {
                switch (GetExceptionCode()) {
                case EXCEPTION_ACCESS_VIOLATION:
                    catch_callback("Win32", "Access violation SEH exception.");
                    break;
                case EXCEPTION_DATATYPE_MISALIGNMENT:
                    catch_callback("Win32", "Data misalignment SEH exception.");
                    break;
                case EXCEPTION_INT_DIVIDE_BY_ZERO:
                    catch_callback("Win32", "Divide by zero SEH exception.");
                    break;
                case EXCEPTION_ILLEGAL_INSTRUCTION:
                    catch_callback("Win32", "Illegal instruction SEH exception.");
                    break;
                case EXCEPTION_STACK_OVERFLOW:
                    catch_callback("Win32", "Stack overflow SEH exception.");
                    break;
                }
            }
        }

        void win32_protect(
            const std::function<void()>& try_callback,
            const std::function<void(backtraced_exception&&)>& catch_callback)
        {
            win32_protect_impl([try_callback, catch_callback] () {
                native_protect(try_callback, catch_callback);
            }, [catch_callback](const char* tag, const char* what) {
                catch_callback(detail::backtraced_exception(tag, what, 0u, 0u));
            });
        }

#endif




        std::string get_key(const std::string& tag, const std::string& what)
        {
            return tag + what;
        }

        std::string get_what_and_emplace_tracing(
            const std::string& tag, 
            const std::string& what, 
            std::vector<backtraced_step>& tracing, 
            const std::size_t tracing_offset, 
            const std::size_t tracing_size, 
            const std::optional<std::string>& js_tracing = std::nullopt)
        {
#if BUNGEEGUM_USE_BACKTRACE && TOOLCHAIN_PLATFORM_EMSCRIPTEN
            if (!js_tracing.has_value()) {
                std::string _trace = emscripten_get_tracing(tracing_offset, tracing_size);
                return tag + "!" + what + "!" + _trace;
            } else {
                emscripten_emplace_tracing(tracing, js_tracing.value());
                return what;
            }
#elif BUNGEEGUM_USE_BACKTRACE && TOOLCHAIN_PLATFORM_DESKTOP
            (void)js_tracing;
            desktop_emplace_tracing(tracing, tracing_offset, tracing_size);
            return what;
#else 
            (void)tag;
            (void)tracing;
            (void)tracing_offset;
            (void)tracing_size;
            (void)js_tracing;
            return what;
#endif
        }

    }

    backtraced_exception::backtraced_exception(const std::string& tag, const std::string& what, const std::size_t tracing_offset, const std::size_t tracing_size)
        : _tag(tag)
        , _what(get_what_and_emplace_tracing(tag, what, tracing, tracing_offset, tracing_size))
        , _key(get_key(tag, what))
    {
    }

    backtraced_exception::backtraced_exception(const std::string& tag, const std::wstring& what, const std::size_t tracing_offset, const std::size_t tracing_size)
        : _tag(tag)
        , _what(get_what_and_emplace_tracing(tag, narrow(what), tracing, tracing_offset, tracing_size))
        , _key(get_key(tag, narrow(what)))
    {
    }

    backtraced_exception::backtraced_exception(const std::string& tag, const std::exception& existing, const std::size_t tracing_offset, const std::size_t tracing_size)
        : _tag(tag)
        , _what(get_what_and_emplace_tracing(tag, existing.what(), tracing, tracing_offset, tracing_size))
        , _key(get_key(tag, existing.what()))
    {
    }

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN

    backtraced_exception::backtraced_exception(const std::string& tag, const std::string& what, const std::string& js_tracing)
        : _tag(tag)
        , _what(get_what_and_emplace_tracing(tag, what, tracing, 0, 0, js_tracing))
        , _key(get_key(tag, what))
    {
    }

#endif

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
#if !TOOLCHAIN_BUILD_DEBUG
        std::cout << "lol\n";
        try_callback();
        (void)catch_callback;
#elif TOOLCHAIN_PLATFORM_EMSCRIPTEN
        emscripten_protect(try_callback, catch_callback);
#elif TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP
        win32_protect(try_callback, catch_callback);
#else
        native_protect(try_callback, catch_callback);
#endif
    }

}
}