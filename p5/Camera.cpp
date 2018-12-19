//[]---------------------------------------------------------------[]
//|                                                                 |
//| Copyright (C) 2018 Orthrus Group.                               |
//|                                                                 |
//| This software is provided 'as-is', without any express or       |
//| implied warranty. In no event will the authors be held liable   |
//| for any damages arising from the use of this software.          |
//|                                                                 |
//| Permission is granted to anyone to use this software for any    |
//| purpose, including commercial applications, and to alter it and |
//| redistribute it freely, subject to the following restrictions:  |
//|                                                                 |
//| 1. The origin of this software must not be misrepresented; you  |
//| must not claim that you wrote the original software. If you use |
//| this software in a product, an acknowledgment in the product    |
//| documentation would be appreciated but is not required.         |
//|                                                                 |
//| 2. Altered source versions must be plainly marked as such, and  |
//| must not be misrepresented as being the original software.      |
//|                                                                 |
//| 3. This notice may not be removed or altered from any source    |
//| distribution.                                                   |
//|                                                                 |
//[]---------------------------------------------------------------[]
//
// OVERVIEW: Camera.cpp
// ========
// Source file for camera.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 15/09/2018

#include "Camera.h"

namespace cg
{ // begin namespace cg


//////////////////////////////////////////////////////////
//
// Camera implementation
// ======
uint32_t Camera::_nextId;

inline const char*
Camera::defaultName()
{
  static char name[16];

  snprintf(name, sizeof name, "Camera %d", ++_nextId);
  return name;
}

Camera::Camera():
  NameableObject{defaultName()}
{
  setDefaultView();
}
//****************************************************************************
	//****************************************************************************
	//****************************************************************************
	//****************************************************************************
	//****************************************************************************

void Camera::setInverseMatrix()
{
	_inverseMatrix.set(_matrix);
	_inverseMatrix.invert();
}

inline void Camera::setMatrix() {

	auto r = mat3f{ _rotation };
	auto u = r[0];
	auto v = r[1];
	auto n = r[2];

	auto c = -_position;

	vec4f fColumn, sColumn, tColumn, fthColumn;

	fColumn.set(u[0], v[0], n[0], 0);
	sColumn.set(u[1], v[1], n[1], 0);
	tColumn.set(u[2], v[2], n[2], 0);
	fthColumn.set(c.dot(u), c.dot(v), c.dot(n), 1);

	_matrix.set(fColumn, sColumn, tColumn, fthColumn);

	setInverseMatrix();
}

void
Camera::setProjectionMatrix()
{
	if (_projectionType == Perspective) {
		_projectionMatrix = mat4f::perspective(_viewAngle, _aspectRatio, _F, _B);
	}
	else if (_projectionType == Parallel) {
		_projectionMatrix = mat4f::ortho(-((_aspectRatio * _height) / 2), -(_height / 2), (_aspectRatio * _height) / 2, _height / 2, _F, _B);
	}
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************

void
Camera::setPosition(const vec3f& value)
//[]---------------------------------------------------[]
//|  Set the camera's position                          |
//|                                                     |
//|  Setting the camera's position will not change      |
//|  neither the direction of projection nor the        |
//|  distance between the position and the focal point. |
//|  The focal point will be moved along the direction  |
//|  of projection.                                     |
//[]---------------------------------------------------[]
{
	auto fx = value.x - _position.x;
	auto fy = value.y - _position.y;
	auto fz = value.z - _position.z;
	_focalPoint.set(fx + _focalPoint.x, fy + _focalPoint.y, fz + _focalPoint.z);
	_position.set(value);
	setMatrix();
}

void
Camera::setEulerAngles(const vec3f& value)
{
	_eulerAngles = value;
	setRotation(quatf::eulerAngles(value));
}

void
Camera::setRotation(const quatf& value)
{
	_rotation = value;
	_focalPoint = _rotation.rotate(focalPoint());
	_vup = _rotation.rotate(_vup).versor();
	setMatrix();
}

void
Camera::setProjectionType(ProjectionType value)
//[]---------------------------------------------------[]
//|  Set the camera's projection type                   |
//[]---------------------------------------------------[]
{
	_projectionType = value;
	setProjectionMatrix();
}

void
Camera::setDistance(float value)
//[]---------------------------------------------------[]
//|  Set the camera's distance                          |
//|                                                     |
//|  Setting the distance between the position and      |
//|  focal point will move the focal point along the    |
//|  direction of projection.                           |
//[]---------------------------------------------------[]
{
	_distance = value;
}

void
Camera::setViewAngle(float value)
//[]---------------------------------------------------[]
//|  Set the camera's view angle                        |
//[]---------------------------------------------------[]
{
	_viewAngle = value;
	setProjectionMatrix();
}

void
Camera::setHeight(float value)
//[]---------------------------------------------------[]
//|  Set the camera's view_height                       |
//[]---------------------------------------------------[]
{
	_height = value;
	setProjectionMatrix();
}

void
Camera::setAspectRatio(float value)
//[]---------------------------------------------------[]
//|  Set the camera's aspect ratio                      |
//[]---------------------------------------------------[]
{
	_aspectRatio = value;
	setProjectionMatrix();
}

void
Camera::setClippingPlanes(float F, float B)
//[]---------------------------------------------------[]
//|  Set the distance of the clippling planes           |
//[]---------------------------------------------------[]
{
	_F = F;
	_B = B;
	setProjectionMatrix();
}

void
Camera::rotateYX(float ay, float ax, bool orbit)
//[]---------------------------------------------------[]
//|  Rotate YX                                          |
//|                                                     |
//|  If orbit is true, then it is a composition of an   |
//|  azimuth of ay with an elevation of ax, in this     |
//|  order. Otherwise, it is a composition of a yaw of  |
//|  ay with a pitch of ax, in this order.              |
//[]---------------------------------------------------[]
{
	quatf qy(ay, _vup);
	vec3f u = (focalPoint() - position()).versor().cross(_vup);

	u = qy.rotate(u).versor();

	quatf qx(ax, u);
	_vup = qx.rotate(_vup).versor();

	vec3f dop = _vup.cross(u).versor();

	if (orbit) {
		_position = _focalPoint - dop * _distance;
	}
	else {
		_focalPoint = _position - dop * _distance;
	}

	setMatrix();
}

void
Camera::zoom(float zoom)
//[]---------------------------------------------------[]
//|  Zoom                                               |
//|                                                     |
//|  Change the view angle (or height) of the camera so |
//|  that more or less of a scene occupies the view     |
//|  window. A value > 1 is a zoom-in. A value < 1 is   |
//|  zoom-out.                                          |
//[]---------------------------------------------------[]
{
	if (_projectionType == Parallel) {
		_height /= zoom;
	}
	else if (_projectionType == Perspective) {
		_viewAngle /= zoom;
	}
	setProjectionMatrix();
}

void
Camera::translate(float dx, float dy, float dz)
//[]---------------------------------------------------[]
//|  Translate the camera                               |
//[]---------------------------------------------------[]
{
	vec3f delta = focalPoint() - position();

	_position.x += dx;
	_position.y += dy;
	_position.z += dz;

	_focalPoint = _position + delta;
	setMatrix();
}

void
Camera::setDefaultView(float aspect)
//[]---------------------------------------------------[]
//|  Set default view                                   |
//[]---------------------------------------------------[]
{
  _position.set(0.0f, 5.0f, 10.0f);
  _eulerAngles.set(0.0f);
  _rotation = quatf::identity();
  _focalPoint.set(0.0f, 0.0f, 0.0f);
  _distance = 10.0f;
  _aspectRatio = aspect;
  _projectionType = Perspective;
  _viewAngle = 60.0f;
  _height = 10.0f;
  _F = 0.01f;
  _B = 1000.0f;
}

void
Camera::update()
{
  _matrix = mat4f::lookAt(_position, _focalPoint, vec3f::up());
  _matrix.inverse(_inverseMatrix);
  _projectionMatrix = mat4f::perspective(_viewAngle, _aspectRatio, _F, _B);
}

} // end namespace cg
