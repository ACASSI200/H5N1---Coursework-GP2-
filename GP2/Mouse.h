#pragma once

class CMouse
{
public:
	CMouse();
	~CMouse();

	void setMouseButtonsDown(bool mouse1,bool mouse2,bool mouse3);
	void setMouseMove(float x,float y);

	bool getMouseDown(int index);
	bool getMouseUp(int index);

	float getRelativeMouseX();
	float getRelativeMouseY();
	float getAbsoluteMouseX();
	float getAbsoluteMouseY();
private:
	bool m_bMouseButtons[3];
	float m_fMouseDeltaX;
	float m_fMouseDeltaY;
	float m_fMouseAbsX;
	float m_fMouseAbsY;
	float m_fMouseWheel;
};