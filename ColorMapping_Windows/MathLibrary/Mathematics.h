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
#ifndef MATHEMATICS_H
#define MATHEMATICS_H

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace sgpu {

static const double PI = 3.14159265;
static const double HALF_PI = PI * 0.5;

/* R(r, t, p) = rsin(phi)cos(theta)i + rsin(phi)sin(theta)j + rcos(phi)k */
template <typename Real>
inline Vector3<Real> SphereicalToCartesian(Real r, Real theta, Real phi) {
    Vector3<Real> result;
    result.x() = r * std::sin(phi) * std::cos(theta);
    result.y() = r * std::sin(phi) * std::sin(theta);
    result.z() = r * std::cos(phi);
    return result;
}

/* Rt(r, t, p) = -rsin(pphi)sin(theta)i + rsin(phi)cos(theta)j + 0k */
template <typename Real>
inline Vector3<Real> SphereicalToCartesian_dTheta(Real r, Real theta, Real phi) {
    Vector3<Real> result;
    result.x() = -r * std::sin(phi) * std::sin(theta);
    result.y() = r * std::sin(phi) * std::cos(theta);
    result.z() = Real(0);
    return result;
}

/* Rp(r, t, p) = rcos(phi)cos(theta)i + rcos(phi)sin(theta)j - rsin(phi)k */
template <typename Real>
inline Vector3<Real> SphereicalToCartesian_dPhi(Real r, Real theta, Real phi) {
    Vector3<Real> result;
    result.x() = r * std::cos(phi) * std::cos(theta);
    result.y() = r * std::cos(phi) * std::sin(theta);
    result.z() = -r * std::sin(phi);
    return result;
}

/* Rp X Rt = r^2 * sin^2(phi)cos(theta)i + r^2 * sin^2(phi)sin(theta)j + r^2 * sin(phi)cos(phi)k */
template <typename Real>
inline Vector3<Real> SphereicalToCartesian_dPhiCrossdTheta(Real r, Real theta, Real phi) {
    Vector3<Real> result;
    Real rs = (r*r);
    result.x() = rs * std::pow(std::sin(phi), Real(2)) * std::cos(theta);
    result.y() = rs * std::pow(std::sin(phi), Real(2)) * std::sin(theta);
    result.z() = rs * std::sin(phi) * std::cos(phi);
    return result;
}

}

#endif
