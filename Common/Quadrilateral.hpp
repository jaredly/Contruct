// QUADRILATERAL CLASS
// Represents a quad
#ifndef _QUADRILATERAL_HPP_
#define _QUADRILATERAL_HPP_

#define Rad(a) float((a) / 57.29577951308f)
#define Deg(a) float((a) * 57.29577951308f)

#ifdef DEFINE_POINTF
struct POINTF {
	float x;
	float y;
};
#endif
#ifdef DEFINE_RECTF
struct RECTF {
	float left;
	float top;
	float right;
	float bottom;
};
#endif

/* this version is old and doesnt match the runtime
class Quadrilateral
{
public:
	POINTF p1;		// Top left; origin; remains same throughout transformations
	POINTF p2;		// Top right
	POINTF p3;		// Bottom left
	POINTF p4;		// Bottom right

	// Create from a stretched rect
	void Stretched(RECTF rc, float xf, float yf)
	{
		// Set from rc
		operator = (rc);

		// Width and height
		float w = rc.right - rc.left;
		float h = rc.bottom - rc.top;

		// p1 does not move

		// stretch p2 relative to p1
		p2.x = p1.x + (w * xf);

		// stretch p3 relative to p1
		p3.y = p1.y + (h * yf);

		// stretch p4 relative to p1
		p4.x = p2.x;
		p4.y = p3.y;
	}

	// Create from a rotated rect.  Angle in degrees
	void Rotated(RECTF rc, float angle)
	{
		// Set from rc
		operator = (rc);

		if (angle == 0)
			return;

		// Covert mathematical angle to radians and flip (for bearing)
		angle = Rad(-angle);

		float w = rc.right - rc.left;
		float h = rc.bottom - rc.top;
		float t = atanf(w/h);
		float d = sqrtf((w * w) + (h * h));	// diagonal distance

		// p1 does not move

		// rotate p2 about p1
		p2.x = p1.x + (w * sinf(angle + Rad(90)));
		p2.y = p1.y + (w * cosf(angle + Rad(90)));

		// rotate p3 about p1
		p3.x = p1.x + (h * sinf(angle));
		p3.y = p1.y + (h * cosf(angle));

		// rotate p4 about p1
		p4.x = p1.x + (d * sinf(angle + t));
		p4.y = p1.y + (d * cosf(angle + t));
	}

	// Create from a stretched then rotated rect
	void StretchRotated(RECTF rc, float xf, float yf, float angle)
	{
		// First create from stretch
		Stretched(rc, xf, yf);

		// Make new rect
		RECTF stretched;
		stretched.left = p1.x;
		stretched.top = p1.y;
		stretched.right = p4.x;
		stretched.bottom = p4.y;

		// Create from rotated
		if (angle != 0)
			Rotated(stretched, angle);
	}

	// Create from rect
	void operator=(RECTF rc)
	{
		p1.x = rc.left;
		p1.y = rc.top;

		p2.x = rc.right;
		p2.y = rc.top;

		p3.x = rc.left;
		p3.y = rc.bottom;

		p4.x = rc.right;
		p4.y = rc.bottom;
	}

	void operator=(RECT rc)
	{
		p1.x = (float)rc.left;
		p1.y = (float)rc.top;

		p2.x = (float)rc.right;
		p2.y = (float)rc.top;

		p3.x = (float)rc.left;
		p3.y = (float)rc.bottom;

		p4.x = (float)rc.right;
		p4.y = (float)rc.bottom;
	}
};
*/

inline void RotatePoint(float& x, float& y, float radians)
{
	float dist = sqrt(x*x + y*y);
	float angle = atan2(y, x);
	angle += radians;
	x = cos(angle) * dist;
	y = sin(angle) * dist;
}

inline void RotatePoint(double& x, double& y, double radians)
{
	double dist = sqrt(x*x + y*y);
	double angle = atan2(y, x);
	angle += radians;
	x = cos(angle) * dist;
	y = sin(angle) * dist;
}

class Quadrilateral
{
public:
	Quadrilateral()
	{
		width = 0;
		height = 0;
	}
	POINTF p1;		// Top left; origin
	POINTF p2;		// Top right
	POINTF p3;		// Bottom left
	POINTF p4;		// Bottom right

	int width;  // used by davo to clamp uv
	int height; // used by davo to clamp uv

	// Create from a stretched rect
	void Stretched(RECTF& rc, float xf, float yf)
	{
		// Set from rc
		operator = (rc);

		// Width and height
		float w = rc.right - rc.left;
		float h = rc.bottom - rc.top;

		// p1 does not move

		// stretch p2 relative to p1
		p2.x = p1.x + (w * xf);

		// stretch p3 relative to p1
		p3.y = p1.y + (h * yf);

		// stretch p4 relative to p1
		p4.x = p2.x;
		p4.y = p3.y;

	}

