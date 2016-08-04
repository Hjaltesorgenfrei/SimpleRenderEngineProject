#pragma once

#include <cstdint>

namespace SRE{
class Texture {
public:
    static Texture* createTextureFromJPEG(const char* data, int size, bool generateMipmaps = false);
    static Texture* createTextureFromPNG(const char* data, int size, bool generateMipmaps = false);
    static Texture* createTextureFromRGBA(const char* data, int width, int height, bool generateMipmaps = false);
    static Texture* getWhiteTexture();
    int getWidth();
    int getHeight();
    // returns true if texture sampling should be filtered (bi-linear or tri-linear sampling) otherwise use point sampling.
    bool isFilterSampling();
    // if true texture sampling is filtered (bi-linear or tri-linear sampling) otherwise use point sampling.
    void setFilterSampling(bool enable);
    bool isWrapTextureCoordinates();
    void setWrapTextureCoordinates(bool enable);
private:
    Texture(const char* rgba, int width, int height, uint32_t type,bool generateMipmaps);
    void updateTextureSampler();
    static Texture* whiteTexture;
    int width;
    int height;
    bool generateMipmap;
    bool filterSampling = true; // true = linear/trilinear sampling, false = point sampling
    bool wrapTextureCoordinates = true;
    unsigned int textureId;

};
}