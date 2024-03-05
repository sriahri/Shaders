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
#ifndef SHADER_H
#define SHADER_H

#include <memory>
#include <string>
#include <Matrix4.h>
#include "Texture.h"
#include "Color3.h"
#include "Color4.h"

namespace sgpu {

class Shader {
public:
    Shader();
    Shader(const Shader& shader);
    ~Shader();

    bool load(const std::string& vertexFilename, const std::string& fragmentFilename);
    bool compile();
    bool link();

	bool loadDiffuseTexture(const std::string& filename, bool textureIndex);
	bool loadNormalTexture(const std::string& filename, bool textureIndex);
	bool loadSpecularTexture(const std::string& filename, bool textureIndex);
	bool loadHeightmapTexture(const std::string& filename, bool textureIndex);

    bool enable();
    bool disable();

    operator unsigned int () const;
    unsigned int getProgramID() const;
    unsigned int id() const;

    void uniform1f(const std::string& name, float value) const;
	void uniform2f(const std::string& name, float value0, float value1) const;
	void uniform3f(const std::string& name, float value0, float value1, float value2) const;
	void uniform4f(const std::string& name, float value0, float value1, float value2, float value3) const;
	void uniform1i(const std::string& name, int value) const;
	void uniform2i(const std::string& name, int value0, int value1) const;
	void uniform3i(const std::string& name, int value0, int value1, int value2) const;
	void uniform4i(const std::string& name, int value0, int value1, int value2, int value3) const;
	void uniform4fv(const std::string& name, unsigned int count, const float* values) const;
    void uniformMatrix(const std::string& name, const Matrix4f& matrix) const;
    void uniformMatrix(const std::string& name, const Matrix3f& matrix) const;
    void uniformVector(const std::string& name, const Vector3f& vector) const;
    void uniformVector(const std::string& name, const Vector4f& vector) const;
	void uniformColor(const std::string& name, const Color3f& color) const;
	void uniformColor(const std::string& name, const Color4f& color) const;

protected:
    bool loadFile(const std::string& filename, std::string& content);
    bool compileStatus(unsigned int shaderId, const std::string& filename) const;
    bool linkStatus(unsigned int programId) const;

protected:
    unsigned int programId;
    unsigned int vertexId;
    unsigned int fragmentId;

	/* 2D Texture data of this mesh */
	std::shared_ptr<Texture> diffuseTexture0;
	std::shared_ptr<Texture> normalTexture0;
	std::shared_ptr<Texture> specularTexture0;
	std::shared_ptr<Texture> heightmapTexture0;

	/*
	 * 2D Secondary Texture
	 *
	 * This can be extended to multiple textures (vector, array, etc.) assuming the
	 * underlying hardware contains an adequate number of texture units.
	 */
	std::shared_ptr<Texture> diffuseTexture1;
	std::shared_ptr<Texture> normalTexture1;
	std::shared_ptr<Texture> specularTexture1;
	std::shared_ptr<Texture> heightmapTexture1;

    /* Shader File Info */
    std::string vertFilename;
    std::string fragFilename;
    std::string vertSource;
    std::string fragSource;
};

}

#endif
