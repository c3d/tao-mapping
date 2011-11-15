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

DEFINES     += GLEW_STATIC

include(../modules.pri)
INCLUDEPATH += $${TAOTOPSRC}/tao/include/tao/
HEADERS      = \
               mapping.h \
    texture_mapping.h \
    plane.h \
    cube_map.h \
    noise_map.h \
    3rdparty/fbm.h \

SOURCES      = mapping.cpp $${TAOTOPSRC}/tao/include/tao/GL/glew.c \
    texture_mapping.cpp \
    plane.cpp \
    cube_map.cpp \
    noise_map.cpp \
    3rdparty/fbm.c \

TBL_SOURCES  = mapping.tbl
OTHER_FILES  = mapping.xl mapping.tbl traces.tbl
QT          += core \
               gui \
               opengl

INSTALLS    += thismod_icon


