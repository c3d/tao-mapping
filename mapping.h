#ifndef MAPPING_H
#define MAPPING_H
// *****************************************************************************
// mapping.h                                                       Tao3D project
// *****************************************************************************
//
// File description:
//
//    Prototype of the function used by mapping.tbl to
//    implement the new XL primitive
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2011-2013, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2015,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2011-2013, Jérôme Forissier <jerome@taodyne.com>
// (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// *****************************************************************************
// This file is part of Tao3D
//
// Tao3D is free software: you can r redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tao3D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tao3D, in a file named COPYING.
// If not, see <https://www.gnu.org/licenses/>.
// *****************************************************************************
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
