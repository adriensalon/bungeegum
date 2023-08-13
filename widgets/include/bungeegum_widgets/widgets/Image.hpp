#pragma once

#include <bungeegum/asset/texture.hpp>
#include <bungeegum_widgets/core/Alignment.hpp>
#include <bungeegum_widgets/core/Rect.hpp>
#include <bungeegum_widgets/core/BlendMode.hpp>
#include <bungeegum_widgets/core/BoxFit.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief
    struct Image {

		/// @brief Signature used by Image.errorBuilder to create a replacement widget to render
		/// instead of the image.
		using ImageErrorWidgetBuilder = std::function<runtime_widget(const std::string& error_message)>;

		/// @brief Signature used by Image.loadingBuilder to build a representation of the image's
		/// loading progress.
		/// @details This is useful for images that are incrementally loaded (e.g. over a local 
		/// file system or a network), and the application wishes to give the user an indication of
		/// when the image will be displayed. The child argument contains the default image widget 
		/// and is guaranteed to be non-null. Typically, this builder will wrap the child widget in 
		/// some way and return the wrapped widget. If this builder returns child directly, it will
		/// yield the same result as if Image.loadingBuilder was null.
		/// The loadingProgress argument contains the current progress towards loading the image. 
		/// This argument will be non-null while the image is loading, but it will be null in the
		/// following cases:
		/// 	- When the widget is first rendered before any bytes have been loaded.
		/// 	- When an image has been fully loaded and is available to be painted.
		using ImageLoadingBuilder = std::function<runtime_widget(const runtime_widget& child, const std::optional<ImageChunkEvent>& loadingProgress)>;

		Image& fromFile(const std::filesystem::path& value);

		Image& fromFutureTexture(const std::future<texture>&& value);

		Image& fromMemory(const std::vector<char>& value);

		Image& fromTexture(const texture& value);

		Image& alignment(const Alignment& value);

		Image& centerSlice(const std::optional<Rect>& value);

		Image& color(const std::optional<Color>& value);

		Image& colorBlendMode(const std::optional<BlendMode> value);

		Image& errorBuilder(const std::optional<ImageErrorWidgetBuilder>& value);

		Image& fit(const std::optional<BoxFit>& value);

		/// @brief Whether to continue showing the old image (true), or briefly show nothing
		/// (false), when the image provider changes. The default value is false. 
		Image& gaplessPlayback(const bool1 value);

		Image& loadingBuilder(const std::optional<ImageLoadingBuilder>& value);

		// etc.............


    private:
		Alignment _alignment = Alignment::center();
		std::optional<Rect> _centerSlice = std::nullopt;
		std::optional<Color> _color = std::nullopt;
		std::optional<BlendMode> _colorBlendMode = std::nullopt;
		std::optional<ImageErrorWidgetBuilder> _errorBuilder = std::nullopt;
		std::optional<BoxFit> _fit = std::nullopt;
		bool1 _gapless_playback = false;
		std::optional<ImageLoadingBuilder> _loadingBuilder = std::nullopt;
    };

}
}