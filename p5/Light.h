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
// OVERVIEW: Light.h
// ========
// Class definition for light.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 05/10/2018

#ifndef __Light_h
#define __Light_h

#include "Component.h"
#include "graphics/Color.h"

namespace cg
{ // begin namespace cg


/////////////////////////////////////////////////////////////////////
//
// Light: light class
// =====
	class Light : public Component
	{
	public:
		enum Type
		{
			Directional,
			Point,
			Spot
		};

		Color color{ Color::white };

		Light(int tp) :
			Component{ "Light" }
		{
			_falloff = 1;
			if (tp == 0) {
				_type = Directional;
			}
			else if (tp == 1) {
				_type = Point;
			}
			else {
				_type = Spot;
				_lightAngle = 90;
			}


			_nextLight = nullptr;
		}

		auto type() const
		{
			return _type;
		}




		//////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////

		auto intType() {
			if (_type == Type::Directional) {
				return 0;
			}
			else if (_type == Type::Point) {
				return 1;
			}
			else {
				return 2;
			}
		}

		auto falloff() {
			return _falloff;
		}

		auto lightAngle() {
			return _lightAngle;
		}

		void setNextLight(Light* l) {
			_nextLight = l;
		}

		auto nxtLight() {
			return _nextLight;
		}

		void setFalloff(int value) {
			if (value == 0 || value == 1 || value == 2)
				_falloff = value;
		}

		void setLightAngle(float value) {
			_lightAngle = value;
		}

		//////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////


		void setType(Type type)
		{
			_type = type;
		}



	private:
		Type _type;
		float _lightAngle;
		int _falloff;
		Light* _nextLight;

	}; // Light

} // end namespace cg

#endif // __Light_h
