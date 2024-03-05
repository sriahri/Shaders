/*
 * Copyright (c) 2024 University of Colorado [http://graphics.ucdenver.edu]
 * Computer VR and Graphics Laboratory
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef VERTEX_H
#define VERTEX_H

#include <Vector3.h>
#include <unordered_map>
#include "Color3.h"

namespace sgpu {

struct Vertex {
    float operator () (const Vertex& v) const {
        float ret = v.position.x() + v.position.y() + v.position.z();
        ret += v.normal.x() + v.normal.y() + v.normal.z();
        ret += v.tangent.x() + v.tangent.y() + v.tangent.z() + v.tangent.w();
        ret += v.textureCoord.x() + v.textureCoord.y();
        return ret;
    }

    bool operator () (const Vertex& u, const Vertex& v) const {
        if ( u.position == v.position &&
             u.normal == v.normal &&
             u.tangent == v.tangent &&
             u.textureCoord == v.textureCoord ) return true;
        return false;
    }

    Vector3f position;
    Vector3f normal;
    Vector4f tangent;
    Vector3f textureCoord;
    Color3f color;
};

typedef std::unordered_map<Vertex, unsigned int, Vertex, Vertex> VertexSet;

}

#endif
