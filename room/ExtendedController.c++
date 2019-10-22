// ExtendedController.c++

#include "ExtendedController.h"
#include "ModelView.h"

ExtendedController::ExtendedController(const std::string& name, int rcFlags):
	GLFWController(name, rcFlags)
{
}

void ExtendedController::handleMouseMotion(int x, int y)
{
	std::cout << "Implement ExtendedController::handleMouseMotion!\n";
	// 1. Get a pixel (dx, dy)
	// 2. update screenbaseX, screenBaseY
	// 3. if translation, map (dx, dy) to (dxInLDS, dyInLDS) and pass to
	//    ModelView::addToGlobalPan
	// 4. else if rotation, map (dx, dy) to rotation angles and pass to
	//    ModelView::addToGlobalRotationDegrees.
	// 5. do a redraw()
}
