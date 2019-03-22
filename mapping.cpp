// *****************************************************************************
// mapping.cpp                                                     Tao3D project
// *****************************************************************************
//
// File description:
//
//     Implementation of the XL primitives for the "mapping" module.
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

#include "mapping.h"
#include <iostream>

XL_DEFINE_TRACES

#define MAX_SEEDS 200 // Maximum of noise seeds

CubeMap* cubeMap;


Tree_p cube_map(Context *context, Integer_p size, Tree_p, Tree_p prog)
// ----------------------------------------------------------------------------
//   Create a cube map texture
// ----------------------------------------------------------------------------
{
    cubeMap = new CubeMap(size);
    context->Evaluate(prog);

    cubeMap->loadCubeMap();

    TextureMapping::tao->AddToLayout2(TextureMapping::render_callback,
                                      TextureMapping::identify_callback,
                                      cubeMap,
                                      TextureMapping::delete_callback);

    return xl_true;
}


Tree_p cube_map_cross(text cross)
// ----------------------------------------------------------------------------
//    Create a cube map from a cross image
// ----------------------------------------------------------------------------
{
    cubeMap = new CubeMap(cross);
    TextureMapping::tao->AddToLayout2(TextureMapping::render_callback,
                                      TextureMapping::identify_callback,
                                      cubeMap,
                                      TextureMapping::delete_callback);
    return xl_true;
}


Tree_p cube_map_face(Tree_p tree, GLuint face, text filename)
// ----------------------------------------------------------------------------
//   Add texture to the current cube map
// ----------------------------------------------------------------------------
{
    if (!cubeMap)
    {
        Ooops("Mapping: No mapping defined for cube map face in $1", tree);
        return xl_false;
    }

    if (!cubeMap->setTexture(filename, face))
    {
        Ooops("Mapping: Cube map texture '$2' could not be set in $1", tree)
            .Arg(filename);
        return xl_false;
    }

    return xl_true;
}


Tree_p cube_map_flip(Tree_p tree, bool u, bool v)
// ----------------------------------------------------------------------------
//   Allow to flip faces of the current cube map
// ----------------------------------------------------------------------------
{
    if (!cubeMap)
    {
        Ooops("Mapping: No cube map for flip in $1 ", tree);
        return xl_false;
    }

    cubeMap->flip( u, v);

    return xl_true;
}


Tree_p cube_mapping(Tree_p /*tree*/, Real_p ratio)
// ----------------------------------------------------------------------------
//   Apply cube mapping
// ----------------------------------------------------------------------------
{
    CubeMapping* cubeMapping = new CubeMapping(ratio);
    TextureMapping::tao->AddToLayout2(TextureMapping::render_callback,
                                      TextureMapping::identify_callback,
                                      cubeMapping,
                                      TextureMapping::delete_callback);

    return xl_true;
}


Tree_p sphere_mapping(Tree_p /*tree*/, Real_p ratio)
// ----------------------------------------------------------------------------
//   Apply cube mapping
// ----------------------------------------------------------------------------
{
    SphereMapping* sphereMapping = new SphereMapping(ratio);
    TextureMapping::tao->AddToLayout2(TextureMapping::render_callback,
                                      TextureMapping::identify_callback,
                                      sphereMapping,
                                      TextureMapping::delete_callback);

    return xl_true;
}



Tree_p noise_map_3D(Tree_p tree, GLuint w, GLuint h, GLuint seed)
// ----------------------------------------------------------------------------
//   Generate a 3D noise texture
// ----------------------------------------------------------------------------
{
    if(seed > MAX_SEEDS)
    {
        Ooops("Mapping: Invalid seed value $2 for 3D noise map in $1, "
              "maximum is $3", tree).Arg(seed).Arg(MAX_SEEDS);
        return xl_false;
    }

    NoiseMap* noiseMap = new NoiseMap(w, h, seed);
    TextureMapping::tao->addToLayout(TextureMapping::render_callback,
                                     noiseMap,
                                     TextureMapping::delete_callback);

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
