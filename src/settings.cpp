#include "settings.h"
#include "blurconfig.h"

namespace KWin
{

void BlurSettings::read()
{
    BlurConfig::self()->read();

    general.blurStrength = BlurConfig::blurStrength() - 1;
    general.noiseStrength = BlurConfig::noiseStrength();
    general.windowOpacityAffectsBlur = BlurConfig::transparentBlur();
    general.brightness = BlurConfig::brightness();
    general.saturation = BlurConfig::saturation();
    general.contrast = BlurConfig::contrast();
    general.blurOpacity = BlurConfig::blurOpacity();

    inactive.enable = BlurConfig::inactiveBlur();
    inactive.blurStrengthMultiplier = BlurConfig::inactiveBlurStrengthMultiplier();
    inactive.brightnessMultiplier = BlurConfig::inactiveBrightnessMultiplier();
    inactive.saturationMultiplier = BlurConfig::inactiveSaturationMultiplier();
    inactive.contrastMultiplier = BlurConfig::inactiveContrastMultiplier();
    inactive.opacityMultiplier = BlurConfig::inactiveOpacityMultiplier();
    inactive.animationDuration = BlurConfig::inactiveAnimationDuration();
    inactive.windowTranslucency = BlurConfig::inactiveWindowTranslucency();
    inactive.windowOpacity = BlurConfig::inactiveWindowOpacity();
    inactive.moveResizeOpacity = BlurConfig::inactiveMoveResizeOpacity();
    inactive.excludedClasses.clear();
    for (const auto &line : BlurConfig::inactiveExcludedClasses().split("\n", Qt::SkipEmptyParts)) {
        inactive.excludedClasses << line.trimmed();
    }

    forceBlur.windowClasses.clear();
    const auto blank = QStringLiteral("blank");
    for (const auto &line : BlurConfig::windowClasses().split("\n", Qt::SkipEmptyParts)) {
        QString unescaped = "";
        bool consumed = false;
        for (qsizetype i = 0; i < line.size(); i++) {
            const auto character = line[i];
            if (character == QChar('$') && !consumed) {
                consumed = true;
                continue;
            }
            if (consumed) {
                const qsizetype skips = blank.size();
                if (line.mid(i, skips) == blank) {
                    consumed = false;
                    i += skips - 1;
                    continue;
                }
            }
            consumed = false;
            unescaped += character;
        }
        if (consumed) {
            unescaped += QChar('$');
        }
        forceBlur.windowClasses << unescaped;
    }
    forceBlur.windowClassMatchingMode = BlurConfig::blurMatching() ? WindowClassMatchingMode::Whitelist : WindowClassMatchingMode::Blacklist;
    forceBlur.blurDecorations = BlurConfig::blurDecorations();
    forceBlur.blurMenus = BlurConfig::blurMenus();
    forceBlur.blurDocks = BlurConfig::blurDocks();
    forceBlur.blurPlasmaWidgets = BlurConfig::blurPlasmaWidgets();

    roundedCorners.windowTopRadius = BlurConfig::topCornerRadius();
    roundedCorners.windowBottomRadius = BlurConfig::bottomCornerRadius();
    roundedCorners.menuRadius = BlurConfig::menuCornerRadius();
    roundedCorners.dockRadius = BlurConfig::dockCornerRadius();
    roundedCorners.antialiasing = BlurConfig::roundedCornersAntialiasing();
    roundedCorners.roundMaximized = BlurConfig::roundCornersOfMaximizedWindows();

    staticBlur.enable = BlurConfig::fakeBlur();
    staticBlur.disableWhenWindowBehind = BlurConfig::fakeBlurDisableWhenWindowBehind();
    staticBlur.customImage = QImage(BlurConfig::fakeBlurImage());
    if (BlurConfig::fakeBlurImageSourceDesktopWallpaper()) {
        staticBlur.imageSource = StaticBlurImageSource::DesktopWallpaper;
    } else {
        staticBlur.imageSource = StaticBlurImageSource::Custom;
    }
    staticBlur.blurCustomImage = BlurConfig::fakeBlurCustomImageBlur();

    refraction.edgeSizePixels = BlurConfig::refractionEdgeSize() * 10;
    refraction.refractionStrength = BlurConfig::refractionStrength() / 20.0;
    refraction.refractionNormalPow = BlurConfig::refractionNormalPow() / 2.0;
    refraction.refractionRGBFringing = BlurConfig::refractionRGBFringing() / 20.0;
    refraction.refractionTextureRepeatMode = BlurConfig::refractionTextureRepeatMode();
}

}