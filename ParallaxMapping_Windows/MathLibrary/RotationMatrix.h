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
#ifndef ROTATION_MATRIX_H
#define ROTATION_MATRIX_H

#include "Matrix3.h"

namespace sgpu {

template <typename Real>
class RotationMatrix : public Matrix3<Real> {
public:
    RotationMatrix();
    ~RotationMatrix();

    /*
     * The inverse of a rotation matrix is greatly simplified due to its
     * orthogonal property. Using this property, the inverse of this orthogonal
     * matrix is simply its transpose. Therefore, the implementations utilize
	 * the transpose function rather than the full matrix inverse function.
     */
    void invert();

    static RotationMatrix<Real> Inverse(const RotationMatrix<Real>& m);
};

template <typename Real>
RotationMatrix<Real>::RotationMatrix() : Matrix3<Real>(true) {}

template <typename Real>
RotationMatrix<Real>::~RotationMatrix() {}

template <typename Real>
void RotationMatrix<Real>::invert() {
    this->transpose();
}

template <typename Real>
RotationMatrix<Real> RotationMatrix<Real>::Inverse(const RotationMatrix<Real>& m) {
    return Matrix3<Real>::Transpose(m);
}

}

#endif
