// ****************************************************************************
//  cube_map.cpp                                                    Tao project
// ****************************************************************************
//
//   File Description:
//
//   Cube map implementation.
//
//
//
//
//
//
//
// ****************************************************************************
// This software is licensed under the GNU General Public License v3.
// See file COPYING for details.
//  (C) 2011 Baptiste Soulisse <baptiste.soulisse@taodyne.com>
//  (C) 2011 Taodyne SAS
// ****************************************************************************
#include "cube_map.h"

// ============================================================================
//
//   Cube Map
//
// ============================================================================

CubeMap::CubeMap(int size)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : TextureMapping(), cubeMapId(0), cubeMapUID(0), lightsUID(0),
      flip_u(false), flip_v(false), textureCube()
{
    IFTRACE(mapping)
        debug() << "Create cube map\n";

    checkGLContext();

    textureCube.size = size;
}


CubeMap::CubeMap(text cross)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : TextureMapping(), cubeMapId(0), cubeMapUID(0), lightsUID(0),
      flip_u(false), flip_v(false), textureCube()
{
    IFTRACE(mapping)
        debug() << "Create cube map from cross" << cross << "\n";

    checkGLContext();
    
    loadCrossTexture(cross);
}


CubeMap::~CubeMap()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
    if (cubeMapId)
        GL.DeleteTextures(1, &cubeMapId);
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
//   Load files for faces, build texture, add it to Tao's texture list    
// ----------------------------------------------------------------------------
{
    checkGLContext();
    if(!cubeMapId)
    {
        IFTRACE(mapping)
            debug() << "Generate cube map\n";

        // Load cubemap texture
        GL.GenTextures(1, &cubeMapId);
        GL.BindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);

        for(int i = 0; i < 6; i++)
        {
            // Load each face of the cube map
            if (!loadTexture (i))
            {
                GL.DeleteTextures(1, &cubeMapId);
                cubeMapId = 0;
                return false;
            }
        }

        // Setup some parameters for texture filters and mapping
        const uint TCM = GL_TEXTURE_CUBE_MAP;
        GL.BindTexture(TCM, cubeMapId);
        GL.TexParameter(TCM, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GL.TexParameter(TCM, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GL.TexParameter(TCM, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        GL.TexParameter(TCM, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        GL.TexParameter(TCM, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    // This binding allows to get texture id in Tao
    GL.Enable(GL_TEXTURE_CUBE_MAP);
    GL.BindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);

    // Enable pixel blur
    GL.HasPixelBlur(true);
    
    return true;
}


bool CubeMap::loadCrossTexture(text cross)
// ----------------------------------------------------------------------------
//   Load files for faces, build texture, add it to Tao's texture list    
// ----------------------------------------------------------------------------
{
    checkGLContext();
    if(!cubeMapId)
    {
        IFTRACE(mapping)
            debug() << "Generate cube map\n";

        // Load image in tetxure cache
        QImage image = tao->textureImage(cross);

        // Load cubemap texture
        GL.GenTextures(1, &cubeMapId);
        GL.BindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);

        if (image.isNull())
        {
            GL.DeleteTextures(1, &cubeMapId);
            cubeMapId = 0;
            return false;
        }

        textureCube.size = image.height() / 4;
        for(int i = 0; i < 6; i++)
        {
            // Load each face of the cube map
            if (!loadTexture(i, image))
            {
                GL.DeleteTextures(1, &cubeMapId);
                cubeMapId = 0;
                return false;
            }
        }

        // Setup some parameters for texture filters and mapping
        const uint TCM = GL_TEXTURE_CUBE_MAP;
        GL.TexParameter(TCM, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GL.TexParameter(TCM, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GL.TexParameter(TCM, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        GL.TexParameter(TCM, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        GL.TexParameter(TCM, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);        
    }

    // This binding allows to get texture id in Tao
    GL.Enable(GL_TEXTURE_CUBE_MAP);
    GL.BindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);

    // Enable pixel blur
    GL.HasPixelBlur(true);

    return true;
}


void CubeMap::Draw()
// ----------------------------------------------------------------------------
//   Apply fresnel material
// ----------------------------------------------------------------------------
{
    checkGLContext();

    GL.Enable(GL_TEXTURE_CUBE_MAP);
    GL.BindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);

    uint prg_id = GL.CurrentProgram();
    if(!prg_id && program)
    {
        prg_id = program->programId();

        IFTRACE(mapping)
            debug() << "Apply cube map\n";

        // Set shader
        tao->SetShader(prg_id);

        // Set uniform values
        uint tid = GL.ActiveTextureUnitIndex() - GL_TEXTURE0;
        GL.Uniform(cubeMapUID, tid);

        if(tao->isGLExtensionAvailable("GL_EXT_gpu_shader4"))
        {
            GLint lightsmask =  GL.LightsMask();
            GL.Uniform(lightsUID, lightsmask);
        }
    }
}


TextureFace* CubeMap::whichFace(uint face)
// ----------------------------------------------------------------------------
//  Get adress of the texture name according to a given face
// ----------------------------------------------------------------------------
{
    switch(face)
    {
        case 0:  return &textureCube.right;  break;
        case 1:  return &textureCube.left;   break;
        case 2:  return &textureCube.top;    break;
        case 3:  return &textureCube.bottom; break;
        case 4:  return &textureCube.front ; break;
        default: return &textureCube.back;   break;
    }
}


bool CubeMap::loadTexture (uint face)
// ----------------------------------------------------------------------------
//   Load a texture for the given face and bind it to the cubemap texture
// ----------------------------------------------------------------------------
{
    TextureFace* currentFace = whichFace(face);
    QImage image = tao->textureImage(currentFace->name);
    if (!image.isNull())
    {
        if (textureCube.size <= 0)
            textureCube.size = image.width();
        if (textureCube.size != image.width() ||
            textureCube.size != image.height())
            image = image.scaled(textureCube.size, textureCube.size,
                                 Qt::IgnoreAspectRatio,
                                 Qt::SmoothTransformation);
        currentFace->loadTexture(face, image);
        return true;
    }
    return false;
}


bool CubeMap::loadTexture (uint face, QImage &cross)
// ----------------------------------------------------------------------------
//   Load a texture for the given face and bind it to the cubemap texture
// ----------------------------------------------------------------------------
{
    TextureFace* currentFace = whichFace(face);
    int size = cross.height() / 4;
    int x = size;
    int y = size;
    switch(face)
    {
    case 0: x = 2*size; break;
    case 1: x = 0;      break;
    case 2: y = 0;      break;
    case 3: y = 2*size; break;
    case 4: y = 1*size; break;
    case 5: y = 3*size; break;
    default: return false;
    }
    QImage tile = cross.copy(x, y, size, size);
    if (face == 5)
        tile = tile.mirrored(true, true); 
    currentFace->loadTexture(face, tile);
    return true;
}


void TextureFace::loadTexture(uint face, QImage &image)
// ----------------------------------------------------------------------------
//    Load a given image
// ----------------------------------------------------------------------------
{    
    QImage texture = QGLWidget::convertToGLFormat(image);
    if (flip_u || flip_v)
        texture = texture.mirrored(flip_u, flip_v);
    GL.TexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA,
                  texture.width(), texture.height(), 0, GL_RGBA,
                  GL_UNSIGNED_BYTE, texture.bits());
}


void CubeMap::createShaders()
// ----------------------------------------------------------------------------
//   Create shader programs
// ----------------------------------------------------------------------------
{
    if(!GL.CurrentProgram())
    {
        IFTRACE(mapping)
                debug() << "Create cube map shader\n";

        delete program;

        program = new QGLShaderProgram(context);
        bool ok = false;

        // Basic vertex shader
        kstring vSrc =
              "/********************************************************************************\n"
              "**                                                                               \n"
              "** Copyright (C) 2011 Taodyne.                                                   \n"
              "** All rights reserved.                                                          \n"
              "** Contact: Taodyne (contact@taodyne.com)                                        \n"
              "**                                                                               \n"
              "** This file is part of the Tao3D application, developped by Taodyne.\n"
              "** It can be only used in the software and these modules.                        \n"
              "**                                                                               \n"
              "** If you have questions regarding the use of this file, please contact          \n"
              "** Taodyne at contact@taodyne.com.                                               \n"
              "**                                                                               \n"
              "********************************************************************************/\n"
              "varying vec3 viewDir;"
              "varying vec3 normal;"
              "varying vec4 color;"
              "void main()"
              "{"
              "   /* Generate texture coordinates (equivalent to glTexGen) */"
              "    vec4 xPlane = vec4( 1.0, 0.0, 0.0, 0.0 );"
              "    vec4 yPlane = vec4( 0.0, 1.0, 0.0, 0.0 );"
              "    vec4 zPlane = vec4( 0.0, 0.0, 1.0, 0.0 );"

              "    gl_TexCoord[0].x = dot(gl_TextureMatrix[0] * vec4(gl_Vertex.xyz, 1.0), xPlane);"
              "    gl_TexCoord[0].y = dot(gl_TextureMatrix[0] * vec4(gl_Vertex.xyz, 1.0), yPlane);"
              "    gl_TexCoord[0].z = dot(gl_TextureMatrix[0] * vec4(gl_Vertex.xyz, 1.0), zPlane);"

              "    /* Compute position */"
              "    gl_Position = ftransform();"

              "    /* Compute world position and normal */"
              "    normal  = gl_NormalMatrix * gl_Normal;"
              "    viewDir = -vec3(gl_ModelViewMatrix * gl_Vertex);"

              "    color = gl_Color;"
              "}";

        kstring fSrc;
        if(tao->isGLExtensionAvailable("GL_EXT_gpu_shader4"))
        {
            // If the extension is available, use this shader
            // to handle multiple lights
            fSrc =
                    "/********************************************************************************\n"
                    "**                                                                               \n"
                    "** Copyright (C) 2011 Taodyne.                                                   \n"
                    "** All rights reserved.                                                          \n"
                    "** Contact: Taodyne (contact@taodyne.com)                                        \n"
                    "**                                                                               \n"
                    "** This file is part of the Tao3D application, developped by Taodyne.\n"
                    "** It can be only used in the software and these modules.                        \n"
                    "**                                                                               \n"
                    "** If you have questions regarding the use of this file, please contact          \n"
                    "** Taodyne at contact@taodyne.com.                                               \n"
                    "**                                                                               \n"
                    "********************************************************************************/\n"
                    "#extension GL_EXT_gpu_shader4 : require\n"

                    "/* Mapping parameters */"
                    "uniform int         lights;"
                    "uniform samplerCube cubeMap;"

                    "varying vec3 viewDir;"
                    "varying vec3 normal;"
                    "varying vec4 color;"

                    "/**"
                    "* Compute render color according to materials,"
                    "* lights and colors parameters which are"
                    "* set in the current scene."
                    "**/"
                    "vec4 computeRenderColor(vec4 renderColor)"
                    "{"
                    "    vec4 lighting_color = renderColor;"
                    "    if(lights > 0)"
                    "    {"
                    "        /* Define a maximum of lights supported */"
                    "        int MAX_LIGHTS = 8;"

                    "        vec3 N = normalize(normal);"
                    "        vec3 V = normalize(viewDir);"

                    "        vec4 ambient  = vec4(0.0);"
                    "        vec4 diffuse  = vec4(0.0);"
                    "        vec4 specular = vec4(0.0);"
                    "        ambient = gl_FrontLightModelProduct.sceneColor"
                    "                * gl_FrontMaterial.ambient;"

                    "        for(int i = 0; i < MAX_LIGHTS; i++)"
                    "        {"
                    "            if(bool(lights & (1 << i)))"
                    "            {"
                    "                vec3 L = normalize(gl_LightSource[i].position.xyz);"

                    "                /* Compute ambient part */"
                    "                ambient += gl_LightSource[i].ambient * gl_FrontMaterial.ambient;"

                    "                /* Diffuse coefficient */"
                    "                float nDotL = max(clamp(dot(L, N), 0.0, 1.0), 0.0);"
                    "                if (nDotL > 0.0)"
                    "                {"
                    "                    /* Compute diffuse part */"
                    "                    diffuse += gl_LightSource[i].diffuse"
                    "                             * gl_FrontMaterial.diffuse"
                    "                             * nDotL;"

                    "                    /* Compute specular coefficient */"
                    "                    float nDotV = clamp(dot(reflect(-L, N), V), 0.0, 1.0);"
                    "                    if (nDotV > 0.0)"
                    "                    {"
                    "                        /* Compute specular part */"
                    "                        specular += gl_LightSource[i].specular"
                    "                                  * gl_FrontMaterial.specular"
                    "                                  * pow(nDotV, gl_FrontMaterial.shininess);"
                    "                    }"
                    "                }"
                    "            }"
                    "        }"

                    "         /* Define new render color */"
                    "        lighting_color  = (ambient + diffuse) * renderColor + specular;"
                    "    }"
                    "    else"
                    "    {"
                    "        lighting_color = renderColor * color;"
                    "    }"
                    ""
                    "    return lighting_color;"
                    "}"


                    "void main()"
                    "{"
                    "   vec4 renderColor = textureCube(cubeMap, gl_TexCoord[0].xyz);"
                    "   gl_FragColor     = computeRenderColor(renderColor);"
                    "}";

        }
        else
        {
            // If the extension is not available, use this shader
            // to handle an unique light.
            fSrc =
                    /********************************************************************************
                    **
                    ** Copyright (C) 2011 Taodyne.
                    ** All rights reserved.
                    ** Contact: Taodyne (contact@taodyne.com)
                    **
                    ** This file is part of the Tao3D application, developped by Taodyne.
                    ** It can be only used in the software and these modules.
                    **
                    ** If you have questions regarding the use of this file, please contact
                    ** Taodyne at contact@taodyne.com.
                    **
                    ********************************************************************************/
                    "/* Mapping parameters */"
                    "uniform samplerCube cubeMap;"

                    "varying vec4 color;"
                    "void main()"
                    "{"
                    "   vec4 renderColor = textureCube(cubeMap, gl_TexCoord[0].xyz) * color;"
                    "   gl_FragColor     = renderColor * color;"
                    "}";
        }

        if (program->addShaderFromSourceCode(QGLShader::Vertex, vSrc))
        {
            if (program->addShaderFromSourceCode(QGLShader::Fragment, fSrc))
            {
                ok = true;
            }
            else
            {
                std::cerr << "Error loading fragment shader code:\n";
                std::cerr << program->log().toStdString();
            }
        }
        else
        {
            std::cerr << "Error loading vertex shader code:\n";
            std::cerr << program->log().toStdString();
        }
        if (!ok)
        {
            delete program;
            program = NULL;
        }
        else
        {
            program->link();

            // Save uniform locations
            uint id = program->programId();
            cubeMapUID = GL.GetUniformLocation(id, "cubeMap");
            lightsUID = GL.GetUniformLocation(id, "lights");
        }
    }
}
