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
// This software is licensed under the GNU General Public License v3.
// See file COPYING for details.
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************
#include "tree.h"
#include "main.h"
#include "runtime.h"
#include "base.h"
#include "tao/module_api.h"
#include "cube_map.h"
#include "noise_map.h"
#include "cube_mapping.h"
#include "sphere_mapping.h"


using namespace XL;


Tree_p plane(Tree_p tree, Real_p x, Real_p y, Real_p w,
             Real_p h, Integer_p lines_nb, Integer_p columns_nb);

Tree_p cube_map(Context *context, Integer_p size, Tree_p, Tree_p prog);
Tree_p cube_map_cross(text cross);
Tree_p cube_map_face(Tree_p tree, GLuint face, text name);
Tree_p cube_map_flip(Tree_p tree, bool u, bool v);

Tree_p cube_mapping(Tree_p tree, Real_p ratio);
Tree_p sphere_mapping(Tree_p tree, Real_p ratio);

Tree_p noise_map_3D(Tree_p tree, GLuint w, GLuint h, GLuint seed);


#endif // MAPPING_H
