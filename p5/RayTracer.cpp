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
// OVERVIEW: RayTracer.cpp
// ========
// Source file for simple ray tracer.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 21/11/2018

#include "Camera.h"
#include "RayTracer.h"
#include "math/Vector3.h"
#include <time.h>
#include <math.h>

using namespace std;

namespace cg
{ // begin namespace cg

void
printElapsedTime(const char* s, clock_t time)
{
  printf("%sElapsed time: %.4f s\n", s, (float)time / CLOCKS_PER_SEC);
}


/////////////////////////////////////////////////////////////////////
//
// RayTracer implementation
// =========
RayTracer::RayTracer(Scene& scene, Camera* camera):
  Renderer{scene, camera},
  _maxRecursionLevel{6},
  _minWeight{MIN_WEIGHT}
{
  // TODO: BVH
}

void
RayTracer::render()
{
  throw std::runtime_error("RayTracer::render() invoked");
}

inline float
windowHeight(Camera* c)
{
  if (c->projectionType() == Camera::Parallel)
    return c->height();
  return c->nearPlane() * tan(math::toRadians(c->viewAngle() * 0.5f)) * 2;

}
void
RayTracer::renderImage(Image& image)
{
  auto t = clock();
  const auto& m = _camera->cameraToWorldMatrix();

  // VRC axes
  _vrc.u = m[0];
  _vrc.v = m[1];
  _vrc.n = m[2];
  // init auxiliary mapping variables
  _W = image.width();
  _H = image.height();
  _Iw = math::inverse(float(_W));
  _Ih = math::inverse(float(_H));

  auto height = windowHeight(_camera);

  _W >= _H ? _Vw = (_Vh = height) * _W * _Ih : _Vh = (_Vw = height) * _H * _Iw;
  // init pixel ray
  _pixelRay.origin = _camera->position();
  _pixelRay.direction = -_vrc.n;
  _camera->clippingPlanes(_pixelRay.tMin, _pixelRay.tMax);
  _numberOfRays = _numberOfHits = 0;
  scan(image);
  printf("\nNumber of rays: %llu", _numberOfRays);
  printf("\nNumber of hits: %llu", _numberOfHits);
  printElapsedTime("\nDONE! ", clock() - t);
}

void
RayTracer::setPixelRay(float x, float y)
//[]---------------------------------------------------[]
//|  Set pixel ray                                      |
//|  @param x coordinate of the pixel                   |
//|  @param y cordinates of the pixel                   |
//[]---------------------------------------------------[]
{
  auto p = imageToWindow(x, y);

  switch (_camera->projectionType())
  {
    case Camera::Perspective:
      _pixelRay.direction = (p - _camera->nearPlane() * _vrc.n).versor();
      break;

    case Camera::Parallel:
      _pixelRay.origin = _camera->position() + p;
      break;
  }
}

void
RayTracer::scan(Image& image)
{
  ImageBuffer scanLine{_W, 1};

  for (int j = 0; j < _H; j++)
  {
    auto y = (float)j + 0.5f;

    printf("Scanning line %d of %d\r", j + 1, _H);
    for (int i = 0; i < _W; i++)
      scanLine[i] = shoot((float)i + 0.5f, y);
    image.setData(0, j, scanLine);
  }
}

Color
RayTracer::shoot(float x, float y)
//[]---------------------------------------------------[]
//|  Shoot a pixel ray                                  |
//|  @param x coordinate of the pixel                   |
//|  @param y cordinates of the pixel                   |
//|  @return RGB color of the pixel                     |
//[]---------------------------------------------------[]
{
  // set pixel ray
  setPixelRay(x, y);

  // trace pixel ray
  Color color = trace(_pixelRay, 0, 1.0f);

  // adjust RGB color
  if (color.r > 1.0f)
    color.r = 1.0f;
  if (color.g > 1.0f)
    color.g = 1.0f;
  if (color.b > 1.0f)
    color.b = 1.0f;
  // return pixel color
  return color;
}

Color
RayTracer::trace(const Ray& ray, uint32_t level, float weight)
//[]---------------------------------------------------[]
//|  Trace a ray                                        |
//|  @param the ray                                     |
//|  @param recursion level                             |
//|  @param ray weight                                  |
//|  @return color of the ray                           |
//[]---------------------------------------------------[]
{
  if (level > _maxRecursionLevel)
    return Color::black;
  _numberOfRays++;

  Intersection hit;

  return intersect(ray, hit) ? shade(ray, hit, level, weight) : background();
}

inline constexpr auto
rt_eps()
{
  return 1e-4f;
}

bool 
RayTracer::intersect(const Ray& ray, Intersection& hit)
//[]---------------------------------------------------[]
//|  Ray/object intersection                            |
//|  @param the ray (input)                             |
//|  @param information on intersection (output)        |
//|  @return true if the ray intersects an object       |
//[]---------------------------------------------------[]
{
	hit.object = nullptr;
	hit.distance = ray.tMax;
	Intersection check;
	check.object = nullptr;
	check.distance = ray.tMax;

	for (auto obj : _root->children()) {
		if (obj->primitive() != nullptr) {
			//printf("Nome do filho: %s\n", obj->name());
			if (obj->primitive()->intersect(ray, &check))
				_numberOfHits++;

			if (!obj->children().empty()) {
				for (auto s : obj->children()) {
					s->primitive()->intersect(ray, &check);

					if (check.distance < hit.distance) {
						hit.distance = check.distance;
						hit.object = check.object;
						hit.p = check.p;
						hit.triangleIndex = check.triangleIndex;
					}

				}

			}
		}

		if (check.distance < hit.distance) {
			hit.distance = check.distance;
			hit.object = check.object;
			hit.p = check.p;
			hit.triangleIndex = check.triangleIndex;
		}
		
	}


	return hit.object != nullptr;
}



Color
RayTracer::shade(const Ray& ray, Intersection& hit, int level, float weight)
//[]---------------------------------------------------[]
//|  Shade a point P                                    |
//|  @param the ray (input)                             |
//|  @param information on intersection (input)         |
//|  @param recursion level                             |
//|  @param ray weight                                  |
//|  @return color at point P                           |
//[]---------------------------------------------------[]
{
	if (level > _maxRecursionLevel || weight < _minWeight)
		return Color::black;

	const Primitive* obj = hit.object;
	const Material& m = obj->material;
	Color c = m.ambient * _renderer->scene()->ambientLight;
	auto P = ray(hit.distance);
	vec3f normal = obj->triangleNormal(hit.triangleIndex, hit.p);
	
	for (Light* l = fLight; l != nullptr; l = l->nxtLight()) {
		Ray r;
		vec3f lL;
		float dL;
		int ltype = l->intType();
		if (ltype == 1) {
			lL = P - l->transform()->position();
			dL = lL.length();
			lL *= 1.0f / dL;
			r.direction = -lL;
		} else if (ltype == 0) {
			lL = l->transform()->rotation();
			dL = 1;
			lL *= 1.0f / dL;
			r.direction = -lL;
		}
		
		r.origin = P + r.direction * rt_eps();
		Intersection intersect;
		if (!this->intersect(r, intersect)) {
			Color iL = l->color / pow(dL, l->falloff());
			vec3f rl = reflection(normal, lL);

			c += (m.diffuse * iL * (-normal).dot(lL)) + (m.spot * iL * pow((rl).dot(ray.direction), m.shine));
		}
	}
	Ray r;
	r.direction = reflection(normal, ray.direction);//Dire��o de reflex�o do ponto de interse��o em rela��o a ray
	r.origin = P + r.direction * rt_eps();
	float w = m.specular[0];
	if (m.specular[1] > w)
		w = m.specular[1];
	else if (m.specular[2] > w)
		w = m.specular[2];
	//c += m.specular * trace(r, level + 1, w * weight);
	

	return c;
}

vec3f 
RayTracer::reflection(vec3f normal, vec3f incomingDir) {
	return (incomingDir - 2 * normal.dot(incomingDir) * normal);
}


Color
RayTracer::background() const
//[]---------------------------------------------------[]
//|  Background                                         |
//|  @return background color                           |
//[]---------------------------------------------------[]
{
  return _scene->backgroundColor;
}

bool
RayTracer::shadow(const Ray& ray)
//[]---------------------------------------------------[]
//|  Verifiy if ray is a shadow ray                     |
//|  @param the ray (input)                             |
//|  @return true if the ray intersects an object       |
//[]---------------------------------------------------[]
{
  Intersection hit;
  return intersect(ray, hit);
}

} // end namespace cg
