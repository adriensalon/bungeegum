#pragma once

#include <streambuf>

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    namespace detail {

        struct StreamBuilderBuffer : public std::streambuf {
            std::function<void(const std::string&)> flushCallback = nullptr;

            StreamBuilderBuffer(std::ostream& sink, std::size_t buffer_size);
            StreamBuilderBuffer(const StreamBuilderBuffer& other) = default;
            StreamBuilderBuffer& operator=(const StreamBuilderBuffer& other) = default;
            StreamBuilderBuffer(StreamBuilderBuffer&& other) = default;
            StreamBuilderBuffer& operator=(StreamBuilderBuffer&& other) = default;

        private:
            std::reference_wrapper<std::ostream> _sink;
            std::vector<char_type> _buffer = {};

            bool1 triggerAndFlush();
            int_type overflow(int_type character) override;
            int_type sync() override;
        };
    }

    struct StreamBuilder {

        /// @brief The build strategy currently used.
        StreamBuilder& builder(const std::function<runtime_widget(const std::string&)>& value);

        /// @brief The data that will be used to build until the stream has completed.
        StreamBuilder& initialData(const std::string& value);

        /// @brief The asynchronous computation to which this builder is currently connected.
        StreamBuilder& stream(std::ostream& value, const uint1 buffer_size = 256u);

    private:
        friend struct access;
        void processInitialData();
        void resolve(resolve_command& command);

        std::optional<runtime_widget> _childWidget = std::nullopt;
        std::optional<std::string> _initialData = std::nullopt;
        std::optional<std::function<runtime_widget(const std::string&)>> _flushCallback = std::nullopt;
        std::optional<detail::StreamBuilderBuffer> _customBuffer = std::nullopt;
        std::streambuf* _formerStreambuf = nullptr;
        bool1 _initialDataBuildDone = false;
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