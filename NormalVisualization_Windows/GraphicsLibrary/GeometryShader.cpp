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
#include "GeometryShader.h"
#include <gl/glew.h>

namespace sgpu {

GeometryShader::GeometryShader() {
    this->geometryId = 0;
}

GeometryShader::~GeometryShader() {
    glDeleteShader(this->geometryId);
}

bool GeometryShader::load(const std::string& vertexFilename, const std::string& geometryFilename, const std::string& fragmentFilename) {
    if ( !this->loadFile(vertexFilename, this->vertSource) ) return false;
    if ( !this->loadFile(fragmentFilename, this->fragSource) ) return false;
    if ( !this->loadFile(geometryFilename, this->geomSource) ) return false;

    this->vertFilename = vertexFilename;
    this->fragFilename = fragmentFilename;
    this->geomFilename = geometryFilename;

    this->vertexId = glCreateShader(GL_VERTEX_SHADER);
    this->fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    this->geometryId = glCreateShader(GL_GEOMETRY_SHADER);

    const char* vsource_cstr = this->vertSource.c_str();
    const char* fsource_cstr = this->fragSource.c_str();
    const char* gsource_cstr = this->geomSource.c_str();

    glShaderSource(this->vertexId, 1, &vsource_cstr, 0);
    glShaderSource(this->fragmentId, 1, &fsource_cstr, 0);
    glShaderSource(this->geometryId, 1, &gsource_cstr, 0);
    return true;
}

bool GeometryShader::compile() {
    glCompileShader(this->vertexId);
    if ( !this->compileStatus(this->vertexId, this->vertFilename) ) return false;

    glCompileShader(this->fragmentId);
    if ( !this->compileStatus(this->fragmentId, this->fragFilename) ) return false;

    glCompileShader(this->geometryId);
    if ( !this->compileStatus(this->geometryId, this->geomFilename) ) return false;
    return true;
}

bool GeometryShader::link() {
    this->programId = glCreateProgram();
    glAttachShader(this->programId, this->vertexId);
    glAttachShader(this->programId, this->geometryId);
    glAttachShader(this->programId, this->fragmentId);
    glLinkProgram(this->programId);
    
    if ( !this->linkStatus(this->programId) ) return false;
    return true;
}

}
