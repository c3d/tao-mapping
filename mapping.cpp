// ****************************************************************************
//  mapping.h                                                       Tao project
// ****************************************************************************
//
//   File Description:
//
//     Implementation of the XL primitives for the "mapping" module.
//
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

#include "mapping.h"

XL_DEFINE_TRACES

CubeMap* cube;

Tree_p plane(Tree_p, Real_p x, Real_p y, Real_p w,
             Real_p h, Integer_p lines_nb, Integer_p columns_nb)
// ----------------------------------------------------------------------------
//    Define a subdivded plane
// ----------------------------------------------------------------------------
{
    Plane* plane = new Plane(x, y, w, h, lines_nb, columns_nb);
    TextureMapping::tao->addToLayout(TextureMapping::render_callback, plane, TextureMapping::delete_callback);

    return xl_true;
}

Tree_p cube_map(Context *context, Tree_p tree, Tree_p prog)
// ----------------------------------------------------------------------------
//    Apply a cube mapping
// ----------------------------------------------------------------------------
{
    cube = new CubeMap();
    context->Evaluate(prog);

    cube->loadCubeMap();

    TextureMapping::tao->addToLayout(TextureMapping::render_callback, cube, TextureMapping::delete_callback);

    return xl_true;
}

Tree_p texture_cube(Tree_p tree, GLuint face, text filename)
// ----------------------------------------------------------------------------
//    Add texture to the current cube map
// ----------------------------------------------------------------------------
{
    if(! cube)
    {
        Ooops("No mapping defined '$1' ", tree);
        return xl_false;
    }

    if(! cube->setTexture( filename, face))
    {
        Ooops("No correct face '$1' ", tree);
        return xl_false;
    }

    return xl_true;
}

int module_init(const Tao::ModuleApi *api, const Tao::ModuleInfo *)
// ----------------------------------------------------------------------------
//   Initialize the Tao module
// ----------------------------------------------------------------------------
{
    XL_INIT_TRACES();
    TextureMapping::tao = api;

    return 0;
}

int module_exit()
// ----------------------------------------------------------------------------
//   Uninitialize the Tao module
// ----------------------------------------------------------------------------
{
    return 0;
}
