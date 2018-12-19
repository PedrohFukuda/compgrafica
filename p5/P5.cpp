#include "MeshSweeper.h"
#include "P5.h"

MeshMap P5::_defaultMeshes;

inline Ray
P5::makeRay(int x, int y) const
{
  auto p = _renderer->unproject(vec3f{float(x), float(height() - y), 0});
  auto c = _renderer->camera();
  Ray r{c->position(), c->directionOfProjection()};

  if (c->projectionType() == Camera::Perspective)
    r.direction = (p - r.origin).versor();
  else
    r.origin = p - r.direction * c->nearPlane();
  return r;
}

inline void
P5::buildDefaultMeshes()
{
  _defaultMeshes["None"] = nullptr;
  _defaultMeshes["Box"] = MeshSweeper::makeBox();
  _defaultMeshes["Cone"] = GLRenderer::cone();
}

inline Primitive*
makePrimitive(MeshMapIterator mit)
{
  return new Primitive(mit->second, mit->first);
}

inline Scene*
P5::buildScene()
{
	auto scene = new cg::Scene{ "Scene 1" };

	scene->ambientLight.setRGB(82, 62, 0);

	_current = scene;

	_box = new cg::SceneObject{ "Box 1", scene };
	_box->addComponent(makePrimitive(_defaultMeshes.find("Box")));

	_box2 = new cg::SceneObject{ "Box 2", scene };
	_box2->addComponent(makePrimitive(_defaultMeshes.find("Cone")));

	_box3 = new cg::SceneObject{ "Box 3", scene };
	_box3->addComponent(makePrimitive(_defaultMeshes.find("Cone")));

	Light* l = new Light(1);
	P5::addNextLight(l);
	cg::SceneObject* empObj = new cg::SceneObject{ ("Light " + std::to_string(_LightCount++)).c_str(), scene };
	empObj->addComponent(l);
	empObj->setParent(((cg::Scene*) _current)->root());

	/*
	Light* l2 = new Light(1);
	P5::addNextLight(l);
	cg::SceneObject* empObj2 = new cg::SceneObject{ ("Light " + std::to_string(_LightCount++)).c_str(), scene };
	empObj2->addComponent(l);
	empObj2->setParent(((cg::Scene*) _current)->root());
	empObj2->transform()->setLocalPosition(vec3f(3, 5, -2));
	*/

	
	_box->setParent(scene->root());
	_box2->setParent(_box);
	_box3->setParent(scene->root());
	
	

	_box->transform()->setLocalPosition(vec3f(-3, 0, 0));
	_box->transform()->setLocalEulerAngles(vec3f(0, -37, 0));
	_box->transform()->setLocalScale(vec3f(1, 1, 1));
	
	_box->primitive()->material.ambient.setRGB(255, 255, 0);
	_box->primitive()->material.diffuse.setRGB(255, 0, 255);
	_box->primitive()->material.spot.setRGB(0, 255, 255);

	_box2->transform()->setLocalPosition(vec3f(0.1, -0.2, -4));
	_box2->transform()->setLocalEulerAngles(vec3f(-98, 0, 6.4));
	_box2->transform()->setLocalScale(vec3f(1, 1, 1));

	_box2->primitive()->material.ambient.setRGB(255, 0, 0);
	_box2->primitive()->material.diffuse.setRGB(255, 150, 0);
	_box2->primitive()->material.spot.setRGB(255, 255, 255);

	_box3->transform()->setLocalPosition(vec3f(3.7, -3, -3));
	_box3->transform()->setLocalEulerAngles(vec3f(-21, 19, -2));
	_box3->transform()->setLocalScale(vec3f(1, 5.6, 1));

	_box3->primitive()->material.ambient.setRGB(255, 255, 0);
	_box3->primitive()->material.diffuse.setRGB(0, 0, 255);
	_box3->primitive()->material.spot.setRGB(0, 255, 255);
	
	

	return scene;
}

