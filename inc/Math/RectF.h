#pragma once

struct RectF
{
public:
	RectF();
	RectF(float x, float y, float width, float height);
	~RectF();

	float getLeft() const	{ return x; }
	float getRight() const	{ return x + width; }
	float getUp() const		{ return y; }
	float getDown()	const	{ return y - height; }

public:
	float x;
	float y;
	float width;
	float height;
};