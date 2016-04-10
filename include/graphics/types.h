#ifndef ROGUELIKE_INCLUDED_GRAPHICS_TYPES
#define ROGUELIKE_INCLUDED_GRAPHICS_TYPES

#include <graphics/glfwwindow.h>
#include <graphics/gltexture.h>
#include <graphics/glsprite.h>
#include <graphics/glrenderctx.h>
#include <graphics/gltilemap.h>

namespace graphics {
    typedef GLFWWindow Window;
    typedef GLTexture Texture;
    typedef GLSprite Sprite;
    typedef GLRenderContext RenderContext;
    typedef GLTilemap Tilemap;
}

#endif
