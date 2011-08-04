// ****************************************************************************
//  cube_map.cpp                                                    Tao project
// ****************************************************************************
//
//   File Description:
//
//   cubeMap implementation.
//
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
#include <string.h>
#include <math.h>
#include "cube_map.h"

CubeMap::context_to_textures CubeMap::texture_maps;

CubeMap::CubeMap(int size) : size(size), flip_u(false), flip_v(false)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
{    
    currentTexture.size = size;
}

CubeMap::~CubeMap()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}

void CubeMap::flip(bool u, bool v)
// ----------------------------------------------------------------------------
//   Flip faces of the cube map
// ----------------------------------------------------------------------------
{
    flip_u = u;
    flip_v = v;
}

bool CubeMap::setTexture(text filename, uint face)
// ----------------------------------------------------------------------------
//   Set a texture to a given face of the cube
// ----------------------------------------------------------------------------
{
    if(face < 6)
    {
        TextureFace* currentFace = whichFace(face);
        currentFace->name = filename;
        currentFace->flip_u = flip_u;
        currentFace->flip_v = flip_v;

        return true;
    }
    return false;
}

bool CubeMap::loadCubeMap()
// ----------------------------------------------------------------------------
//   Create a cubemap texture according to textures of the different faces
//   and set it to the textures list in Tao
// ----------------------------------------------------------------------------
{
    checkGLContext();
    GLuint cubeMapId = isInclude();

    if(! cubeMapId)
    {
        // Prune the map if it gets too big
        while (textures.size() > MAX_TEXTURES)
        {
            texture_map::iterator first = textures.begin();
            glDeleteTextures(1, &(*first).first);
            textures.erase(first);
        }

        // Load cubemap texture
        glGenTextures (1, &cubeMapId);
        glBindTexture (GL_TEXTURE_CUBE_MAP, cubeMapId);

        // Setup some parameters for texture filters and mipmapping
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        for(int i = 0; i < 6; i++)
        {
            // Load each face of the cube map
            if (!loadTexture (i))
            {
                glDeleteTextures (1, &cubeMapId);
                return false;
            }
        }

        // Remember for the next time
        textures[cubeMapId] = currentTexture;
    }

    // Set to the textures list in Tao.
    TextureMapping::tao->SetTexture(cubeMapId, GL_TEXTURE_CUBE_MAP);

    return true;
}

void CubeMap::Draw()
// ----------------------------------------------------------------------------
//   Draw cube map texture
// ----------------------------------------------------------------------------
{
}

uint CubeMap::isInclude()
// ----------------------------------------------------------------------------
//  Check if the current cubemap texture has been already created
// ----------------------------------------------------------------------------
{
   if(textures.size() != 0)
   {
       texture_map::iterator it;
       for(it = textures.begin(); it != textures.end(); it++)
           if(textures.find((*it).first) != textures.end())
                if(textures[(*it).first] == currentTexture)
                    return (*it).first;
   }
   return 0;
}

TextureFace* CubeMap::whichFace(uint face)
// ----------------------------------------------------------------------------
//  Get adress of the texture name according to a given face
// ----------------------------------------------------------------------------
{
    switch(face)
    {
        case 0:  return &currentTexture.right;  break;
        case 1:  return &currentTexture.left;   break;
        case 2:  return &currentTexture.top;    break;
        case 3:  return &currentTexture.bottom; break;
        case 4:  return &currentTexture.front ; break;
        default: return &currentTexture.back;   break;
    }
}

bool CubeMap::loadTexture (uint face)
// ----------------------------------------------------------------------------
//   Load a texture according to a given face and
//   bind it to the face of the cubemap texture
// ----------------------------------------------------------------------------
{
    TextureFace* currentFace = whichFace(face);
    QImage image(currentFace->name.c_str());
    if (image.isNull())
    {
        text qualified = "texture:" + currentFace->name;
        image.load(qualified.c_str());
    }
    if (!image.isNull())
    {
        if (size <= 0)
            size = image.width();
        if (size != image.width() || size != image.height())
            image = image.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        QImage texture = QGLWidget::convertToGLFormat(image);
        texture = texture.mirrored(currentFace->flip_u, currentFace->flip_v);

        glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA,
                      texture.width(), texture.height(), 0, GL_RGBA,
                      GL_UNSIGNED_BYTE, texture.bits());

        return true;
    }
    return false;
}

void CubeMap::checkGLContext()
// ----------------------------------------------------------------------------
//   Make sure a texture_map has been allocated for the current GL context
// ----------------------------------------------------------------------------
{
    if (!texture_maps.count(QGLContext::currentContext()))
    {
        texture_map m;
        texture_maps[QGLContext::currentContext()] = m;
    }
}
