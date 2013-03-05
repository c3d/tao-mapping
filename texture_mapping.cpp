// ****************************************************************************
//  texture_mapping.cpp                                             Tao project
// ****************************************************************************
//
//   File Description:
//
//   Texture Mapping implementation.
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
#include "texture_mapping.h"

const Tao::ModuleApi *TextureMapping::tao = NULL;

bool TextureMapping::tested = false;
bool TextureMapping::licensed = false;

// ============================================================================
//
//    Material
//
// ============================================================================

TextureMapping::TextureMapping(const QGLContext **pcontext)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : pcontext(pcontext)
{
}


TextureMapping::~TextureMapping()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}


void TextureMapping::render_callback(void *arg)
// ----------------------------------------------------------------------------
//   Rendering callback: call the render function for the object
// ----------------------------------------------------------------------------
{
    ((TextureMapping *)arg)->Draw();
}


void TextureMapping::identify_callback(void *arg)
// ----------------------------------------------------------------------------
//   Identify callback: don't do anything
// ----------------------------------------------------------------------------
{
    (void) arg;
}


void TextureMapping::delete_callback(void *arg)
// ----------------------------------------------------------------------------
//   Delete callback: destroy object
// ----------------------------------------------------------------------------
{
    delete (TextureMapping *)arg;
}


void TextureMapping::Draw()
// ----------------------------------------------------------------------------
//   Draw texture mapping
// ----------------------------------------------------------------------------
{
}


void TextureMapping::checkGLContext()
// ----------------------------------------------------------------------------
//   Re-create context-dependent resources if GL context has changed
// ----------------------------------------------------------------------------
{
    tao->makeGLContextCurrent();
    if (*pcontext != QGLContext::currentContext())
    {
        IFTRACE(mapping)
                debug() << "Context has changed" << "\n";

        *pcontext = QGLContext::currentContext();
        createShaders();
    }
}


void TextureMapping::createShaders()
// ----------------------------------------------------------------------------
//   Create shader programs for the material
// ----------------------------------------------------------------------------
{
}


std::ostream & TextureMapping::debug()
// ----------------------------------------------------------------------------
//   Convenience method to log with a common prefix
// ----------------------------------------------------------------------------
{
    std::cerr << "[Mapping] " << (void*)this << " ";
    return std::cerr;
}

