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
#include "main.h"
#include "runtime.h"
#include "base.h"
#include "plane.h"
#include "texture_mapping.h"

XL_DEFINE_TRACES

Tree_p plane(Tree_p tree, Real_p x, Real_p y, Real_p w,
             Real_p h, Integer_p lines_nb, Integer_p columns_nb)
// ----------------------------------------------------------------------------
//    Define the lens flare target
// ----------------------------------------------------------------------------
{
    Plane* plane = new Plane(x, y, w, h, lines_nb, columns_nb);
    TextureMapping::tao->addToLayout(Plane::render_callback, plane, Plane::delete_callback);

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
