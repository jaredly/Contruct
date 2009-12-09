// Include StdAfx.h
#include "StdAfx.h"

void EndProperties(int iMode, CVirtualPropItem* item, MicroEdittime* pm)
{
	if (iMode == MODE_INIT) {
		ASSIGN_CSTRING(item->Description, "");
		ASSIGN_CSTRING(item->Label, "");
		ASSIGN_CSTRING(item->Text, "");
		item->Type = -1;	// end
		pm->Proc(item);		// post end to property vector
	}
}

void Quad::Rotated(const RECTF& rc, double angle) {

		operator=(rc);

		if (angle == 0.0f)
			return;

		angle = RADIANS(-angle);

		double w = rc.right - rc.left;
		double h = rc.bottom - rc.top;
		double t = atan(w/h);
		double d = sqrt((w * w) + (h * h));	// diagonal distance

		// p1 does not move

		// rotate p2 about p1
		p2.x = p1.x + (w * sin(angle + RADIANS(90)));
		p2.y = p1.y + (w * cos(angle + RADIANS(90))); 

		// rotate p3 about p1
		p3.x = p1.x + (h * sin(angle));
		p3.y = p1.y + (h * cos(angle));

		// rotate p4 about p1
		p4.x = p1.x + (d * sin(angle + t));
		p4.y = p1.y + (d * cos(angle + t));

	}

/////////////////////////////////////////////////////////////////////////////////////////////
//	CRect

CObjectRectangle::CObjectRectangle(double _x, double _y, double _w, double _h,double _a, double _ox, double _oy)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	a = _a;
	ox = _ox;
	oy = _oy;
}
CRect CObjectRectangle::GetBoundingRect()
{
#define Rad(a) float((a) / 57.29577951308f)
#define Deg(a) float((a) * 57.29577951308f)

//			0   1
//			2   3
//
	int i;

	double m[2][2];
	m[0][0] = cos(Rad(-a));
	m[1][0] = -sin(Rad(-a));
	m[0][1] = sin(Rad(-a));
	m[1][1] = cos(Rad(-a));

	double source[4][2];
	source[0][0] = -w*(ox);
	source[0][1] = -h*(oy);
	source[1][0] = w*(1-ox);
	source[1][1] = -h*(oy);
	source[2][0] = -w*(ox);
	source[2][1] = h*(1-oy);
	source[3][0] = w*(1-ox);
	source[3][1] = h*(1-oy);

	for(i = 0; i < 4; i++)
	{
		double x = source[i][0];
		double y = source[i][1];
		source[i][0] = x * m[0][0] + y * m[0][1];
		source[i][1] = x * m[1][0] + y * m[1][1];
	}

	double l = source[0][0];
	double r = source[0][0];
	double t = source[0][1];
	double b = source[0][1];
	
	for(i = 1; i < 4; i++)
	{
		double lx = source[i][0];
		double ly = source[i][1];

		if(l > lx) l = lx;
		if(r < lx) r = lx;
		if(t > ly) t = ly;
		if(b < ly) b = ly;
	}

	//TODO: angles
	return CRect(l+x,t+y,r+x,b+y); 
}
bool CObjectRectangle::PtInRect(POINT point)
{
	// TODO: actually program this to check if a point is in a rotated box...not the bounding box
	return GetBoundingRect().PtInRect(point) ? true : false;
}
#define POW2(x) ((x)*(x))

void CObjectRectangle::CalculateBlitValues(double& _x, double& _y, double& _w, double& _h, double& _a)
{
	double HotSpotAngle = atan2(float(oy*h), float(ox*w));
	double HotSpotDist = sqrt((double)POW2(ox*w) + POW2(oy*h));


	float dx = HotSpotDist * cos(Rad(a) + HotSpotAngle);
	float dy = HotSpotDist * sin(Rad(a) + HotSpotAngle);
		
	_x = x - dx;
	_y = y - dy;
	_w = w;
	_h = h;
	_a = a;
}

CPoint CObjectRectangle::GetPoint(double xratio, double yratio, double distance)
{
	float pointx = x;
	float pointy = y;

	// horizontal
	double hor = ((-(w) * ox) + (w)*xratio + (distance*(-0.5+xratio))*2)+0.5-xratio;

	pointx +=  cos(Rad(-a)) * hor;
	pointy += -sin(Rad(-a)) * hor;

	// vertical

	double vert = ((-(h) * oy) + (h)*yratio + (distance*(-0.5+yratio))*2)+0.5-yratio;
	pointx += sin(Rad(-a)) * vert;
	pointy += cos(Rad(-a)) * vert;





	return CPoint(floor(pointx), floor(pointy));
}