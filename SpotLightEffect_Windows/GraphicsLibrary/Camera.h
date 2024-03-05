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
#ifndef CAMERA_H
#define CAMERA_H

#include <Mathematics.h>
#include <Matrix4.h>

namespace sgpu {

template <typename Real>
class Camera {
public:
    Camera(Real radius = Real(1));
    Camera(const Camera<Real>& camera);
    virtual ~Camera();

    void rotate(Real dTheta, Real dPhi);

    void setPerspective(Real fov, Real aspectRatio, Real znear, Real zfar);
    void setFrustum(Real left, Real right, Real top, Real bottom, Real znear, Real zfar);

    void setRotation(Real theta, Real phi);
    void setPosition(Real r, Real theta, Real phi);
    void setPosition(const Vector3<Real>& position);
    void setRadius(Real radius);
    void addRadius(Real dRadius);

    /*
     * Constructs a rotation matrix based on the three linearly indpendent
     * vectors that compose the cameras view direction, right vector, and
     * up direction. These three orthogonal vectors are composed into a 
     * single rotation matrix.
     */
    Matrix3<Real> getBasisMatrix(bool colMajor = true) const;
    Matrix4<Real> toViewMatrix() const;
    Matrix4<Real> toProjectionMatrix() const;
    Vector3<Real> toCartesianCoordinates() const;
    Vector3<Real> toSphericalCoordinates() const;

    Vector3<Real> getEyeDirection() const;
    Vector3<Real> getUpDirection() const;
    Vector3<Real> getRightDirection() const;

    Matrix4<Real>& getViewMatrix();
    Matrix4<Real>& getProjectionMatrix();
    Real& getRadius();
    Vector3<Real>& getEye();
    Vector3<Real>& getLookAt();
    Vector3<Real>& getUp();
    Vector3<Real>& getRight();

    const Matrix4<Real>& getViewMatrix() const;
    const Matrix4<Real>& getProjectionMatrix() const;

    const Real& getRadius() const;
    const Vector3<Real>& getEye() const;
    const Vector3<Real>& getLookAt() const;
    const Vector3<Real>& getUp() const;
    const Vector3<Real>& getRight() const;

protected:
    void compile();

protected:
    Matrix4<Real> view;
    Matrix4<Real> projection;

    Vector3<Real> eye;
    Vector3<Real> lookAt;

    Vector3<Real> up;
    Vector3<Real> right;
    Vector3<Real> dir;

