#ifndef MAPPING_H
#define MAPPING_H
// ****************************************************************************
//  mapping.h                                                       Tao project
// ****************************************************************************
//
//   File Description:
//
//    Prototype of the function used by mapping.tbl to
//    implement the new XL primitive
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
#include "tree.h"
#include "main.h"
#include "runtime.h"
#include "base.h"
#include "tao/module_api.h"
#include "plane.h"
#include "cube_map.h"
#include "texture_mapping.h"


using namespace XL;


Tree_p plane(Tree_p tree, Real_p x, Real_p y, Real_p w,
             Real_p h, Integer_p lines_nb, Integer_p columns_nb);
Tree_p cube_map(Context *context, Tree_p, Tree_p prog);
Tree_p texture_cube(Tree_p tree, GLuint face, text name);


#endif // MAPPING_H
