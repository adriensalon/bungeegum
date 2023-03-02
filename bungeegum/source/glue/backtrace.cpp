//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <backward.hpp>

#include <bungeegum/glue/backtrace.hpp>

namespace bungeegum {
namespace detail {

    namespace {

        void emplace_traces(std::vector<backtraced_result>& tracing, const unsigned int tracing_size)
        {
            backward::StackTrace _stack_trace;
            backward::TraceResolver _trace_resolver;
            unsigned int _offset = 3U; // escape backwardcpp calls
            _stack_trace.load_here(tracing_size + _offset);
            _trace_resolver.load_stacktrace(_stack_trace);
            tracing.resize(tracing_size);
            for (unsigned int _i = 0; _i < tracing_size; _i++) {
                backward::ResolvedTrace _trace = _trace_resolver.resolve(_stack_trace[_i + _offset]);
                tracing[_i].address = _trace.addr;
                tracing[_i].primary = backtraced_source { _trace.source.filename, _trace.source.function, _trace.source.line, _trace.source.col };
                for (auto& _inliner : _trace.inliners)
                    tracing[_i].inliners.emplace_back(backtraced_source { _inliner.filename, _inliner.function, _inliner.line, _inliner.col });
            }
        }
    }

    backtraced_exception::backtraced_exception(const std::string& what, const unsigned int tracing_size)
    {
        _what = what;
        emplace_traces(tracing, tracing_size);
    }

    backtraced_exception::backtraced_exception(const std::exception& existing)
    {
        _what = std::string(existing.what());
    }

    backtraced_exception::backtraced_exception(backtraced_exception&& other)
    {
        *this = std::move(other);
    }

    backtraced_exception& backtraced_exception::operator=(backtraced_exception&& other)
    {
        this->tracing = std::move(other.tracing);
        this->_what = std::move(other._what);
        return *this;
    }

    const char* backtraced_exception::what() const
    {
        return _what.c_str();
    }
}
}