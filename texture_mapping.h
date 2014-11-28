﻿#ifndef TEXTURE_MAPPING_H
#define TEXTURE_MAPPING_H
// ****************************************************************************
//  texture_mapping.h                                               Tao project
// ****************************************************************************
//
//   File Description:
//
//      Define texture mapping.
//
//
//
//
//
// ****************************************************************************
// This software is licensed under the GNU General Public License v3.
// See file COPYING for details.
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "tao/coords3d.h"
#include "tao/module_api.h"
#include "tao/graphic_state.h"
#include "tao/tao_gl.h"
#include <QObject>
#include <QGLContext>
#include <QGLShaderProgram>

using namespace std;
using namespace Tao;


#define MAPPING_FEATURE "Mapping 1.008"  // Used in checkLicense etc.

struct TextureMapping : QObject
{
    TextureMapping(const QGLContext **pcontext = NULL);
    ~TextureMapping();

    // Draw mapping
    virtual void    Draw();

    // Re-create shaders if GL context has changed
    void            checkGLContext();
    virtual void    createShaders();

    std::ostream &  debug();

    static void     render_callback(void *arg);
    static void     identify_callback(void *arg);
    static void     delete_callback(void *arg);

public:
    const QGLContext    **pcontext;

public:
    // Pointer to Tao functions
    static const Tao::ModuleApi *tao;
};


#endif
