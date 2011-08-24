// ****************************************************************************
//  mapping.cpp                                                    Tao project
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
#include <iostream>

XL_DEFINE_TRACES

CubeMap* cube;

Tree_p plane(Tree_p,
             Real_p x, Real_p y, Real_p w, Real_p h,
             Integer_p rows_nb, Integer_p columns_nb)
// ----------------------------------------------------------------------------
//   Define a subdivded plane
// ----------------------------------------------------------------------------
{
    coord xx = x, yy = y, ww = w, hh = h;
    int r = rows_nb, c = columns_nb;
    Plane* plane = new Plane(xx, yy, ww, hh, r, c);
    TextureMapping::tao->addToLayout(TextureMapping::render_callback, plane,
                                     TextureMapping::delete_callback);

    return xl_true;
}


Tree_p texture_cube(Context_p context, Integer_p size, Tree_p, Tree_p prog)
// ----------------------------------------------------------------------------
//   Create a cube map texture
// ----------------------------------------------------------------------------
{
    cube = new CubeMap(size);
    context->Evaluate(prog);

    cube->loadCubeMap();

    TextureMapping::tao->addToLayout(TextureMapping::render_callback, cube,
                                     TextureMapping::delete_callback);

    return xl_true;
}


Tree_p cube_map_face(Tree_p tree, GLuint face, text filename)
// ----------------------------------------------------------------------------
//   Add texture to the current cube map
// ----------------------------------------------------------------------------
{
    if (!cube)
    {
        Ooops("No mapping defined '$1' ", tree);
        return xl_false;
    }

    if (!cube->setTexture( filename, face))
    {
        Ooops("No correct face '$1' ", tree);
        return xl_false;
    }

    return xl_true;
}


Tree_p cube_map_flip(Tree_p tree, bool u, bool v)
// ----------------------------------------------------------------------------
//   Allow to flip faces of the current cube map
// ----------------------------------------------------------------------------
{
    if (!cube)
    {
        Ooops("No mapping defined '$1' ", tree);
        return xl_false;
    }

    cube->flip( u, v);

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
