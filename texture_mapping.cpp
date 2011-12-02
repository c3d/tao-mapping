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
//    Texture Mapping
//
// ============================================================================

TextureMapping::TextureMapping()
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
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

void TextureMapping::delete_callback(void *arg)
// ----------------------------------------------------------------------------
//   Delete callback: destroy object
// ----------------------------------------------------------------------------
{
    delete (TextureMapping *)arg;
}

void TextureMapping::Draw()
{
}

