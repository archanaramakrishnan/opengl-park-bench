// ExtendedController.c++

#include "ExtendedController.h"
#include "ModelView.h"

ExtendedController::ExtendedController(const std::string& name, int rcFlags):
	GLFWController(name, rcFlags)
{
}

void ExtendedController::handleMouseMotion(int x, int y)
{
	// 1. Get a pixel (dx, dy)
	double dx=x-screenBaseX;
	double dy=y-screenBaseY;
	
	// 2. update screenbaseX, screenBaseY
	screenBaseX=x;
	screenBaseY=y;

	// 3. if translation, map (dx, dy) to (dxInLDS, dyInLDS) and pass to
	//    ModelView::addToGlobalPan
	if(mouseMotionIsTranslate==true)
	{
		double dxInLDS=2*dx/newWindowWidth;
		double dyInLDS=2*dy/newWindowHeight;
		ModelView::addToGlobalPan(dxInLDS, dyInLDS, 0.0);

	}
	// 4. else if rotation, map (dx, dy) to rotation angles and pass to
	//    ModelView::addToGlobalRotationDegrees.
	else if(mouseMotionIsRotate==true)
	{
		//double rotY = factor * Δx; // x mouse motion ⇒ rotation about y (and vice versa)
		ModelView::addToGlobalRotationDegrees(dy*0.8, dx*0.8, 0.0);
	}

	// 5. do a redraw()
	redraw();
}
