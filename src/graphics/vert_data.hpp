#pragma once

// Local Headers
#include "vert_attributes.hpp"

typedef unsigned char u_char;

class VertData
{
  public:
    VertData(VertAttributes attrs, std::vector<u_char> vertices)  
    : attributes(std::move(attrs)), vertices(std::move(vertices))
    {}

    VertAttributes attributes;
    std::vector<u_char> vertices;

    template <class type>
    inline type get(int vertI, int attrOffset)
    {
        type v;
        memcpy(
                &v,
                &(vertices[vertI * attributes.getVertSize() + attrOffset]),
                sizeof(type)
        );
        return v;
    }

    template <class type>
    inline void set(const type &v, int vertI, int attrOffset)
    {
        memcpy(
                &(vertices[vertI * attributes.getVertSize() + attrOffset]),
                &v,
                sizeof(type)
        );
    }

    template <class type>
    inline void add(const type &v, int vertI, int attrOffset)
    {
        set(get<type>(vertI, attrOffset) + v, vertI, attrOffset);
    }

    template <class vecType>
    void normalizeVecAttribute(int attrOffset)
    {
        for (unsigned int vertI = 0; vertI < ((unsigned int) vertices.size()) / attributes.getVertSize(); vertI++)
            set(normalize(get<vecType>(vertI, attrOffset)), vertI, attrOffset);
    }

    void removeVertices(int count);

    void addVertices(int count);
};