inline Scene*
P5::buildScene2()
{
	auto scene = new cg::Scene{ "Scene 2" };

	scene->ambientLight.setRGB(82, 62, 0);

	_current = scene;

	_box = new cg::SceneObject{ "Box 1", scene };
	_box->addComponent(makePrimitive(_defaultMeshes.find("Box")));

	_box2 = new cg::SceneObject{ "Box 2", scene };
	_box2->addComponent(makePrimitive(_defaultMeshes.find("Box")));

	_box3 = new cg::SceneObject{ "Box 3", scene };
	_box3->addComponent(makePrimitive(_defaultMeshes.find("Cone")));

	Light* l = new Light(1);
	P5::addNextLight(l);
	cg::SceneObject* empObj = new cg::SceneObject{ ("Light " + std::to_string(_LightCount++)).c_str(), scene };
	empObj->addComponent(l);
	empObj->setParent(((cg::Scene*) _current)->root());

	_box->setParent(scene->root());
	_box2->setParent(scene->root());
	_box3->setParent(scene->root());



	_box->transform()->setLocalPosition(vec3f(-0.1, 0.1, -1.1));
	_box->transform()->setLocalEulerAngles(vec3f(-83, -76, 38));
	_box->transform()->setLocalScale(vec3f(0.2, 7.4, 1));

	_box->primitive()->material.ambient.setRGB(0, 0, 0);
	_box->primitive()->material.diffuse.setRGB(255, 0, 255);
	_box->primitive()->material.spot.setRGB(0, 255, 255);

	_box2->transform()->setLocalPosition(vec3f(-3.6, -0.9, -4));
	_box2->transform()->setLocalEulerAngles(vec3f(-75, -54, 6.4));
	_box2->transform()->setLocalScale(vec3f(1, 1, 1));

	_box2->primitive()->material.ambient.setRGB(255, 255, 255);
	_box2->primitive()->material.diffuse.setRGB(0, 150, 0);
	_box2->primitive()->material.spot.setRGB(255, 255, 255);

	_box3->transform()->setLocalPosition(vec3f(4.4, -4.5, 0.5));
	_box3->transform()->setLocalEulerAngles(vec3f(19, -35, 19));
	_box3->transform()->setLocalScale(vec3f(1, 0.1, 1));

	_box3->primitive()->material.ambient.setRGB(255, 0, 0);
	_box3->primitive()->material.diffuse.setRGB(255, 0, 255);
	_box3->primitive()->material.spot.setRGB(0, 255, 255);



	return scene;
}

void
P5::initialize()
{
  Application::loadShaders(_program, "shaders/p5.vs", "shaders/p5.fs");
  Assets::initialize();
  buildDefaultMeshes();
  _renderer = new GLRenderer{*buildScene()};
  _renderer->setImageSize(width(), height());
  _rayTracer = new RayTracer{*_renderer->scene(), _renderer->camera()};
  _rayTracer->_root = _renderer->scene()->root();
  _rayTracer->_renderer = _renderer;
  _rayTracer->fLight = firstLight;
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0f, 1.0f);
  glEnable(GL_LINE_SMOOTH);
  _program.use();
}

