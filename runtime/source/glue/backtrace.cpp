#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/string.hpp>

#if BUNGEEGUM_USE_BACKTRACE
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#else
#include "backward.hpp" // We can't use angle brackets because this lib is compiled with add_library(... OBJECT) lmao
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
                    ['number', 'string'],
                    [catch_callback, e.stack]);
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
            std::size_t _offset = 4u + tracing_offset; // Escape backwardcpp calls + optionnaly defined count
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
                // tracing[_i].primary = backtraced_source { _trace.source.filename, _trace.source.function, _trace.source.line, _trace.source.col };
                // for (backward::ResolvedTrace::SourceLoc& _inliner : _trace.inliners) {
                //     tracing[_i].inliners.emplace_back(backtraced_source { _inliner.filename, _inliner.function, _inliner.line, _inliner.col });
                // }
            }
#endif
        }

    }

    void protect(
		const std::function<void()>& try_callback,
		const std::function<void(const std::string&)>& catch_callback)
    {
#if !BUNGEEGUM_USE_BACKTRACE
		try_callback();
		(void)catch_callback;
#elif TOOLCHAIN_PLATFORM_EMSCRIPTEN
		int _try_ccallback = (int)(&try_callback);
		int _catch_ccallback = (int)(&catch_callback);
		emscripten_protect(_try_ccallback, _catch_ccallback);
#else
		std::function<void()> _impl = [&try_callback, &catch_callback] () {
			try {
				try_callback();
			} catch (const std::exception& _exception) {
				catch_callback(std::string(_exception.what()));
			} catch (const char* _what) {
				catch_callback(std::string(_what));
			} catch (const std::string& _what) {
				catch_callback(_what);
			} catch (const std::wstring& _what) {
				catch_callback(narrow(_what));
			} catch (...) {
				catch_callback(std::string("Unknown exception occured."));
			}
		};		
#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP)
        // __try {
			_impl();
        // } __except (EXCEPTION_EXECUTE_HANDLER) {
        //     catch_callback(std::string("Unknown SEH exception occured."));
        // }
#else
		_impl();
#endif
#endif
    }

    backtraced_exception::backtraced_exception(const std::string& what, const std::size_t tracing_offset, const std::size_t tracing_size)
    {
        _what = what;
        emplace_traces(tracing, tracing_offset, tracing_size);
    }

    backtraced_exception::backtraced_exception(const std::wstring& what, const std::size_t tracing_offset, const std::size_t tracing_size)
    {
        _what = narrow(what);
        emplace_traces(tracing, tracing_offset, tracing_size);
    }

    backtraced_exception::backtraced_exception(const std::exception& existing, const std::size_t tracing_offset, const std::size_t tracing_size)
    {
        _what = existing.what();
        emplace_traces(tracing, tracing_offset, tracing_size);
    }

    const char* backtraced_exception::what() const noexcept
    {
        return _what.c_str();
    }

}
}