	// Create from a rotated rect.  Angle in degrees
	void Rotated(RECTF& rc, float angle)
	{
		// Set from rc
		operator = (rc);

		if (angle == 0)
			return;

		// Covert mathematical angle to radians and flip (for bearing)
		angle = Rad(-angle);

		const float w = rc.right - rc.left;
		const float h = rc.bottom - rc.top;

		const float sina = sin(angle);
		const float cosa = cos(angle);

		const float wcosa = w * cosa;
		const float wsina = w * sina;
		const float hsina = h * sina;
		const float hcosa = h * cosa;

		p2.x = p1.x + wcosa;
		p2.y = p1.y - wsina;

		p3.x = p1.x + hsina;
		p3.y = p1.y + hcosa;

		p4.x = p1.x + wcosa + hsina;
		p4.y = p1.y - wsina + hcosa;
	}

	void RotatedAll(RECTF& rc, float angle)
	{
		// Set from rc
		operator = (rc);

		// Covert mathematical angle to radians and flip (for bearing)
		angle = Rad(angle);

		// Rotate points individually
		RotatePoint(p1.x, p1.y, angle);
		RotatePoint(p2.x, p2.y, angle);
		RotatePoint(p3.x, p3.y, angle);
		RotatePoint(p4.x, p4.y, angle);
	}
	void Rotated(RECTF& rc, float angle, float skewX, float skewY)
	{
		// No skew:  use faster overload
		if (skewX == 0.0f && skewY == 0.0f)
			Rotated(rc, angle);

		// Set from rc
		operator = (rc);

		float originX = rc.left;
		float originY = rc.top;

		// Apply skew before rotation
		Skew(skewX, skewY);

		// Covert mathematical angle to radians and flip (for bearing)
		angle = Rad(angle);

		// Rotate points individually - skew unaligns points
		p1.x -= originX;	p1.y -= originY;
		p2.x -= originX;	p2.y -= originY;
		p3.x -= originX;	p3.y -= originY;
		p4.x -= originX;	p4.y -= originY;

		RotatePoint(p1.x, p1.y, angle);
		RotatePoint(p2.x, p2.y, angle);
		RotatePoint(p3.x, p3.y, angle);
		RotatePoint(p4.x, p4.y, angle);

		p1.x += originX;	p1.y += originY;
		p2.x += originX;	p2.y += originY;
		p3.x += originX;	p3.y += originY;
		p4.x += originX;	p4.y += originY;
	}

	void Skew(float skewX, float skewY)
	{
		// Apply skew
		if (skewX != 0.0f) {
			p1.x += skewX;
			p2.x += skewX;
		}
		if (skewY != 0.0f) {
			p2.y += skewY;
			p4.y += skewY;
		}
	}

	// Create from a stretched then rotated rect
	void StretchRotated(RECTF& rc, float xf, float yf, float angle)
	{
		// First create from stretch
		Stretched(rc, xf, yf);

		// Make new rect
		RECTF stretched;
		stretched.left = p1.x;
		stretched.top = p1.y;
		stretched.right = p4.x;
		stretched.bottom = p4.y;

		// Create from rotated
		if (angle != 0)
			Rotated(stretched, angle);
	}
	void StretchRotatedAll(RECTF& rc, float xf, float yf, float angle)
	{
		// First create from stretch
		Stretched(rc, xf, yf);

		// Make new rect
		RECTF stretched;
		stretched.left = p1.x;
		stretched.top = p1.y;
		stretched.right = p4.x;
		stretched.bottom = p4.y;

		// Create from rotated
		if (angle != 0)
			RotatedAll(stretched, angle);	// Skew overload
	}

	void GetBoundingRect(RECTF& rc)
	{
		rc.left = min(min(p1.x, p2.x), min(p3.x, p4.x));
		rc.right = max(max(p1.x, p2.x), max(p3.x, p4.x));
		rc.top = min(min(p1.y, p2.y), min(p3.y, p4.y));
		rc.bottom = max(max(p1.y, p2.y), max(p3.y, p4.y));
	}

	// Create from rect
	void operator=(RECTF rc)
	{
		p1.x = rc.left;
		p1.y = rc.top;

		p2.x = rc.right;
		p2.y = rc.top;

		p3.x = rc.left;
		p3.y = rc.bottom;

		p4.x = rc.right;
		p4.y = rc.bottom;
	}

	void operator=(RECT rc)
	{
		p1.x = rc.left;
		p1.y = rc.top;

		p2.x = rc.right;
		p2.y = rc.top;

		p3.x = rc.left;
		p3.y = rc.bottom;

		p4.x = rc.right;
		p4.y = rc.bottom;
	}
};

#endif