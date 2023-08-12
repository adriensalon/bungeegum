#pragma once

namespace bungeegum {
namespace widgets {

    constexpr int kBackMouseButton = 0x08;
    constexpr int kForwardMouseButton = 0x10;
    constexpr int kPrimaryButton = 0x01;
    constexpr int kSecondaryButton = 0x02;
    constexpr int kTertiaryButton = 0x04;

    constexpr int kMiddleMouseButton = kTertiaryButton;
    constexpr int kPrimaryMouseButton = kPrimaryButton;
    // constexpr int kPrimaryStylusButton = kSecondaryButton;
    constexpr int kSecondaryMouseButton = kSecondaryButton;
    // constexpr int kSecondaryStylusButton = kTertiaryButton;
    // constexpr int kStylusContact = kPrimaryButton;
    constexpr int kTouchContact = kPrimaryButton;

    /// @brief Returns whether buttons contains one and only one button.
    bool isSingleButton(int buttons);

    /// @brief The bit of PointerEvent.buttons that corresponds to the nth mouse button.
    int nthMouseButton(int number);

    /// @brief The bit of PointerEvent.buttons that corresponds to the nth stylus button.
    int nthStylusButton(int number);

    /// @brief Returns the button of buttons with the smallest integer.
    int smallestButton(int buttons);

}

}