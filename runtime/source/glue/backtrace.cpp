#include <bungeegum/glue/backtrace.fwd>
#include <bungeegum/glue/console.fwd>

#if BUNGEEGUM_USE_BACKTRACE
#include "backward.hpp" // We can't use angle brackets because this lib is compiled with add_library(... OBJECT) lmao
#endif

namespace bungeegum {
namespace detail {

    void emplace_traces(std::vector<backtraced_result>& tracing, const std::size_t tracing_offset, const std::size_t tracing_size)
    {
#if BUNGEEGUM_USE_BACKTRACE

        backward::StackTrace _stack_trace;
        backward::TraceResolver _trace_resolver;
        std::size_t _offset = 4u + tracing_offset; // Escape backwardcpp calls + optionnaly defined count
        _stack_trace.load_here(tracing_size + _offset);
        _trace_resolver.load_stacktrace(_stack_trace);
        tracing.resize(tracing_size);
        for (std::size_t _i = 0; _i < tracing_size; _i++) {
            backward::ResolvedTrace _trace = _trace_resolver.resolve(_stack_trace[_i + _offset]);
            tracing[_i].address = _trace.addr;
            tracing[_i].primary = backtraced_source { _trace.source.filename, _trace.source.function, _trace.source.line, _trace.source.col };
            for (backward::ResolvedTrace::SourceLoc& _inliner : _trace.inliners) {
                tracing[_i].inliners.emplace_back(backtraced_source { _inliner.filename, _inliner.function, _inliner.line, _inliner.col });
            }
        }
#else
        (void)tracing;
        (void)tracing_offset;
        (void)tracing_size;
#endif
    }

    void protected_emplace_traces(std::vector<backtraced_result>& tracing, const std::size_t tracing_offset, const std::size_t tracing_size)
    {
#if TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP
        __try {
            emplace_traces(tracing, tracing_offset, tracing_size);
        } __except (EXCEPTION_EXECUTE_HANDLER) {
        }
#else
        try {
            emplace_traces(tracing, tracing_offset, tracing_size);
        } catch (...) {
        }
#endif
    }

    backtraced_exception::backtraced_exception(const std::string& what, const std::size_t tracing_offset, const std::size_t tracing_size)
    {
        _what = what;
        protected_emplace_traces(tracing, tracing_offset, tracing_size);
    }

    backtraced_exception::backtraced_exception(const std::wstring& what, const std::size_t tracing_offset, const std::size_t tracing_size)
    {
        _what = narrow(what);
        protected_emplace_traces(tracing, tracing_offset, tracing_size);
    }

    backtraced_exception::backtraced_exception(const std::exception& existing, const std::size_t tracing_offset, const std::size_t tracing_size)
    {
        _what = existing.what();
        protected_emplace_traces(tracing, tracing_offset, tracing_size);
    }

    const char* backtraced_exception::what() const noexcept
    {
        return _what.c_str();
    }
	
}
}