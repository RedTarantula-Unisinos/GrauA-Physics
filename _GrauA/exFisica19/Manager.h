#include "Render.h"
#include "JStructs.h"

#include <list>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include <Box2D/Box2D.h>
#include <vector>

#pragma once
class Manager
{
public:
	Manager() {};
	~Manager() {};

	float32 GetTimeSteps() { return timesteps; };
	void SetTimeSteps(float32 _timesteps) { timesteps = _timesteps; };

	int32 GetVelocityIt() { return velocityIterations; };
	void SetVelocityIt(int32 _vi) { velocityIterations = _vi; };
	int32 GetPositionIt() { return positionIterations; };
	void SetPositionIt(int32 _pi) { positionIterations = _pi; };

	float GetXMin() { return xMin; };
	void SetXMin(float _n) { xMin = _n; };

	float GetXMax() { return xMax; };
	void SetXMax(float _n) { xMax = _n; };

	float GetYMin() { return yMin; };
	void SetYMin(float _n) { yMin = _n; };

	float GetYMax() { return yMax; };
	void SetYMax(float _n) { yMax = _n; };
private:
	float32 timesteps;
	int32 velocityIterations, positionIterations;
	float xMin = -40.0f, xMax = 40.0f, yMin = -40.0f, yMax = 40.0f;
};

