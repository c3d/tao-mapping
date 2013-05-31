# ******************************************************************************
#  lorem_ipsum.pro                                                  Tao project
# ******************************************************************************
# File Description:
# Qt build file for the Lorem ipsum module
# ******************************************************************************
# This software is property of Taodyne SAS - Confidential
# Ce logiciel est la propriété de Taodyne SAS - Confidentiel
# (C) 2010 Jerome Forissier <jerome@taodyne.com>
# (C) 2010 Taodyne SAS
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

win32 {
  DEFINES     += GLEW_STATIC
  SOURCES     += $${TAOTOPSRC}/tao/include/tao/GL/glew.c
}

CRYPT_XL_SOURCES = alpha_mapping.xl displacement_mapping.xl normal_mapping.xl noise_mapping.xl
include(../crypt_xl.pri)

TBL_SOURCES  = mapping.tbl
OTHER_FILES  = mapping.xl mapping.tbl traces.tbl \
    normal_mapping.xl \
    noise_mapping.xl \
    alpha_mapping.xl \
    displacement_mapping.xl
QT          += core \
               gui \
               opengl

INSTALLS    += thismod_icon pics

LICENSE_FILES = mapping.taokey.notsigned
include(../licenses.pri)

QMAKE_SUBSTITUTES = doc/Doxyfile.in
DOXYFILE = doc/Doxyfile
DOXYLANG = en,fr
include(../modules_doc.pri)
