// SPDX-FileCopyrightText: 2023 Carl Schwan <carl@carlschwan.eu>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <QColor>

namespace ColorUtils
{
QColor tintWithAlpha(const QColor &targetColor, const QColor &tintColor, double alpha);
QColor contrastColor(const QColor &color);
}