inline void
P5::hierarchyWindow()
{
  ImGui::Begin("Hierarchy");
  if (ImGui::Button("Create###object"))
    ImGui::OpenPopup("CreateObjectPopup");
  if (ImGui::BeginPopup("CreateObjectPopup"))
  {
    if (ImGui::MenuItem("Empty Object"))
	{
		cg::SceneObject* aux = new cg::SceneObject{ ("Empty object " + std::to_string(_EOCount++)).c_str(), _renderer->scene() };

		auto cur = dynamic_cast<cg::SceneObject *>(_current);
		if (cur != nullptr) {
			aux->setParent((cg::SceneObject*) _current);
		}
		else {
			aux->setParent(((cg::Scene*) _current)->root());
		}
	}
    if (ImGui::BeginMenu("3D Object"))
    {
      if (ImGui::MenuItem("Box"))
	  {
		  cg::SceneObject* aux = new cg::SceneObject{ ("Box " + std::to_string(_boxesCount++)).c_str(), _renderer->scene() };

		  aux->addComponent(makePrimitive(_defaultMeshes.find("Box")));

		  auto cur = dynamic_cast<cg::SceneObject *>(_current);
		  if (cur != nullptr) {
			  aux->setParent((cg::SceneObject*) _current);
		  }
		  else {
			  aux->setParent(((cg::Scene*) _current)->root());
		  }
	  }
      if (ImGui::MenuItem("Capsule"))
      {
        // TODO: create a new capsule.
      }
      if (ImGui::MenuItem("Cylinder"))
      {
        // TODO: create a new cylinder.
      }
      if (ImGui::MenuItem("Sphere"))
      {
        // TODO: create a new sphere.
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Light"))
	{
		if (ImGui::MenuItem("Directional Light"))
		{
			// TODO: create a new directional light.
			cg::Light* aux = new Light(0);
			P5::addNextLight(aux);

			auto cur = dynamic_cast<cg::SceneObject *>(_current);
			if (cur != nullptr) {
				cur->addComponent(aux);
			}
			else {
				cg::SceneObject* empObj = new cg::SceneObject{ ("Light " + std::to_string(_LightCount++)).c_str(), _renderer->scene() };
				empObj->addComponent(aux);
				empObj->setParent(((cg::Scene*) _current)->root());
			}
		}
		if (ImGui::MenuItem("Point Light"))
		{
			// TODO: create a new pontual light.
			cg::Light* aux = new Light(1);
			P5::addNextLight(aux);

			auto cur = dynamic_cast<cg::SceneObject *>(_current);
			if (cur != nullptr) {
				cur->addComponent(aux);
			}
			else {
				cg::SceneObject* empObj = new cg::SceneObject{ ("Light " + std::to_string(_LightCount++)).c_str(), _renderer->scene() };
				empObj->addComponent(aux);
				empObj->setParent(((cg::Scene*) _current)->root());
			}
		}
		if (ImGui::MenuItem("Spot Light"))
		{
			// TODO: create a new pontual light.
			cg::Light* aux = new Light(2);
			P5::addNextLight(aux);

			auto cur = dynamic_cast<cg::SceneObject *>(_current);
			if (cur != nullptr) {
				cur->addComponent(aux);
			}
			else {
				cg::SceneObject* empObj = new cg::SceneObject{ ("Light " + std::to_string(_LightCount++)).c_str(), _renderer->scene() };
				empObj->addComponent(aux);
				empObj->setParent(((cg::Scene*) _current)->root());
			}
		}
      ImGui::EndMenu();
    }
    ImGui::EndPopup();
  }
  ImGui::SameLine();
  if (ImGui::Button("Show###Scene"))
	  ImGui::OpenPopup("ShowScenesPopup");
  if (ImGui::BeginPopup("ShowScenesPopup"))
  {
	  if (ImGui::MenuItem("Scene 1"))
	  {
		  _renderer = new GLRenderer{ *buildScene() };
		  _renderer->setImageSize(width(), height());
	  }
	  if (ImGui::MenuItem("Scene 2"))
	  {
		  _renderer = new GLRenderer{ *buildScene2() };
		  _renderer->setImageSize(width(), height());
	  }
	  ImGui::EndPopup();
  }
  
  ImGui::Separator();
  /*
  ImGuiTreeNodeFlags flag{ImGuiTreeNodeFlags_OpenOnArrow};
  const auto scene = _renderer->scene();
  auto open = ImGui::TreeNodeEx(scene,
    _current == scene ? flag | ImGuiTreeNodeFlags_Selected : flag,
    scene->name());

  if (ImGui::IsItemClicked())
    _current = scene;
  if (open)
  {
    flag |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    ImGui::TreeNodeEx(_box,
      _current == _box ? flag | ImGuiTreeNodeFlags_Selected : flag,
      _box->name());
    if (ImGui::IsItemClicked())
      _current = _box;
    ImGui::TreePop();
  }
  ImGui::End();
*/
  ImGuiTreeNodeFlags flag{ ImGuiTreeNodeFlags_OpenOnArrow };
  auto s = _renderer->scene();
  auto open = ImGui::TreeNodeEx(s,
	  _current == s ? flag | ImGuiTreeNodeFlags_Selected : flag,
	  s->name());

  if (ImGui::IsItemClicked()) {
	  _current = s;
  }
  if (open)
  {
	  for (cg::SceneObject* sc : s->root()->children()) {
		  show(&flag, sc);
	  }
	  ImGui::TreePop();
  }


  ImGui::End();
}


inline void
P5::show(ImGuiTreeNodeFlags* flag, cg::SceneObject* obj) {
	if (!obj->children().empty()) {

		auto open = ImGui::TreeNodeEx(obj,
			_current == obj ? *flag | ImGuiTreeNodeFlags_Selected : *flag,
			obj->name());
		if (ImGui::IsItemClicked()) {
			_current = obj;
		}

		if (open) {
			if (!obj->children().empty()) {
				for (cg::SceneObject* sc : obj->children()) {
					show(flag, sc);
				}
				ImGui::TreePop();
			}
		}

	}
	else {

		ImGuiTreeNodeFlags flag;
		flag |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;


		ImGui::TreeNodeEx(obj,
			_current == _renderer->scene() ? flag | ImGuiTreeNodeFlags_Selected : flag,
			obj->name());

		if (ImGui::IsItemClicked())
			_current = obj;
	}
}

namespace ImGui
{ // begin namespace ImGui

void
ObjectNameInput(NameableObject* object)
{
  const int bufferSize{128};
  static NameableObject* current;
  static char buffer[bufferSize];

  if (object != current)
  {
    strcpy_s(buffer, bufferSize, object->name());
    current = object;
  }
  if (ImGui::InputText("Name", buffer, bufferSize))
    object->setName(buffer);
}

inline bool
ColorEdit3(const char* label, Color& color)
{
  return ImGui::ColorEdit3(label, (float*)&color);
}

inline bool
DragVec3(const char* label, vec3f& v)
{
  return DragFloat3(label, (float*)&v, 0.1f, 0.0f, 0.0f, "%.2g");
}

void
TransformEdit(Transform* transform)
{
  vec3f temp;

  temp = transform->localPosition();
  if (ImGui::DragVec3("Position", temp))
    transform->setLocalPosition(temp);
  temp = transform->localEulerAngles();
  if (ImGui::DragVec3("Rotation", temp))
    transform->setLocalEulerAngles(temp);
  temp = transform->localScale();
  if (ImGui::DragVec3("Scale", temp))
    transform->setLocalScale(temp);
}

} // end namespace ImGui

inline void
P5::sceneGui()
{
  auto scene = (Scene*)_current;

  ImGui::ObjectNameInput(_current);
  ImGui::Separator();
  if (ImGui::CollapsingHeader("Colors"))
  {
    ImGui::ColorEdit3("Background", scene->backgroundColor);
    ImGui::ColorEdit3("Ambient Light", scene->ambientLight);
  }
}

inline void
P5::inspectShape(Primitive& primitive)
{
  char buffer[16];

  snprintf(buffer, 16, "%s", primitive.meshName());
  ImGui::InputText("Mesh", buffer, 16, ImGuiInputTextFlags_ReadOnly);
  if (ImGui::BeginDragDropTarget())
  {
    if (auto* payload = ImGui::AcceptDragDropPayload("PrimitiveMesh"))
    {
      auto mit = *(MeshMapIterator*)payload->Data;
      primitive.setMesh(mit->second, mit->first);
    }
    ImGui::EndDragDropTarget();
  }
  ImGui::SameLine();
  if (ImGui::Button("...###PrimitiveMesh"))
    ImGui::OpenPopup("PrimitiveMeshPopup");
  if (ImGui::BeginPopup("PrimitiveMeshPopup"))
  {
    auto& meshes = Assets::meshes();

    if (!meshes.empty())
    {
      for (auto mit = meshes.begin(); mit != meshes.end(); ++mit)
        if (ImGui::Selectable(mit->first.c_str()))
          primitive.setMesh(Assets::loadMesh(mit), mit->first);
      ImGui::Separator();
    }
    for (auto mit = _defaultMeshes.begin(); mit != _defaultMeshes.end(); ++mit)
      if (ImGui::Selectable(mit->first.c_str()))
        primitive.setMesh(mit->second, mit->first);
    ImGui::EndPopup();
  }
}

  inline void
P5::inspectMaterial(Material& material)
{
  ImGui::ColorEdit3("Ambient", material.ambient);
  ImGui::ColorEdit3("Diffuse", material.diffuse);
  ImGui::ColorEdit3("Spot", material.spot);
  ImGui::DragFloat("Shine", &material.shine, 1, 0, 1000.0f);
  ImGui::ColorEdit3("Specular", material.specular);
  }

inline void
P5::inspectPrimitive(Primitive& primitive)
{
  //const auto flag = ImGuiTreeNodeFlags_NoTreePushOnOpen;

  //if (ImGui::TreeNodeEx("Shape", flag))
    inspectShape(primitive);
  //if (ImGui::TreeNodeEx("Material", flag))
    inspectMaterial(primitive.material);
}

inline void
P5::inspectLight(Light& light)
{
  static const char* lightTypes[]{"Directional", "Point", "Spot"};
  auto lt = light.type();

  if (ImGui::BeginCombo("Type", lightTypes[lt]))
  {
    for (auto i = 0; i < IM_ARRAYSIZE(lightTypes); ++i)
    {
      bool selected = lt == i;

      if (ImGui::Selectable(lightTypes[i], selected))
        lt = (Light::Type)i;
      if (selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
  light.setType(lt);
  ImGui::ColorEdit3("Color", light.color);
}

inline void
P5::addComponentButton(SceneObject& object)
{
  if (ImGui::Button("Add Component"))
    ImGui::OpenPopup("AddComponentPopup");
  if (ImGui::BeginPopup("AddComponentPopup"))
  {
    if (ImGui::MenuItem("Primitive"))
    {
      // TODO
    }
    if (ImGui::MenuItem("Light"))
    {
      // TODO
    }
    ImGui::EndPopup();
  }
}

inline void
P5::sceneObjectGui()
{
  auto object = (SceneObject*)_current;

  addComponentButton(*object);
  ImGui::Separator();
  ImGui::ObjectNameInput(object);
  ImGui::SameLine();
  ImGui::Checkbox("###visible", &object->visible);
  ImGui::Separator();
  if (ImGui::CollapsingHeader(object->transform()->typeName()))
    ImGui::TransformEdit(object->transform());
  if (object->primitive() != nullptr && ImGui::CollapsingHeader(object->primitive()->typeName()))
    inspectPrimitive(*object->primitive());
  if (object->light() != nullptr && ImGui::CollapsingHeader(object->light()->typeName()))
	  inspectLight(*object->light());
}

inline void
P5::objectGui()
{
  if (_current == nullptr)
    return;
  if (dynamic_cast<SceneObject*>(_current))
  {
    sceneObjectGui();
    return;
  }
  if (dynamic_cast<Scene*>(_current))
    sceneGui();
}

inline void
P5::inspectorWindow()
{
  ImGui::Begin("Inspector");
  objectGui();
  ImGui::End();
}

inline void
P5::cameraGui()
{
  auto camera = _renderer->camera();

  if (ImGui::Button("Set Default View"))
    camera->setDefaultView(float(width()) / float(height()));
  ImGui::Separator();

  vec3f temp;

  ImGui::ObjectNameInput(camera);
  temp = camera->position();
  if (ImGui::DragVec3("Position", temp))
    camera->setPosition(temp);
  temp = camera->eulerAngles();
  if (ImGui::DragVec3("Rotation", temp))
    camera->setEulerAngles(temp);

  static const char* projectionNames[]{"Perspective", "Orthographic"};
  auto cp = camera->projectionType();

  if (ImGui::BeginCombo("Projection", projectionNames[cp]))
  {
    for (auto i = 0; i < IM_ARRAYSIZE(projectionNames); ++i)
    {
      bool selected = cp == i;

      if (ImGui::Selectable(projectionNames[i], selected))
        cp = (Camera::ProjectionType)i;
      if (selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
  camera->setProjectionType(cp);
  if (cp == Camera::Perspective)
  {
    auto fov = camera->viewAngle();

    if (ImGui::SliderFloat("View Angle",
      &fov,
      MIN_ANGLE,
      MAX_ANGLE,
      "%.0f deg",
      1.0f))
      camera->setViewAngle(fov <= MIN_ANGLE ? MIN_ANGLE : fov);
  }
  else
  {
    auto h = camera->height();

    if (ImGui::DragFloat("Height",
      &h,
      MIN_HEIGHT * 10.0f,
      MIN_HEIGHT,
      math::Limits<float>::inf()))
      camera->setHeight(h <= 0 ? MIN_HEIGHT : h);
  }

  float n;
  float f;

  camera->clippingPlanes(n, f);

  if (ImGui::DragFloatRange2("Clipping Planes",
    &n,
    &f,
    MIN_DEPTH,
    MIN_DEPTH,
    math::Limits<float>::inf(),
    "Near: %.2f",
    "Far: %.2f"))
  {
    if (n <= 0)
      n = MIN_DEPTH;
    if (f - n < MIN_DEPTH)
      f = n + MIN_DEPTH;
    camera->setClippingPlanes(n, f);
  }
}

inline void
P5::assetsWindow()
{
  ImGui::Begin("Assets");
  if (ImGui::CollapsingHeader("Meshes"))
  {
    auto& meshes = Assets::meshes();

    for (auto mit = meshes.begin(); mit != meshes.end(); ++mit)
    {
      auto meshName = mit->first.c_str();
      auto selected = false;

      ImGui::Selectable(meshName, &selected);
      if (ImGui::BeginDragDropSource())
      {
        Assets::loadMesh(mit);
        ImGui::Text(meshName);
        ImGui::SetDragDropPayload("PrimitiveMesh", &mit, sizeof(mit));
        ImGui::EndDragDropSource();
      }
    }
  }
  ImGui::Separator();
  if (ImGui::CollapsingHeader("Textures"))
  {
    // p4
  }
  ImGui::End();
}

inline void
P5::renderModeGui()
{
  static int sm;

  ImGui::Combo("Shading Mode", &sm, "None\0Flat\0Gouraud\0Phong\0\0");
  // TODO

  static Color edgeColor;
  static bool showEdges;

  ImGui::ColorEdit3("Edges", edgeColor);
  ImGui::SameLine();
  ImGui::Checkbox("###showEdges", &showEdges);
}

inline void
P5::rendererWindow()
{
  int rt = _traceFlag;

  ImGui::Begin("Renderer");
  ImGui::Combo("Type", &rt, "OpenGL\0Ray Tracer\0\0");
  if (!rt)
    renderModeGui();
  else
    _traceFlag = true;
  ImGui::Separator();
  if (ImGui::CollapsingHeader("Camera"))
    cameraGui();
  ImGui::End();
}

inline void
P5::fileMenu()
{
  if (ImGui::MenuItem("New"))
  {
    // TODO
  }
  if (ImGui::MenuItem("Open...", "Ctrl+O"))
  {
    // TODO
  }
  ImGui::Separator();
  if (ImGui::MenuItem("Save", "Ctrl+S"))
  {
    // TODO
  }
  if (ImGui::MenuItem("Save As..."))
  {
    // TODO
  }
  ImGui::Separator();
  if (ImGui::MenuItem("Exit", "Alt+F4"))
  {
    shutdown();
  }
}

inline bool
showStyleSelector(const char* label)
{
  static int style = 1;

  if (!ImGui::Combo(label, &style, "Classic\0Dark\0Light\0"))
    return false;
  switch (style)
  {
    case 0: ImGui::StyleColorsClassic();
      break;
    case 1: ImGui::StyleColorsDark();
      break;
    case 2: ImGui::StyleColorsLight();
      break;
  }
  return true;
}

inline void
P5::showOptions()
{
  ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f);
  showStyleSelector("Color Theme##Selector");
  ImGui::ColorEdit3("Selected Wireframe", _selectedWireframeColor);
  ImGui::PopItemWidth();
}

inline void
P5::mainMenu()
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      fileMenu();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Renderer"))
    {
      ImGui::MenuItem("Ray Tracer", nullptr, &_traceFlag);
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Tools"))
    {
      if (ImGui::BeginMenu("Options"))
      {
        showOptions();
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void
P5::gui()
{
  mainMenu();
  if (_traceFlag)
    return;
  hierarchyWindow();
  inspectorWindow();
  assetsWindow();
  rendererWindow();
}

inline void
renderMesh(GLMesh* mesh, GLuint mode)
{
  glPolygonMode(GL_FRONT_AND_BACK, mode);
  glDrawElements(GL_TRIANGLES, mesh->vertexCount(), GL_UNSIGNED_INT, 0);
}

constexpr auto CAMERA_RES = 0.01f;
constexpr auto ZOOM_SCALE = 1.01f;

void
P5::render()
{
  _renderer->update();
  if (_traceFlag)
  {
    if (_image == nullptr)
    {
      _image = new GLImage{width(), height()};
      _rayTracer->renderImage(*_image);
    }
    _image->draw();
    return;
  }
  /*
  const auto& bc = _renderer->scene()->backgroundColor;

  _program.use();
  glClearColor(bc.r, bc.g, bc.b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  _renderer->drawGround(10, 1);
  if (!_box->visible)
    return;

  auto camera = _renderer->camera();

  if (_moveFlags)
  {
    const auto delta = camera->distance() * CAMERA_RES;
    auto d = vec3f::null();

    if (_moveFlags.isSet(MoveBits::Forward))
      d.z -= delta;
    if (_moveFlags.isSet(MoveBits::Back))
      d.z += delta;
    if (_moveFlags.isSet(MoveBits::Left))
      d.x -= delta;
    if (_moveFlags.isSet(MoveBits::Right))
      d.x += delta;
    if (_moveFlags.isSet(MoveBits::Up))
      d.y += delta;
    if (_moveFlags.isSet(MoveBits::Down))
      d.y -= delta;
    camera->translate(d);
  }
  _program.setUniformMat4("vpMatrix", vpMatrix(camera));
  _program.setUniformVec4("ambientLight", _renderer->scene()->ambientLight);
  _program.setUniformVec3("lightPosition", camera->position());

  const auto mesh = _primitive->mesh();
  auto m = glMesh(mesh);

  if (nullptr == m)
    return;

  auto t = _primitive->transform();
  auto normalMatrix = mat3f{t->worldToLocalMatrix()}.transposed();

  _program.setUniformMat4("transform", t->localToWorldMatrix());
  _program.setUniformMat3("normalMatrix", normalMatrix);
  _program.setUniformVec4("color", _primitive->material.diffuse);
  _program.setUniform("flatMode", (int)0);
  m->bind();
  renderMesh(m, GL_FILL);
  if (_current != _box)
    return;
  //_program.setUniformVec4("color", _selectedWireframeColor);
  //_program.setUniform("flatMode", (int)1);
  //renderMesh(m, GL_LINE);
  _renderer->setVectorColor(Color::white);
  _renderer->drawNormals(*mesh, t->localToWorldMatrix(), normalMatrix);

  auto b = _primitive->mesh()->bounds();

  b.transform(t->localToWorldMatrix());
  _renderer->setLineColor(_selectedWireframeColor);
  _renderer->drawBounds(b);
  _renderer->drawAxes(t->position(), mat3f{t->rotation()});
  */

  const auto& bc = _renderer->scene()->backgroundColor;

  glClearColor(bc.r, bc.g, bc.b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  _program.use();
  _renderer->drawGround(10, 1);
  if (!_box->visible)
	  return;
  if (_moveFlags)
  {
	  const auto delta = _renderer->camera()->distance() * CAMERA_RES;
	  auto d = vec3f::null();

	  if (_moveFlags.isSet(MoveBits::Forward))
		  d.z -= delta;
	  if (_moveFlags.isSet(MoveBits::Back))
		  d.z += delta;
	  if (_moveFlags.isSet(MoveBits::Left))
		  d.x -= delta;
	  if (_moveFlags.isSet(MoveBits::Right))
		  d.x += delta;
	  if (_moveFlags.isSet(MoveBits::Up))
		  d.y += delta;
	  if (_moveFlags.isSet(MoveBits::Down))
		  d.y -= delta;
	  _renderer->camera()->translate(d);
  }
  Light* l = firstLight;

  int i;
  for (i = 0; i < _LightCount && l != nullptr; i++, l = l->nxtLight()) {
	  char aux[50];

	  sprintf_s(aux, "luzes[%d].color", i);
	  _program.setUniformVec4(aux, l->color);
	  sprintf_s(aux, "luzes[%d].position", i);
	  _program.setUniformVec3(aux, l->sceneObject()->transform()->localPosition());
	  sprintf_s(aux, "luzes[%d].falloff", i);
	  _program.setUniform(aux, l->falloff());
	  _program.setUniform("light_type", l->intType());

	  if (l->intType() == 2) {
		  sprintf_s(aux, "luzes[%d].angle", i);
		  _program.setUniform(aux, l->lightAngle());
		  sprintf_s(aux, "luzes[%d].direction", i);
		  _program.setUniformVec3(aux, l->sceneObject()->transform()->rotation());
	  }
	  else {
		  sprintf_s(aux, "luzes[%d].angle", i);
		  _program.setUniform(aux, 0);
		  sprintf_s(aux, "luzes[%d].direction", i);
		  _program.setUniformVec3(aux, l->sceneObject()->transform()->rotation() * vec3f(0, -5, 10));
	  }
  }
  _program.setUniform("l_count", i);

  for (auto &s : (_renderer->scene()->root()->children())) {
	  P5::drawOnScreen(s);
  }
}

void
P5::drawOnScreen(cg::SceneObject* obj) {
	if (obj->primitive() != nullptr) {
		_program.setUniformMat4("vpMatrix", vpMatrix(_renderer->camera()));
		_program.setUniformVec4("material.oA", obj->primitive()->material.ambient);
		_program.setUniformVec4("material.oD", obj->primitive()->material.diffuse);
		_program.setUniformVec4("material.oS", obj->primitive()->material.spot);
		_program.setUniform("material.ns", obj->primitive()->material.shine);
		_program.setUniformVec4("iA", _renderer->scene()->ambientLight);
		_program.setUniformVec3("cameraPosition", _renderer->camera()->position());

		auto m = glMesh(obj->primitive()->mesh());
		if (nullptr == m)
			return;

		auto t = obj->primitive()->transform();
		auto normalMatrix = mat3f{ t->worldToLocalMatrix() }.transposed();

		_program.setUniformMat4("transform", t->localToWorldMatrix());
		_program.setUniformMat3("normalMatrix", normalMatrix);
		m->bind();
		renderMesh(m, GL_FILL);

		if (_current == obj) {
			//_program->setUniformVec4("color", _selectedWireframeColor);
			renderMesh(m, GL_LINE);
		}
	}
	for (cg::SceneObject* s : obj->children()) {
		P5::drawOnScreen(s);
	}
}

bool
P5::windowResizeEvent(int width, int height)
{
  _renderer->setImageSize(width, height);
  _image = nullptr;
  _traceFlag = false;
  return true;
}

bool
P5::keyInputEvent(int key, int action, int mods)
{
  auto active = action != GLFW_RELEASE && mods == GLFW_MOD_ALT;

  switch (key)
  {
    case GLFW_KEY_W:
      _moveFlags.enable(MoveBits::Forward, active);
      break;
    case GLFW_KEY_S:
      _moveFlags.enable(MoveBits::Back, active);
      break;
    case GLFW_KEY_A:
      _moveFlags.enable(MoveBits::Left, active);
      break;
    case GLFW_KEY_D:
      _moveFlags.enable(MoveBits::Right, active);
      break;
    case GLFW_KEY_Q:
      _moveFlags.enable(MoveBits::Up, active);
      break;
    case GLFW_KEY_Z:
      _moveFlags.enable(MoveBits::Down, active);
      break;
  }
  return false;
}

bool
P5::scrollEvent(double, double yOffset)
{
  if (ImGui::GetIO().WantCaptureMouse)
    return false;
  _renderer->camera()->zoom(yOffset < 0 ? 1.0f / ZOOM_SCALE : ZOOM_SCALE);
  return true;
}

bool
P5::mouseButtonInputEvent(int button, int actions, int mods)
{
  if (ImGui::GetIO().WantCaptureMouse)
    return false;
  (void)mods;

  auto active = actions == GLFW_PRESS;

  if (button == GLFW_MOUSE_BUTTON_LEFT)
  {
    if (active)
    {
      cursorPosition(_pivotX, _pivotY);

      // TODO: scene intersection
      const auto ray = makeRay(_pivotX, _pivotY);
	  Intersection i;
	  i.object = nullptr;
	  if (_rayTracer->intersect(ray, i) && i.object != nullptr) {
		  _current = i.object->sceneObject();
		  Primitive obj = *(i.object);
		  vec3f n = obj.triangleNormal(i.triangleIndex, i.p);
		  printf("%f %f %f\n", n[0], n[1], n[2]);
	  }


    }
    return true;
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT)
    _dragFlags.enable(DragBits::Rotate, active);
  else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    _dragFlags.enable(DragBits::Pan, active);
  if (_dragFlags)
    cursorPosition(_pivotX, _pivotY);
  return true;
}

bool
P5::mouseMoveEvent(double xPos, double yPos)
{
  if (!_dragFlags)
    return false;
  _mouseX = (int)xPos;
  _mouseY = (int)yPos;

  const auto dx = (_pivotX - _mouseX);
  const auto dy = (_pivotY - _mouseY);

  _pivotX = _mouseX;
  _pivotY = _mouseY;
  if (dx != 0 || dy != 0)
  {
    auto camera = _renderer->camera();

    if (_dragFlags.isSet(DragBits::Rotate))
    {
      const auto da = -camera->viewAngle() * CAMERA_RES;
      camera->rotateYX(dx * da, dy * da, isKeyPressed(GLFW_KEY_LEFT_ALT));
    }
    if (_dragFlags.isSet(DragBits::Pan))
    {
      const auto dt = -camera->distance() * CAMERA_RES;
      camera->translate(-dt * math::sign(dx), dt * math::sign(dy), 0);
    }
  }
  return true;
}
