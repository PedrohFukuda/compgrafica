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
// OVERVIEW: Primitive.cpp
// ========
// Source file for primitive.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 30/10/2018

#include "Primitive.h"
#include "Transform.h"
#include "Intersection.h"
#include "geometry/TriangleMesh.h"
#include "Geometry/Ray.h"

namespace cg
{ // begin namespace cg

	bool Primitive::intersect(const Ray& ray, Intersection* intersect) const
	{
		if (_mesh == nullptr)
			return false;

		auto t = const_cast<Primitive*>(this)->transform();
		Ray localRay{ ray, t->worldToLocalMatrix() };
		auto d = math::inverse(localRay.direction.length());
		float tMin;
		float tMax;

		localRay.direction *= d;
		if (_mesh->bounds().intersect(localRay, tMin, tMax))
		{

			if (tMin >= ray.tMin && tMin <= ray.tMax) {
				return meshIntersection(localRay, intersect, d, true, tMax);
			} 
			if (tMax >= ray.tMin && tMax <= ray.tMax) {
				return meshIntersection(localRay, intersect, d, false, tMax);
			}
		}
		return false;
	}

	bool Primitive::meshIntersection(const Ray& r, Intersection* intersect, float aux, bool min, float tmax) const {
	

		//TODO mesh intersection
		for (int i = 0; i < _mesh->data().numberOfTriangles; i++) {
			vec3f vertices[3];

			//*******************************************************************
			vertices[0] = _mesh->data().vertices[(_mesh->data().triangles + i)->v[0]];
			vertices[1] = _mesh->data().vertices[(_mesh->data().triangles + i)->v[1]];
			vertices[2] = _mesh->data().vertices[(_mesh->data().triangles + i)->v[2]];
			//*******************************************************************

			//Passo 1
			auto e1 = vertices[1] - vertices[0];
			auto e2 = vertices[2] - vertices[0];
			auto s1 = r.direction.cross(e2);
			auto det = s1.dot(e1);
			
			if (det != 0) {
				//Passo 2
				det = 1 / det;
				
				//Passo 3
				auto s = r.origin - vertices[0];
				auto s2 = s.cross(e1);
				auto t = (s2.dot(e2)) * det;

				if (t > 0 && t <= tmax) {
					//passo 4
					auto b1 = (s.dot(s1)) * det;
					
					if (b1 >= 0|| b1 > 1) {
						//Passo 5
						auto b2 = (s2.dot(r.direction)) * det;

						if (b2 >= 0 && ((b1 + b2) <= 1)) {
							//printf("ACHOU\n");
							auto b0 = 1 - b1 - b2;
							if (min && intersect->distance >= t) {
								intersect->distance = t;
								intersect->triangleIndex = i;
								intersect->p = vec3f{ b0, b1, b2 };
								intersect->object = this;
							}
						}
					}
				}
			}
		}

		return intersect->object != nullptr;
	}
	//*/

	} // end namespace cg

