# ******************************************************************************
# mapping.pro                                                      Tao3D project
# ******************************************************************************
#
# File description:
# Qt build file for the Lorem ipsum module
#
#
#
#
#
#
#
# ******************************************************************************
# This software is licensed under the GNU General Public License v3
# (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
# (C) 2011-2013, Catherine Burvelle <catherine@taodyne.com>
# (C) 2011-2015,2019, Christophe de Dinechin <christophe@dinechin.org>
# (C) 2011-2013, Jérôme Forissier <jerome@taodyne.com>
# (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
# ******************************************************************************
# This file is part of Tao3D
#
# Tao3D is free software: you can r redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Tao3D is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Tao3D, in a file named COPYING.
# If not, see <https://www.gnu.org/licenses/>.
# ******************************************************************************

MODINSTDIR = mapping

include(../modules.pri)
INCLUDEPATH += $${TAOTOPSRC}/tao/include/tao/
HEADERS      = \
               mapping.h \
    texture_mapping.h \
    cube_map.h \
    cube_mapping.h \
    sphere_mapping.h \
    noise_map.h \
    3rdparty/fbm.h \

SOURCES      = mapping.cpp \
    texture_mapping.cpp \
    cube_map.cpp \
    cube_mapping.cpp \
    sphere_mapping.cpp \
    noise_map.cpp \
    3rdparty/fbm.cpp \

PICTURES = \
    colorMap.png \
    displacementMap.png \
    $$NULL

pics.path  = $$MODINSTPATH
pics.files = $$PICTURES

xlfiles.path  = $$MODINSTPATH
xlfiles.files =     normal_mapping.xl \
    noise_mapping.xl \
    alpha_mapping.xl \
    displacement_mapping.xl


TBL_SOURCES  = mapping.tbl
OTHER_FILES  = mapping.xl mapping.tbl traces.tbl \
    normal_mapping.xl \
    noise_mapping.xl \
    alpha_mapping.xl \
    displacement_mapping.xl

QT          += core \
               gui \
               opengl

INSTALLS    += thismod_icon pics xlfiles

QMAKE_SUBSTITUTES = doc/Doxyfile.in
DOXYFILE = doc/Doxyfile
DOXYLANG = en,fr
include(../modules_doc.pri)
