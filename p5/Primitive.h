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
// OVERVIEW: Primitive.h
// ========
// Class definition for primitive.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 30/10/2018

#ifndef __Primitive_h
#define __Primitive_h

#include "Component.h"
#include "graphics/GLMesh.h"
#include "geometry/TriangleMesh.h"
#include "Intersection.h"
#include "Material.h"

namespace cg
{ // begin namespace cg


/////////////////////////////////////////////////////////////////////
//
// Primitive: primitive class
// =========
class Primitive: public Component
{
public:
  Material material;

  Primitive(TriangleMesh* mesh, const std::string& meshName):
    Component{"Primitive"},
    _mesh{mesh},
    _meshName(meshName)
  {
    // do nothing
  }
 

  TriangleMesh* mesh() const
  {
    return _mesh;
  }

  const char* const meshName() const
  {
    return _meshName.c_str();
  }

  void setMesh(TriangleMesh* mesh, const std::string& meshName)
  {
    _mesh = mesh;
    _meshName = meshName;
  }

  bool intersect(const Ray& ray, Intersection* intersect) const;
  
  vec3f trianglePoint(int index, vec3f p) const {
	  vec3f vertices[3];
	  auto t = _mesh->data().triangles + index;
	  vertices[0] = _mesh->data().vertices[t->v[0]];
	  vertices[1] = _mesh->data().vertices[t->v[1]];
	  vertices[2] = _mesh->data().vertices[t->v[2]];

	  return p[0] * vertices[0] + p[1] * vertices[1] + p[2] * vertices[2];
  }

  vec3f triangleNormal(int index, const vec3f& p) const {
	  vec3f vertices[3];
	  auto t = _mesh->data().triangles + index;
	  vertices[0] = _mesh->data().vertexNormals[t->v[0]];
	  vertices[1] = _mesh->data().vertexNormals[t->v[1]];
	  vertices[2] = _mesh->data().vertexNormals[t->v[2]];
	  vec3f n = (p[0] * vertices[0] + p[1] * vertices[2] + p[2] * vertices[1]);
	  
	  return n.versor();
  }
  
private:
  Reference<TriangleMesh> _mesh;
  std::string _meshName;

  bool meshIntersection(const Ray& r, Intersection* intersect, float, bool, float) const;

}; // Primitive

} // end namespace cg

#endif // __Primitive_h
