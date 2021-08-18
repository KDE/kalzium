/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2007 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef LIBKDEEDU_SCIENCE_EXPORT_H
#define LIBKDEEDU_SCIENCE_EXPORT_H

#ifndef SCIENCE_EXPORT
# if defined(MAKE_SCIENCE_LIB)
   /* We are building this library */ 
#  define SCIENCE_EXPORT Q_DECL_EXPORT
# else
   /* We are using this library */ 
#  define SCIENCE_EXPORT Q_DECL_IMPORT
# endif
#endif

# ifndef SCIENCE_EXPORT_DEPRECATED
#  define SCIENCE_EXPORT_DEPRECATED KDE_DEPRECATED SCIENCE_EXPORT
# endif

#endif // LIBKDEEDU_SCIENCE_EXPORT_H
