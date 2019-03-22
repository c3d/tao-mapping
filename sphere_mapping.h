#ifndef SPHERE_MAPPING_H
#define SPHERE_MAPPING_H
// *****************************************************************************
// sphere_mapping.h                                                Tao3D project
// *****************************************************************************
//
// File description:
//
//      Define sphere mapping.
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2012-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2011-2012, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2015,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2012-2013, Jérôme Forissier <jerome@taodyne.com>
// (C) 2012-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
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

#include "tao/coords3d.h"
#include "tao/matrix.h"
#include "tao/module_api.h"
#include "tao/tao_gl.h"
#include "texture_mapping.h"
#include <QObject>

using namespace std;
using namespace Tao;


struct SphereMapping : public TextureMapping
{
    SphereMapping(float ratio);
    ~SphereMapping();

    // Draw sphere mapping
    virtual void    Draw();

protected:
    virtual void    createShaders();

private:
    float   ratio;
    GLfloat model[4][4]; // model matrix
    uint    ratioUID, lightsUID, colorMapUID, sphereMapUID;
    uint    hasColorMapUID, cameraUID, modelMatrixUID;
};


#endif
