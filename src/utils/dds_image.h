#pragma once

// System Headers
#include <glad/glad.h>

// Standard Headers
#include <iostream>
#include <cstring>
#include <string>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII


class DDSData
{
  public:

    GLuint height, width, linearSize, mipMapCount, fourCC, format, bufsize;

    unsigned char *buffer;

    DDSData(const char *path) {
        FILE *fp;
        fp = fopen(path, "rb");
        if (!fp)
            throw "Cannot open DDS texture: " + std::string(path);

        char fileCode[4];
        size_t s = fread(fileCode, 1, 4, fp);
        if (std::strncmp(fileCode, "DDS ", 4) != 0)
        {
            fclose(fp);
            throw "File is not a DDS texture: " + std::string(path);
        }
        unsigned char header[124];
        s = fread(&header, 124, 1, fp);

        height = *(unsigned int *)&(header[8]);
        width = *(unsigned int *)&(header[12]);
        linearSize = *(unsigned int *)&(header[16]);
        mipMapCount = *(unsigned int *)&(header[24]);
        fourCC = *(unsigned int *)&(header[80]);

        /* how big is it going to be including all mipmaps? */
        bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
        buffer = new unsigned char[bufsize];
        s = fread(buffer, 1, bufsize, fp);
        fclose(fp);

        switch (fourCC)
        {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            delete[] buffer;
            throw "DDS texture is not DXT1 DXT3 or DXT5: " + std::string(path);
        }
    }

    ~DDSData() {
        delete[] buffer;
    }
};