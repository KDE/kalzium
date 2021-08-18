/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2007 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef LIBKDEEDU_COMPOUNDVIEWER_EXPORT_H
#define LIBKDEEDU_COMPOUNDVIEWER_EXPORT_H

#ifndef COMPOUNDVIEWER_EXPORT
# if defined(MAKE_COMPOUNDVIEWER_LIB)
/* We are building this library */
#  define COMPOUNDVIEWER_EXPORT Q_DECL_EXPORT
# else
/* We are using this library */
#  define COMPOUNDVIEWER_EXPORT Q_DECL_IMPORT
# endif
#endif

# ifndef COMPOUNDVIEWER_EXPORT_DEPRECATED
#  define COMPOUNDVIEWER_EXPORT_DEPRECATED KDE_DEPRECATED COMPOUNDVIEWER_EXPORT
# endif

#endif // LIBKDEEDU_COMPOUNDVIEWER_EXPORT_H