    Real r, theta, phi;
};

template <typename Real>
Camera<Real>::Camera(Real radius) {
    this->r = radius;
    this->theta = Real(0);
    this->phi = Real(HALF_PI);

    this->eye = Vector3<Real>::UnitZ();
    this->lookAt = Vector3<Real>::Zero();
    this->up = Vector3<Real>::UnitY();

    this->compile();
}

template <typename Real>
Camera<Real>::Camera(const Camera<Real>& camera) {
    this->view = camera.view;
    this->eye = camera.eye;
    this->lookAt = camera.lookAt;
    this->up = camera.up;
    this->r = camera.r;
    this->theta = camera.theta;
    this->phi = camera.phi;
}

template <typename Real>
Camera<Real>::~Camera() {}

template <typename Real>
void Camera<Real>::rotate(Real dTheta, Real dPhi) {
    this->theta += dTheta;
    this->phi += dPhi;
    this->compile();
}

template <typename Real>
void Camera<Real>::setPerspective(Real fov, Real aspectRatio, Real znear, Real zfar) {
    Real ymax = znear * std::tan(fov * Real(PI) / Real(360));
    Real xmax = ymax * aspectRatio;
    this->setFrustum(-xmax, xmax, ymax, -ymax, znear, zfar);
}

template <typename Real>
void Camera<Real>::setFrustum(Real left, Real right, Real top, Real bottom, Real znear, Real zfar) {
    this->projection.zero();

    Real z, width, height, d;
    z = Real(2) * znear;
    width = right - left;
    height = top - bottom;
    d = zfar - znear;

    this->projection[0] = z / width;
    this->projection[5] = z / height;
    this->projection[8] = (right + left) / width;
    this->projection[9] = (top + bottom) / height;
    this->projection[10] = (-zfar - znear) / d;
    this->projection[11] = Real(-1);
    this->projection[14] = (-z * zfar) / d;
}

template <typename Real>
void Camera<Real>::setRotation(Real theta, Real phi) {
    this->theta = theta;
    this->phi = phi;
    this->compile();
}

template <typename Real>
void Camera<Real>::setPosition(Real r, Real theta, Real phi) {
    this->r = r;
    this->theta = theta;
    this->phi = phi;
    this->compile();
}

template <typename Real>
void Camera<Real>::setPosition(const Vector3<Real>& position) {
    this->r = position.x();
    this->theta = position.y();
    this->phi = position.z();
    this->compile();
}

template <typename Real>
void Camera<Real>::setRadius(Real radius) {
    this->r = radius;
    this->compile();
}

template <typename Real>
void Camera<Real>::addRadius(Real dRadius) {
    this->r += dRadius;
    this->compile();
}

template <typename Real>
Matrix3<Real> Camera<Real>::getBasisMatrix(bool colMajor) const {
    Matrix3<Real> matrix;

    if ( colMajor ) {
        matrix.setColumn(0, this->dir);
        matrix.setColumn(1, this->up);
        matrix.setColumn(2, this->right);
    }
    else {
        matrix.setRow(0, this->dir);
        matrix.setRow(1, this->up);
        matrix.setRow(2, this->right);
    }

    return matrix;
}

template <typename Real>
Matrix4<Real> Camera<Real>::toViewMatrix() const {
    this->compile();
    return this->view;
}

template <typename Real>
Matrix4<Real> Camera<Real>::toProjectionMatrix() const {
    return this->projection;
}

template <typename Real>
Vector3<Real> Camera<Real>::toCartesianCoordinates() const {
    return SphereicalToCartesian<Real>(this->r, this->theta, this->phi);
}

template <typename Real>
Vector3<Real> Camera<Real>::toSphericalCoordinates() const {
    return Vector3<Real>(this->r, this->theta, this->phi);
}

template <typename Real>
Vector3<Real> Camera<Real>::getEyeDirection() const {
    return Vector3<Real>(this->eye, this->lookAt);
}

template <typename Real>
Vector3<Real> Camera<Real>::getUpDirection() const {
    return this->up;
}

template <typename Real>
Vector3<Real> Camera<Real>::getRightDirection() const {
    return this->right;
}

template <typename Real>
Matrix4<Real>& Camera<Real>::getViewMatrix() {
    return this->view;
}

template <typename Real>
Matrix4<Real>& Camera<Real>::getProjectionMatrix() {
    return this->projection;
}

template <typename Real>
Real& Camera<Real>::getRadius() {
    return this->r;
}

template <typename Real>
Vector3<Real>& Camera<Real>::getEye() {
    return this->eye;
}

template <typename Real>
Vector3<Real>& Camera<Real>::getLookAt() {
    return this->lookAt;
}

template <typename Real>
Vector3<Real>& Camera<Real>::getUp() {
    return this->up;
}

template <typename Real>
Vector3<Real>& Camera<Real>::getRight() {
    return this->right;
}

template <typename Real>
const Matrix4<Real>& Camera<Real>::getViewMatrix() const {
    return this->view;
}

template <typename Real>
const Matrix4<Real>& Camera<Real>::getProjectionMatrix() const {
    return this->projection;
}

template <typename Real>
const Real& Camera<Real>::getRadius() const {
    return this->r;
}

template <typename Real>
const Vector3<Real>& Camera<Real>::getEye() const {
    this->compile();
    return this->eye;
}

template <typename Real>
const Vector3<Real>& Camera<Real>::getLookAt() const {
    this->compile();
    return this->lookAt;
}

template <typename Real>
const Vector3<Real>& Camera<Real>::getUp() const {
    this->compile();
    return this->up;
}


template <typename Real>
const Vector3<Real>& Camera<Real>::getRight() const {
    this->compile();
    return this->right;
}

template <typename Real>
void Camera<Real>::compile() {
    this->eye = SphereicalToCartesian<Real>(this->r, this->theta, this->phi);
    this->up = -SphereicalToCartesian_dPhi<Real>(this->r, this->theta, this->phi);
    this->right = SphereicalToCartesian_dTheta<Real>(this->r, this->theta, this->phi);
    this->dir = SphereicalToCartesian_dPhiCrossdTheta(this->r, this->theta, this->phi);

    //--------------------------------------------------------------------------
    // Sphereical coordinates use a z-up axis, therefore we have to swap
    // the y and z components of the position.
    //--------------------------------------------------------------------------
    this->eye.swapYZ();
    this->right.swapYZ();
    this->up.swapYZ();
    this->dir.swapYZ();

    //--------------------------------------------------------------------------
    // Normalize these vectors to form a basis.
    //--------------------------------------------------------------------------
    this->right.normalize();
    this->up.normalize();
    this->dir.normalize();

    this->view = Matrix4<Real>::LookAt(this->eye, this->lookAt, this->up);
}

typedef Camera<float> Cameraf;
typedef Camera<double> Camerad;

}

#endif
