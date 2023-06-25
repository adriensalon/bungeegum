#pragma once

#include <streambuf>

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    namespace detail {

        struct event_buffer : public std::streambuf {

            event_buffer(std::ostream& sink, std::size_t buffer_size = 256);

            event_buffer(const event_buffer& other) = default;
            event_buffer& operator=(const event_buffer& other) = default;
            event_buffer(event_buffer&& other) = default;
            event_buffer& operator=(event_buffer&& other) = default;

        private:
            std::reference_wrapper<std::ostream> _sink;
            std::vector<char_type> _buffer;

            bool trigger_and_flush();
            int_type overflow(int_type ch) override;
            int_type sync() override;
        };

    }

    struct StreamBuilder {

        /// @brief The build strategy currently used.
        StreamBuilder& builder(const std::function<runtime_widget(const std::string&)>& value);

        /// @brief The data that will be used to build until the stream has completed.
        StreamBuilder& initialData(const std::string& value);

        /// @brief The asynchronous computation to which this builder is currently connected.
        StreamBuilder& stream(std::ostream& value);

        StreamBuilder(const StreamBuilder& other) = delete;
        StreamBuilder& operator=(const StreamBuilder& other) = delete;
        StreamBuilder(StreamBuilder&& other) = default;
        StreamBuilder& operator=(StreamBuilder&& other) = default;

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<runtime_widget> _childWidget = std::nullopt;
        std::optional<std::function<void(const std::string&)>> _builderFunction = std::nullopt;
        std::optional<std::string> _initialData = std::nullopt;
        std::optional<detail::event_buffer> _customBuffer = std::nullopt;
        std::streambuf* _formerStreambuf = nullptr;
    };

    // struct WideStreamBuilder {

    //     /// @brief The build strategy currently used.
    //     WideStreamBuilder& builder(const std::function<runtime_widget(const std::wstring&)>& value);

    //     /// @brief The data that will be used to build until the stream has completed.
    //     WideStreamBuilder& initialData(const std::wstring& value);

    //     /// @brief The asynchronous computation to which this builder is currently connected.
    //     WideStreamBuilder& stream(std::wostream& value);

    // private:
    //     friend struct access;
    //     void resolve(resolve_command& command);

    //     std::optional<runtime_widget> _childWidget = std::nullopt;
    //     std::optional<std::function<void(const std::wstring&)>> _builderFunction = std::nullopt;
    //     std::optional<std::wstring> _initialData = std::nullopt;
    //     std::streambuf* _formerStreambuf = nullptr;
    // };

}
}