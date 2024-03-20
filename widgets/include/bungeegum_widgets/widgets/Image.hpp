#pragma once

#include <bungeegum_widgets/core/Alignment.hpp>
#include <bungeegum_widgets/core/Color.hpp>
#include <bungeegum_widgets/core/Rect.hpp>
#include <bungeegum_widgets/core/BlendMode.hpp>
#include <bungeegum_widgets/core/BoxFit.hpp>
#include <bungeegum_widgets/core/ImageChunkEvent.hpp>
#include <bungeegum_widgets/core/ImageRepeat.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief
    struct Image {

		/// @brief Signature used by Image.errorBuilder to create a replacement widget to render
		/// instead of the image.
		using ImageErrorWidgetBuilder = std::function<widget_id(const std::string& error_message)>;

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
		using ImageLoadingBuilder = std::function<widget_id(const widget_id& child, const std::optional<ImageChunkEvent>& loadingProgress)>;

		Image& fromFile(const std::filesystem::path& value);

		Image& fromFutureTexture(std::future<texture>&& value);

		Image& fromMemory(const std::vector<unsigned char>& pixels, const float2 size);

		Image& fromTexture(texture&& value);

		/// @brief How to align the image within its bounds.
		/// @details The alignment aligns the given position in the image to the given position in
		/// the layout bounds. For example, an Alignment alignment of (-1.0, -1.0) aligns the image
		/// to the top-left corner of its layout bounds, while an Alignment alignment of (1.0, 1.0)
		/// aligns the bottom right of the image with the bottom right corner of its layout bounds. 
		/// Similarly, an alignment of (0.0, 1.0) aligns the bottom middle of the image with the 
		/// middle of the bottom edge of its layout bounds.
		/// To display a subpart of an image, consider using a CustomPainter and 
		/// Canvas.drawImageRect.
		/// If the alignment is TextDirection-dependent (i.e. if it is a AlignmentDirectional), 
		/// then an ambient Directionality widget must be in scope.
		/// Defaults to Alignment.center.
		Image& alignment(const Alignment& value);

		Image& centerSlice(const std::optional<Rect>& value);

		Image& color(const std::optional<Color>& value);

		Image& colorBlendMode(const std::optional<BlendMode> value);

		Image& errorBuilder(const std::optional<ImageErrorWidgetBuilder>& value);

		/// @brief How to inscribe the image into the space allocated during layout.
		/// @details The default varies based on the other fields.
		Image& fit(const std::optional<BoxFit>& value);

		// frame builder (go gif!)

		/// @brief Whether to continue showing the old image (true), or briefly show nothing
		/// (false), when the image provider changes. The default value is false. 
		/// @details Having the default value of gaplessPlayback be false helps prevent situations 
		/// where stale or misleading information might be presented. Consider the following case:
		/// We have constructed a 'Person' widget that displays an avatar Image of the currently 
		/// loaded person along with their name. We could request for a new person to be loaded 
		/// into the widget at any time. Suppose we have a person currently loaded and the widget 
		/// loads a new person. What happens if the Image fails to load?
		/// - gaplessPlayback = false: The new person's name is coupled with a blank image.
		/// - gaplessPlayback = true: The widget displays the avatar of the previous person and the 
		/// name of the newly loaded person.
		/// This is why the default value is false. Most of the time, when you change the image 
		/// provider you're not just changing the image, you're removing the old widget and adding
		/// a new one and not expecting them to have any relationship. With gaplessPlayback on you 
		/// might accidentally break this expectation and re-use the old widget.
		Image& gaplessPlayback(const bool value);

		/// @brief If non-null, require the image to have this height (in logical pixels).
		/// @details If null, the image will pick a size that best preserves its intrinsic aspect
		/// ratio. Consider using fit to adapt the image's rendering to fit the given width and 
		/// height if the exact image dimensions are not known in advance.
		Image& height(const float value);

		// image provider

		Image& loadingBuilder(const std::optional<ImageLoadingBuilder>& value);

		Image& matchTextDirection(const bool value);

		// Image& opacity(const animation<float> value)

		Image& repeat(const ImageRepeat value);

		Image& width(const float value);


    private:
        friend struct access;
        void resolve(resolve_command& command);
        void draw(draw_command& command);

		Alignment _alignment = Alignment::center();
		std::optional<Rect> _centerSlice = std::nullopt;
		std::optional<Color> _color = std::nullopt;
		std::optional<BlendMode> _colorBlendMode = std::nullopt;
		std::optional<shader> _colorShader = std::nullopt;
		std::optional<ImageErrorWidgetBuilder> _errorBuilder = std::nullopt;
		std::optional<BoxFit> _fit = std::nullopt;
		std::optional<std::future<texture>> _futureTexture = std::nullopt;
		bool _gaplessPlayback = false;
		std::optional<ImageLoadingBuilder> _loadingBuilder = std::nullopt;
		ImageRepeat _repeat = ImageRepeat::noRepeat;
		std::optional<texture> _texture = std::nullopt;

		SERIALIZE_FIELDS(
			_alignment,
			_centerSlice,
			_color,
			_colorBlendMode,
			_colorShader,
			_errorBuilder,
			_fit,
			_futureTexture,
			_gaplessPlayback,
			_loadingBuilder,
			_repeat,
			_texture)
    };

}
}