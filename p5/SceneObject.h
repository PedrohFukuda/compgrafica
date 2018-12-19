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
// OVERVIEW: SceneObject.h
// ========
// Class definition for scene object.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 25/08/2018

#ifndef __SceneObject_h
#define __SceneObject_h

#include "SceneNode.h"
#include "Primitive.h"
#include "Transform.h"
#include "Light.h"
#include <list>

namespace cg
{ // begin namespace cg

	// Forward definition
	class Scene;


	/////////////////////////////////////////////////////////////////////
	//
	// SceneObject: scene object class	
	// ===========
	class SceneObject : public SceneNode
	{
	public:
		bool visible{ true };

		/// Constructs an empty scene object.
		SceneObject(const char* name, Scene* scene) :
			SceneNode{ name },
			_scene{ scene },
			_parent{ nullptr },
			_primitive{ nullptr },
			_light{ nullptr }
		{
			makeUse(&_transform);
			addComponent(&_transform);
		}

		/// Returns the scene which this scene object belong to.
		auto scene() const
		{
			return _scene;
		}

		/// Returns the parent of this scene object.
		auto parent() const
		{
			return _parent;

		}


		auto light() {
			return _light;
		}

		/// Returns the transform of this scene object.
		auto transform()
		{
			return &_transform;
		}


		//**********************************************
		//**********************************************
		//**********************************************

		auto primitive() {
			return _primitive;
		}

		auto children() {
			return _children;
		}

		//Setar como privado
		void addChild(SceneObject* c) {
			_children.insert(_children.begin(), c);
		}

		bool rmvChild(Reference<SceneObject> c) {
			std::list<Reference<SceneObject>>::iterator _childrenIterator;
			for (_childrenIterator = _children.begin(); *_childrenIterator != c; _childrenIterator++);
			if (_childrenIterator != _children.end()) {
				_children.erase(_childrenIterator);
			}
			else {
				return false;
			}
			return true;
		}

		bool addComponent(Component* c) {
			auto p = dynamic_cast<Primitive *>(c);
			if (p != nullptr && _primitive == nullptr) {
				_primitive = p;
				_components.insert(_components.begin(), c);
			}
			auto l = dynamic_cast<Light *>(c);
			if (l != nullptr && _light == nullptr) {
				_light = l;
				_components.insert(_components.begin(), c);

			}
			else {
				_components.insert(_components.begin(), c);
			}
			c->setSceneObject(this);
			return true;
		}

		bool rmvComponent(Reference<Component> c) {
			std::list<Reference<Component>>::iterator _componentsIterator;
			for (_componentsIterator = _components.begin(); *_componentsIterator != c; _componentsIterator++);
			if (_componentsIterator != _components.end()) {
				_components.erase(_componentsIterator);//Adicionar if se não encontrar
			}
			else {
				return false;
			}
			return true;
		}


		/// Sets the parent of this scene object.
		void setParent(SceneObject* parent) {
			if (_parent != nullptr) {
				_parent->rmvChild(this);

			}

			if (parent == nullptr) {
				for (parent = _parent; parent->name() != "root"; parent = parent->parent());
			}

			parent->addChild(this);
			_parent = parent;

		}


		//**********************************************
		//**********************************************
		//**********************************************


	private:
		Scene* _scene;
		SceneObject* _parent;
		Transform _transform;
		Primitive* _primitive;
		Light* _light;
		std::list<Reference<SceneObject>> _children;
		std::list<Reference<Component>> _components;

		friend class Scene;

	}; // SceneObject

/// Returns the transform of a component.
	inline Transform*
		Component::transform() // declared in Component.h
	{
		return sceneObject()->transform();
	}

	/// Returns the parent of a transform.
	inline Transform*
		Transform::parent() const // declared in Transform.h
	{
		if (auto p = sceneObject()->parent())
			return p->transform();
		return nullptr;
	}

} // end namespace cg

#endif // __SceneObject_h
