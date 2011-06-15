// ****************************************************************************
//  Plane.cpp                                                       Tao project
// ****************************************************************************
//
//   File Description:
//
//   Plane implementation.
//
//
//
//
//
//
//
// ****************************************************************************
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriÃ©tÃ© de Taodyne SAS - Confidentiel
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************
#include <string.h>
#include <math.h>
#include "CubeMap.h"

CubeMap::CubeMap()
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
{
}

CubeMap::~CubeMap()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
    glDeleteTextures(1, &cubeMapId);
}

bool CubeMap::setTexture(const char* filename, GLuint face)
{
    if(face >= 0 && face < 6)
    {
        textures.insert(textures.begin() + face, filename);
        return true;
    }
    return false;
}

bool CubeMap::loadTexture (const char* filename, GLuint face)
{
    /*QImage image(filename);
    if (!image.isNull())
    {
        QImage texture = QGLWidget::convertToGLFormat(image);
        glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA,
                      texture.width(), texture.height(), 0, GL_RGBA,
                      GL_UNSIGNED_BYTE, texture.bits());
        return true;
    }
    return false;*/
    return true;
}

bool CubeMap::loadCubeMap ()
{
    if(textures.size() != 6)
        return false;

    // Load cubemap texture
    glGenTextures (1, &cubeMapId);
    glBindTexture (GL_TEXTURE_CUBE_MAP, cubeMapId);

    // Detup some parameters for texture filters and mipmapping
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for(int i = 0; i < 6; i++)
    {
        // Load each face of the cube map
        if (!loadTexture (textures[i], i))
        {
            glDeleteTextures (1, &cubeMapId);
            return false;
        }
    }

    TextureMapping::tao->SetTexture(cubeMapId, GL_TEXTURE_CUBE_MAP);

    return true;
}

void CubeMap::Draw()
{
}
