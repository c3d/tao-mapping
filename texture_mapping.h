#ifndef TEXTURE_MAPPING_H
#define TEXTURE_MAPPING_H
// ****************************************************************************
//  cube_map.h                                                  Tao project
// ****************************************************************************
//
//   File Description:
//
//      Define a cube map.
//
//
//
//
//
// ****************************************************************************
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriété de Taodyne SAS - Confidentiel
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "tao/coords3d.h"
#include "tao/module_api.h"
#include "tao/tao_gl.h"
#include <QObject>

using namespace std;
using namespace Tao;

struct TextureMapping : public QObject
{
    TextureMapping();
    ~TextureMapping();

    // Draw mapping
    virtual void    Draw();

    static void     render_callback(void *arg);
    static void     delete_callback(void *arg);

public:
    // Pointer to Tao functions
    static const Tao::ModuleApi *tao;
};


#endif // TEXTURE_MAPPING_H
