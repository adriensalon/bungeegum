#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief An immutable notification of image bytes that have been incrementally loaded.
	/// @details Chunk events represent progress notifications while an image is being loaded 
	/// (e.g. from disk or over the network).
    struct ImageChunkEvent {

		/// @brief 
		const int cumulativeBytesLoaded;

		/// @brief 
		const int expectedTotalBytes;
    };

}
}