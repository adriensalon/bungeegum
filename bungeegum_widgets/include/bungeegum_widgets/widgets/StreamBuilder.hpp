#pragma once

#include <streambuf>

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief Widget that builds itself based on the latest overflow of a std::ostream.
    /// @details
    struct StreamBuilder {

        /// @brief The build strategy currently used.
        /// @details Initial data can be used to fire the build callback only once in the lifetime
        /// of this StreamBuilder, the first time initialData() and builder() are defined.
        StreamBuilder& builder(const std::function<runtime_widget(const std::string&)>& value);

        /// @brief The data that will be used to build until the stream has overflown once.
        /// @details Initial data can be used to fire the build callback only once in the lifetime
        /// of this StreamBuilder, the first time initialData() and builder() are defined.
        StreamBuilder& initialData(const std::string& value);

        /// @brief The std::ostream to which this builder is currently connected.
        /// @details The original streambuf will be restored when this StreamBuilder is destroyed.
        StreamBuilder& stream(std::ostream& stream, const uint1 buffer_size = 256u);

        virtual ~StreamBuilder();

    private:
        friend struct access;
        void processInitialData();
        void resolve(resolve_command& command);

        struct StreamBuilderBuffer : public std::streambuf {
            std::function<void(const std::string&)> flushCallback = nullptr;

            StreamBuilderBuffer(std::ostream& sink, const uint1 buffer_size);
            StreamBuilderBuffer(const StreamBuilderBuffer& other) = default;
            StreamBuilderBuffer& operator=(const StreamBuilderBuffer& other) = default;
            StreamBuilderBuffer(StreamBuilderBuffer&& other) = default;
            StreamBuilderBuffer& operator=(StreamBuilderBuffer&& other) = default;

        private:
            std::reference_wrapper<std::ostream> _sink;
            std::vector<char_type> _buffer = {};

            bool1 triggerAndFlush();
            int_type overflow(int_type character) override;
            int1 sync() override;
        };

        std::optional<runtime_widget> _childWidget = std::nullopt;
        std::optional<std::string> _initialData = std::nullopt;
        std::function<runtime_widget(const std::string&)> _flushCallback = nullptr;
        std::optional<StreamBuilderBuffer> _customBuffer = std::nullopt;
        std::function<void()> _restoreFormerBuffer = nullptr;
        bool1 _initialDataBuildDone = false;
    };

    /// @brief Widget that builds itself based on the latest overflow of a std:w:ostream.
    /// @details
    struct WideStreamBuilder {

        /// @brief The build strategy currently used.
        /// @details Initial data can be used to fire the build callback only once in the lifetime
        /// of this WideStreamBuilder, the first time initialData() and builder() are defined.
        WideStreamBuilder& builder(const std::function<runtime_widget(const std::wstring&)>& value);

        /// @brief The data that will be used to build until the stream has overflown once.
        /// @details Initial data can be used to fire the build callback only once in the lifetime
        /// of this WideStreamBuilder, the first time initialData() and builder() are defined.
        WideStreamBuilder& initialData(const std::wstring& value);

        /// @brief The std::wostream to which this builder is currently connected.
        /// @details The original streambuf will be restored when this WideStreamBuilder is
        /// destroyed.
        WideStreamBuilder& stream(std::wostream& stream, const uint1 buffer_size = 256u);

        virtual ~WideStreamBuilder();

    private:
        friend struct access;
        void processInitialData();
        void resolve(resolve_command& command);

        struct WideStreamBuilderBuffer : public std::wstreambuf {
            std::function<void(const std::wstring&)> flushCallback = nullptr;

            WideStreamBuilderBuffer(std::wostream& sink, const uint1 buffer_size);
            WideStreamBuilderBuffer(const WideStreamBuilderBuffer& other) = default;
            WideStreamBuilderBuffer& operator=(const WideStreamBuilderBuffer& other) = default;
            WideStreamBuilderBuffer(WideStreamBuilderBuffer&& other) = default;
            WideStreamBuilderBuffer& operator=(WideStreamBuilderBuffer&& other) = default;

        private:
            std::reference_wrapper<std::wostream> _sink;
            std::vector<char_type> _buffer = {};

            bool1 triggerAndFlush();
            int_type overflow(int_type character) override;
            int1 sync() override;
        };

        std::optional<runtime_widget> _childWidget = std::nullopt;
        std::optional<std::wstring> _initialData = std::nullopt;
        std::function<runtime_widget(const std::wstring&)> _flushCallback = nullptr;
        std::optional<WideStreamBuilderBuffer> _customBuffer = std::nullopt;
        std::function<void()> _restoreFormerBuffer = nullptr;
        bool1 _initialDataBuildDone = false;
    };
}
}