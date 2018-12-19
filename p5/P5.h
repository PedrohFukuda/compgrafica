#ifndef __P5_h
#define __P5_h

#include "Assets.h"
#include "GLRenderer.h"
#include "GLImage.h"
#include "Light.h"
#include "Primitive.h"
#include "RayTracer.h"
#include "core/Flags.h"
#include "graphics/Application.h"

using namespace cg;

class P5: public GLWindow
{
public:
	Light* firstLight;

  P5(int width, int height):
    GLWindow{"cg2018 - P5", width, height},
    _program{"P5"}
  {
    // do nothing
  }

protected:
  /// Initializes the app.
  void initialize() override;

  /// Updates the GUI.
  void gui() override;

  /// Renders the scene.
  void render() override;

  /////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////

  void addNextLight(Light* l) {
	  Light* aux = firstLight;
	  firstLight = l;
	  firstLight->setNextLight(aux);
  }

  auto light() {
	  return firstLight;
  }

  /////////////////////////////////////////////
  /////////////////////////////////////////////
  /////////////////////////////////////////////

private:
  enum class MoveBits
  {
    Left = 1,
    Right = 2,
    Forward = 4,
    Back = 8,
    Up = 16,
    Down = 32
  };

  enum class DragBits
  {
    Rotate = 1,
    Pan = 2
  };

  GLSL::Program _program;
  Reference<GLRenderer> _renderer;
  Reference<SceneObject> _box, _box2, _box3;
  Reference<Primitive> _primitive;
  SceneNode* _current{};
  Color _selectedWireframeColor{255, 102, 0};
  Flags<MoveBits> _moveFlags{};
  Flags<DragBits> _dragFlags{};
  int _pivotX;
  int _pivotY;
  int _mouseX;
  int _mouseY;
  Reference<RayTracer> _rayTracer;
  Reference<GLImage> _image;
  bool _traceFlag{};

  int _boxesCount = 4, _EOCount = 1, _LightCount = 1, _shading;

  static MeshMap _defaultMeshes;

  Scene* buildScene();
  Scene* buildScene2();

  void mainMenu();
  void fileMenu();
  void showOptions();

  void hierarchyWindow();
  void inspectorWindow();
  void assetsWindow();
  void rendererWindow();
  void sceneGui();
  void sceneObjectGui();
  void objectGui();
  void cameraGui();
  void inspectPrimitive(Primitive&);
  void inspectShape(Primitive&);
  void inspectMaterial(Material&);
  void inspectLight(Light&);
  void addComponentButton(SceneObject&);
  void renderModeGui();

  bool windowResizeEvent(int, int) override;
  bool keyInputEvent(int, int, int) override;
  bool scrollEvent(double, double) override;
  bool mouseButtonInputEvent(int, int, int) override;
  bool mouseMoveEvent(double, double) override;

  void drawOnScreen(cg::SceneObject*);
  void show(ImGuiTreeNodeFlags*, cg::SceneObject*);

  Ray makeRay(int, int) const;

  static void buildDefaultMeshes();

}; // P5

#endif // __P5_h
