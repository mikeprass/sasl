#include "texture.h"

#include "utils.h"
#include "luna.h"
#include "avionics.h"
#include "SOIL.h"


using namespace xa;



Texture::Texture(int id, int width, int height, TextureManager *manager):
    id(id), width(width), height(height), manager(manager)
{
}

Texture::~Texture()
{
    manager->getGraphics()->free_texture(manager->getGraphics(), id);
}



TexturePart::TexturePart(Texture *tex, double x1, double y1, 
        double x2, double y2)
{
    texture = tex;
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
}



TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    unloadAll();
}



Texture* TextureManager::loadImage(const std::string &fileName)
{
    TexturesMap::iterator i = cache.find(fileName);
    if (i != cache.end()) {
        return (*i).second;
    } else {
        int width, height;
        int id = graphics->load_texture(graphics, fileName.c_str(), 
                &width, &height);
        if (-1 == id)
            return NULL;
        
        Texture *tex = new Texture(id, width, height, this);
        cache[fileName] = tex;
        return tex;
    }
}


std::string TextureManager::getPartName(const std::string& fileName, 
        double x, double y, double width, double height)
{
    return fileName + ":" + toString(x) + ";" + toString(y) + ";" +
        toString(width) + ":" + toString(height);
}

void TextureManager::getPartCoords(Texture *texture, double &x1, double &y1,
        double &x2, double &y2)
{
    x1 = y1 = 0.0;
    x2 = y2 = 1.0;
}
        
void TextureManager::getPartCoords(Texture *texture, 
        double width, double height,
        double &x1, double &y1, double &x2, double &y2)
{
    double imageWidth = texture->getWidth();
    double imageHeight = texture->getHeight();
    x1 = ((imageWidth - width) / 2.0) / imageWidth;
    y1 = ((imageHeight - height) / 2.0) / imageHeight;
    x2 = ((imageWidth + width) / 2.0) / imageWidth;
    y2 = ((imageHeight + height) / 2.0) / imageHeight;
}


void TextureManager::getPartCoords(Texture *texture, 
        double x, double y, double width, double height,
        double &x1, double &y1, double &x2, double &y2)
{
    double imageWidth = texture->getWidth();
    double imageHeight = texture->getHeight();
    x1 = x / imageWidth;
    y1 = y / imageHeight;
    x2 = (x + width) / imageWidth;
    y2 = (y + height) / imageHeight;
}

TexturePart* TextureManager::getTexturePart(const std::string &fileName, 
        Texture *texture, double x1, double y1, double x2, double y2)
{
    std::string partName = getPartName(fileName, x1, y1, x2, y2);
    TexturesParts::iterator i = parts.find(partName);
    if (i != parts.end())
        return (*i).second;
    else {
        TexturePart *part = new TexturePart(texture, x1, y1, x2, y2);
        parts[partName] = part;
        return part;
    }
}

TexturePart* TextureManager::load(const std::string &fileName, 
        double x, double y, double width, double height)
{
    Texture *tex = loadImage(fileName);
    if (! tex)
        return NULL;
    else {
        double x1, y1, x2, y2;
        getPartCoords(tex, x, y, width, height, x1, y1, x2, y2);
        return getTexturePart(fileName, tex, x1, y1, x2, y2);
    }
}

TexturePart* TextureManager::load(const std::string &fileName, 
        double width, double height)
{
    Texture *tex = loadImage(fileName);
    if (! tex)
        return NULL;
    else {
        double x1, y1, x2, y2;
        getPartCoords(tex, width, height, x1, y1, x2, y2);
        return getTexturePart(fileName, tex, x1, y1, x2, y2);
    }
}

TexturePart* TextureManager::load(const std::string &fileName)
{
    Texture *tex = loadImage(fileName);
    if (! tex)
        return NULL;
    else {
        double x1, y1, x2, y2;
        getPartCoords(tex, x1, y1, x2, y2);
        return getTexturePart(fileName, tex, x1, y1, x2, y2);
    }
}


void TextureManager::unloadAll()
{
    for (TexturesParts::iterator i = parts.begin(); i != parts.end(); i++)
        delete (*i).second;
    parts.clear();

    for (TexturesMap::iterator i = cache.begin(); i != cache.end(); i++)
        delete (*i).second;
    cache.clear();
}

void TextureManager::setGraphicsCallbacks(struct SaslGraphicsCallbacks *callbacks)
{
    graphics = callbacks;
}

static int luaGetImageSize(lua_State *L)
{
    std::string fileName = lua_tostring(L, 1);
    
    int w,h,ch;
    unsigned char* foo = SOIL_load_image(fileName.c_str(), &w, &h, &ch, SOIL_LOAD_AUTO);
    if (foo)
    {
        SOIL_free_image_data(foo);
    } 
    else
    {
        w = 0;
        h = 0;
    }

    lua_pushnumber(L, w);
    lua_pushnumber(L, h);
    return 2;
}


/// Lua wrapper for texture manager
static int luaLoadImage(lua_State *L)
{
    TextureManager *textureManager = getAvionics(L)->getTextureManager();

    std::string fileName = lua_tostring(L, 1);
    TexturePart *texture = NULL;

    if (lua_isnil(L, 2)) 
        texture = textureManager->load(fileName);
    else if (lua_isnil(L, 4))
        texture = textureManager->load(fileName, 
                lua_tonumber(L, 2), lua_tonumber(L, 3));
    else
        texture = textureManager->load(fileName, 
                lua_tonumber(L, 2), lua_tonumber(L, 3),
                lua_tonumber(L, 4), lua_tonumber(L, 5));
    
    if (texture)
        lua_pushlightuserdata(L, texture);
    else
        lua_pushnil(L);
    return 1;
}

/// Returns size of texture in pixels
static int luaGetTextureSize(lua_State *L)
{
    if ((! lua_islightuserdata(L, 1) || lua_isnil(L, 1)))
        return 0;
    TexturePart *tex = (TexturePart*)lua_touserdata(L, 1);

    lua_pushnumber(L, (tex->getX2() - tex->getX1()) * 
            tex->getTexture()->getWidth());
    lua_pushnumber(L, (tex->getY2() - tex->getY1()) * 
            tex->getTexture()->getHeight());
    return 2;
}


void xa::exportTextureToLua(Luna &lua)
{
    lua_State *L = lua.getLua();

    lua_register(L, "getGLTexture", luaLoadImage);
    lua_register(L, "getTextureSize", luaGetTextureSize);
    lua_register(L, "getImageSize", luaGetImageSize);
}
