// SPDX-FileCopyrightText: 2023 Carl Schwan <carl@carlschwan.eu>
// SPDX-License-Identifier: LGPL-2.0-or-later

#include "colorutils.h"

#include <QGuiApplication>
#include <QPalette>

QColor ColorUtils::tintWithAlpha(const QColor &targetColor, const QColor &tintColor, double alpha)
{
    qreal tintAlpha = tintColor.alphaF() * alpha;
    qreal inverseAlpha = 1.0 - tintAlpha;

    if (qFuzzyCompare(tintAlpha, 1.0)) {
        return tintColor;
    } else if (qFuzzyIsNull(tintAlpha)) {
        return targetColor;
    }

    return QColor::fromRgbF(tintColor.redF() * tintAlpha + targetColor.redF() * inverseAlpha,
                            tintColor.greenF() * tintAlpha + targetColor.greenF() * inverseAlpha,
                            tintColor.blueF() * tintAlpha + targetColor.blueF() * inverseAlpha,
                            tintAlpha + inverseAlpha * targetColor.alphaF());
}

QColor ColorUtils::contrastColor(const QColor &color)
{
    const auto lightness = static_cast<QGuiApplication *>(QGuiApplication::instance())->palette().color(QPalette::Active, QPalette::Window).lightnessF();
    // https://github.com/quotient-im/libQuotient/wiki/User-color-coding-standard-draft-proposal
    return QColor::fromHslF(color.hsvHueF(), 1, -0.7 * lightness + 0.9, 1);
}
