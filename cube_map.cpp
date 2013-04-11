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
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriété de Taodyne SAS - Confidentiel
//  (C) 2011 Baptiste Soulisse <baptiste.soulisse@taodyne.com>
//  (C) 2011 Taodyne SAS
// ****************************************************************************
#include "cube_map.h"

// ============================================================================
//
//   Cube Map
//
// ============================================================================

CubeMap::context_to_textures CubeMap::texture_maps;
bool                         CubeMap::failed = false;
QGLShaderProgram*            CubeMap::pgm = NULL;
std::map<text, GLint>        CubeMap::uniforms;
const QGLContext*            CubeMap::context = NULL;

CubeMap::CubeMap(int size)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : TextureMapping(&context), size(size), flip_u(false), flip_v(false)
{
    IFTRACE(mapping)
            debug() << "Create cube map" << "\n";

    checkGLContext();

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

        IFTRACE(mapping)
                debug() << "Generate cube map" << "\n";

        // Load cubemap texture
        glGenTextures (1, &cubeMapId);
        glBindTexture (GL_TEXTURE_CUBE_MAP, cubeMapId);

        // Setup some parameters for texture filters and mapping
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

    if (!tested)
    {
        if(tao->hasImpressOrLicense("Materials 1.0"))
            licensed = true;
        else
            licensed = tao->checkImpressOrLicense(MAPPING_FEATURE);
        tested = true;
    }

    // Set to the textures list in Tao.
    TextureMapping::tao->BindTexture(cubeMapId, GL_TEXTURE_CUBE_MAP);

    return true;
}


void CubeMap::Draw()
// ----------------------------------------------------------------------------
//   Apply fresnel material
// ----------------------------------------------------------------------------
{
    checkGLContext();

    // Enable pixel blur
    TextureMapping::tao->HasPixelBlur(true);

    uint prg_id = 0;
    if(pgm)
        prg_id = pgm->programId();

    if(prg_id)
    {
        IFTRACE(mapping)
                debug() << "Apply cube map" << "\n";

        // Set shader
        tao->SetShader(prg_id);

        // Set uniform values
        glUniform1i(uniforms["cubeMap"], tao->TextureUnit());

        if(tao->isGLExtensionAvailable("GL_EXT_gpu_shader4"))
        {
            GLint lightsmask = tao->EnabledLights();
            glUniform1i(uniforms["lights"], lightsmask);
        }
    }
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
    TextureMapping::checkGLContext();
    if (!texture_maps.count(QGLContext::currentContext()))
    {
        texture_map m;
        texture_maps[QGLContext::currentContext()] = m;
    }
}


void CubeMap::createShaders()
// ----------------------------------------------------------------------------
//   Create shader programs
// ----------------------------------------------------------------------------
{
    if(!failed)
    {
        IFTRACE(mapping)
                debug() << "Create cube map shader" << "\n";

        delete pgm;

        pgm = new QGLShaderProgram(*pcontext);
        bool ok = false;

        // Basic vertex shader
        static string vSrc =
              "/********************************************************************************\n"
              "**                                                                               \n"
              "** Copyright (C) 2011 Taodyne.                                                   \n"
              "** All rights reserved.                                                          \n"
              "** Contact: Taodyne (contact@taodyne.com)                                        \n"
              "**                                                                               \n"
              "** This file is part of the Tao Presentations application, developped by Taodyne.\n"
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

        static string fSrc;
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
                    "** This file is part of the Tao Presentations application, developped by Taodyne.\n"
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

                    "    return lighting_color;"
                    "}"


                    "void main()"
                    "{"
                    "   vec4 renderColor = textureCube(cubeMap, gl_TexCoord[0].xyz) * color;"
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
                    ** This file is part of the Tao Presentations application, developped by Taodyne.
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

        if (pgm->addShaderFromSourceCode(QGLShader::Vertex, vSrc.c_str()))
        {
            if (pgm->addShaderFromSourceCode(QGLShader::Fragment, fSrc.c_str()))
            {
                ok = true;
            }
            else
            {
                std::cerr << "Error loading fragment shader code: " << "\n";
                std::cerr << pgm->log().toStdString();
            }
        }
        else
        {
            std::cerr << "Error loading vertex shader code: " << "\n";
            std::cerr << pgm->log().toStdString();
        }
        if (!ok)
        {
            delete pgm;
            pgm = NULL;
            failed = true;
        }
        else
        {
            pgm->link();

            // Save uniform locations
            uint id = pgm->programId();

            uniforms["cubeMap"] = glGetUniformLocation(id, "cubeMap");
            uniforms["lights"] = glGetUniformLocation(id, "lights");
        }
    }
}
