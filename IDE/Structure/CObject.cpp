// CObj.cpp
// All CObj functions

#include "StdAfx.h"
#include "..\Structure.h"
#include "..\Editors\LayoutEditor.h"

extern CLayoutEditor* g_pLayoutView;

CObj::CObj()
	: editObject(&objectInfo)
{
	// editObject specifies member variable (as above)
	//editObject = NULL;
	bSerializeData = FALSE;
	m_bLocked = FALSE;
	m_bVisible = TRUE;

	m_AllowSize = true;
	m_Draw = true;
	m_ShowAngles = false;

	m_bInGroup = FALSE;
	m_bIsGroup = FALSE;
	m_bGroupOpen = FALSE;
	m_group = NULL;

	// Close the startup groups except for properties
	expandSave["Common"] = 1;
	expandSave["Behaviors"] = 1;
	expandSave["Effects"] = 1;
	expandSave["Private Variables"] = 1;
	expandSave["Attributes"] = 1;
	expandSave["Families"] = 1;
	expandSave["Container"] = 1;

	DuplicateSerialize = false;
}

CObj::~CObj() 
{
	// editObject points to a member variable now
	/*
	if (editObject != NULL) {
		delete editObject;
		editObject = NULL;
	}
	*/
}
void CObj::UpdateGroupInfo(BOOL bUpdateGroupPos, float Zoom)
{
	if (m_bIsGroup) {
		BOOL bValueSet = FALSE;
		CRect rc;
		CObj *o;
		CPoint p1 = CPoint(GetX(), GetY());
		POSITION pos = m_groupObjects.GetHeadPosition();
		for (int i = 0; i < m_groupObjects.GetCount(); i++) 
		{
			o = g_pLayoutView->GetObject(m_groupObjects.GetNext(pos));

			if (!bValueSet)
			{
				/*rc.left = o->GetX();
				rc.top = o->GetY();
				rc.right = o->GetX() + o->GetW();
				rc.bottom = o->GetY() + o->GetH();*/
				rc = o->GetObjectRect(g_pLayoutView, true).GetBoundingRect();
				Zoom = g_pLayoutView->m_Zoom;
				rc.left /= Zoom;
				rc.right /= Zoom;
				rc.top /= Zoom;
				rc.bottom /= Zoom;
				
				bValueSet = TRUE;
			}

			else {
				CRect rc2;
				rc2 =  o->GetObjectRect(g_pLayoutView, true).GetBoundingRect();
	
				rc2.left /= Zoom;
				rc2.right /= Zoom;
				rc2.top /= Zoom;
				rc2.bottom /= Zoom;
				rc.left = min(rc.left, rc2.left);
				rc.top = min(rc.top,rc2.top);
				rc.right = max(rc.right, rc2.right);
				rc.bottom = max(rc.bottom, rc2.bottom);
			}
		}

		SetPos(rc.left, rc.top, bUpdateGroupPos);
		SetSize(rc.Width(), rc.Height());
	}

	if (m_bInGroup)
		m_group->UpdateGroupInfo(bUpdateGroupPos);
}

void CObj::SetPos(double x, double y, BOOL bNotFirst)
{
	CPoint pOffset = CPoint(x - editObject->objectX, y - editObject->objectY);
	editObject->objectX = x;
	editObject->objectY = y;

	if (m_bIsGroup && bNotFirst) {
		POSITION pos = m_groupObjects.GetHeadPosition();
		CObj *o;
		for (int i = 0; i < m_groupObjects.GetCount(); i++) {
			o = g_pLayoutView->GetObject(m_groupObjects.GetNext(pos));
			o->SetPos(o->GetX() + pOffset.x, o->GetY() + pOffset.y, bNotFirst);
		}
		UpdateGroupInfo(FALSE);
	}

	if (m_bInGroup && bNotFirst)
		UpdateGroupInfo(FALSE);
}

void CObj::SetSize(double w, double h)
{
	editObject->objectWidth = w;
	editObject->objectHeight = h;
}
void CObj::SetAngle(double a)
{
	editObject->objectAngle = a;
}
void CObj::SetDLLIdentifier( DWORD id ) 
{
	editObject->DLLIndex = id;
}

float CObj::GetX()
{
	return editObject->objectX;
}

float CObj::GetY()
{
	return editObject->objectY;
}

float CObj::GetW()
{
	return editObject->objectWidth;
}

float CObj::GetH()
{
	return editObject->objectHeight;
}

float CObj::GetA()
{
	return editObject->objectAngle;
}

float CObj::GetOriginX()
{
	// TODO
	return editObject->originX;
}
float CObj::GetOriginY()
{
	// TODO
	return editObject->originY;
}
CObjectRectangle CObj::GetObjectRect(CLayoutEditor* ed, bool useoinfo)
{
	if(useoinfo)
	{
/*
		long x = floor(GetX()*ed->m_Zoom + 0.5);
		long y = floor(GetY()*ed->m_Zoom + 0.5);
		long w = floor((GetW()+GetX())*ed->m_Zoom+0.5);
		long h = floor((GetH()+GetY())*ed->m_Zoom+0.5);*/

		long x = (GetX()*ed->m_Zoom );
		long y = (GetY()*ed->m_Zoom );
		long w = ((GetW()+GetX())*ed->m_Zoom);
		long h = ((GetH()+GetY())*ed->m_Zoom);

		w-= x;
		h-= y;

		zoomed_objectX = x;
		zoomed_objectY = y;
		zoomed_objectWidth = w;
		zoomed_objectHeight = h;
		zoomed_angle = GetA();
		zoomed_originx =  GetOriginX();
		zoomed_originy =  GetOriginY();

	}

	return CObjectRectangle(
					zoomed_objectX,zoomed_objectY,zoomed_objectWidth,zoomed_objectHeight, 
					zoomed_angle, zoomed_originx, zoomed_originy
					);

}
DWORD CObj::GetDLLIdentifier()
{
	return editObject->DLLIndex;
}

long CObj::GetInstanceID()
{
	return editObject->instanceID;
}

long CObj::GetGlobalID()
{
	return editObject->ObjectIdentifier;
}

void CObj::SetLocked(BOOL bLocked) 
{
	m_bLocked = bLocked;
}

BOOL CObj::GetLocked()
{
	return m_bLocked;
}

void CObj::SetVisible(BOOL bVisible) 
{
	m_bVisible = bVisible;
}

BOOL CObj::GetVisible()
{
	return m_bVisible;
}

extern CLayout* g_pFrame;

bool CObj::Serialize(CArchive& ar) 
{
	CString ExpectedName = "CObj";
	int     Version      = 3;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return false;

	// Loading
	if(ar.IsLoading())
	{
		ar >> editObject->objectX >> editObject->objectY >> editObject->objectWidth >> 
			editObject->objectHeight >> editObject->originX >> editObject->originY >> 
			editObject->objectAngle;
		
		if (Version >= 2) {
			ar >> editObject->filter;
		}

		// By default assume not a group (fix for file corruption bug)
		BOOL isGroup = false;

		if (Version >= 3)
			ar >> isGroup;

		CString name = "";

		if (!isGroup) {
			
			ar >> name;
			const CPlugin& plugin = GetPlugin(name);
			editObject->DLLIndex = plugin.m_pInfo->oID;

		}
	
		ar >> editObject->instanceID;
		ar >> editObject->ObjectIdentifier;
		ar >> m_bLocked;
		ar >> m_bVisible;
		ar >> m_AllowSize;
		ar >> m_Draw;
		ar >> m_ShowAngles;
		ar >> m_bInGroup;
		ar >> m_bIsGroup;
		ar >> m_bGroupOpen;

		// Serialize virtual edittime, this is really only useful for copy/paste type stuff
		long tempPointer;
		ar >> tempPointer;
		editObject->peditInfo = (VEditTime*)tempPointer;

		long groupID;
		CLayout* level = g_pFrame;
		if (m_bInGroup) {
			ar >> groupID;
		}

		if (m_bIsGroup) {
			POSITION pos = m_groupObjects.GetHeadPosition();
			int c;
			ar >> c;
			CObj *o2;
			m_groupObjects.RemoveAll();

			for (int j = 0; j < c; j++)
			{
				long objID;
				ar >> objID;
				o2 = new CObj();

				o2->DuplicateSerialize = DuplicateSerialize;

				o2->Serialize(ar);
				o2->m_group = this;

				if(DuplicateSerialize)	// If we are making a duplicate this will be true, and the new object in the group will have a new number automatically
				{
					objID = g_pApp->m_objID++;					
				}

				o2->editObject->instanceID = objID;
				level->objects[objID] = o2;
				m_groupObjects.AddTail(objID);
				
			}
		}

		OINFO* oInfo = NULL;
		
		if (name != "")
			oInfo = GetPlugin(name).m_pInfo;

		// Serialize object data
		ar >> m_tempName;

		if(oInfo)
			oInfo->ETCreateObject(editObject, g_pFrame);

		// oInfo NULL for group case
		/*
		else
		{
			CErrorDlg Dlg;
			Dlg.Error("Error creating object", "The required plugin was not found while loading an object.");
			return false;
		}
		*/
		
		if (!m_bIsGroup) {
			
			// We are serializing from an archive, but the SDK accepts
			// a bin class, so we read to a buffer, attach the bin to
			// the buffer and pass that to the SDK.
			size_t datasize;
			ar >> datasize;
			BYTE* ptr = new BYTE[datasize];
			ar.Read(ptr, datasize);

			bin b(g_allocator);
			b.loading = true;
			b.attach(ptr, datasize);

			oInfo->ETSerialize(editObject, b);

			b.detach();

			delete[] ptr;
		}

		if (!m_ShowAngles) editObject->objectAngle = 0;

		// Load values
		m_values.clear();
		int count;
		ar >> count;
		for (int i = 0; i < count; i++) {
			CString str;
			int id;
			ar >> id;
			ar >> str;
		
			m_values.push_back(InitValue(str,id));
		}

		// Load the saved expand states
		expandSave.clear();
		ar >> count;
		for (int i = 0; i < count; i++) {
			CString name;
			bool expanded;
			ar >> name;
			ar >> expanded;
			expandSave[name] = expanded;
		}		
	}

	// Saving
	else
	{
		ar << editObject->objectX << editObject->objectY << editObject->objectWidth << 
			editObject->objectHeight << editObject->originX << editObject->originY << editObject->objectAngle
			<< editObject->filter;
		
		/*
			OINFO* o = GetOINFO(editObject->DLLIndex);
			CString CStr = o->extName;
			ASSERT(CStr != "");
			ASSERT(o);

			ar << CStr;
			*/

		ar << m_bIsGroup;

		if (!m_bIsGroup) {
			const CPlugin& plugin = GetPlugin(editObject->DLLIndex);

			// Empty plugin name - shouldnt be allowed
			if (plugin.m_Name == "")
				throw runtime_error("An invalid plugin name was encountered");

			ar << plugin.m_Name;
		}
			
		ar << editObject->instanceID;
		ar << editObject->ObjectIdentifier;
		ar << m_bLocked;
		ar << m_bVisible;
		ar << m_AllowSize;
		ar << m_Draw;
		ar << m_ShowAngles;
		ar << m_bInGroup;
		ar << m_bIsGroup;
		ar << m_bGroupOpen;

		// Serialize virtual edittime
		long tempPointer = (long)editObject->peditInfo;
		ar << tempPointer;

		if (m_bInGroup) {
			long groupID = m_group->GetInstanceID();
			ar << groupID;
		}

		if (m_bIsGroup) {
			POSITION pos = m_groupObjects.GetHeadPosition();
			int c = m_groupObjects.GetCount();
			ar << c;
			CObj *o2;
			for (int j = 0; j < c; j++) {
				long objID = m_groupObjects.GetNext(pos);
				o2 = g_pLayoutView->GetObject(objID);
				ar << objID;
				o2->m_group = this;
				o2->Serialize(ar);
			}
		}		
				
		// Serialize object data
		ar << m_tempName;

		if (!m_bIsGroup) {
			// We are serializing to an archive, but the SDK uses a bin class.
			// Save to a temporary bin, then write the bin to the archive.
			bin b(g_allocator);
			b.allocate(100);
			b.loading = false;
			const CPlugin& plugin = GetPlugin(editObject->DLLIndex);
			plugin.m_pInfo->ETSerialize(editObject, b);
			ar << b.size();
			ar.Write(b.ptr(), b.size());
		}

		// Save values
		ar << m_values.size();
		for (int i = 0; i < m_values.size(); i++) {
			ar << m_values[i].ID;
			ar << m_values[i].value;
		}

		// Save the expand states
		ar << expandSave.size();

		map<CString, bool>::iterator e = expandSave.begin();
		map<CString, bool>::const_iterator expandSave_end = expandSave.end();

		for ( ; e != expandSave_end; e++) {
			ar << e->first;
			ar << e->second;
		}
	}

	SerializeClassPadding(5, 10, ar);

	return true;
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
//#define Rad(a) float((a) / 57.29577951308f)
//#define Deg(a) float((a) * 57.29577951308f)

//			0   1
//			2   3
//
	float sin_a, cos_a;
	cr::sincosf(Rad(-a), &sin_a, &cos_a);

	double m[2][2];
	m[0][0] = cos_a;
	m[1][0] = -sin_a;
	m[0][1] = sin_a;
	m[1][1] = cos_a;

	double source[4][2];
	source[0][0] = -w*(ox);
	source[0][1] = -h*(oy);
	source[1][0] = w*(1-ox);
	source[1][1] = -h*(oy);
	source[2][0] = -w*(ox);
	source[2][1] = h*(1-oy);
	source[3][0] = w*(1-ox);
	source[3][1] = h*(1-oy);

	for(int i = 0; i < 4; i++)
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
	
	for(int i = 1; i < 4; i++)
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
	//CRect rc = GetBoundingRect();

	//fail if not in bounding rect immediately
	//if (!::PtInRect(&rc, point))
	//	return false;



//			0   1
//			2   3
//

	// Step one: Create a rectangle where the hotspot is 0,0
	double source[4][2];
	source[0][0] = -w*(ox);
	source[0][1] = -h*(oy);
	source[1][0] = w*(1-ox);
	source[1][1] = -h*(oy);
	source[2][0] = -w*(ox);
	source[2][1] = h*(1-oy);
	source[3][0] = w*(1-ox);
	source[3][1] = h*(1-oy);

	double l = source[0][0];
	double r = source[0][0];
	double t = source[0][1];
	double b = source[0][1];
	
	for(int i = 1; i < 4; i++)
	{
		double lx = source[i][0];
		double ly = source[i][1];

		if(l > lx) l = lx;
		if(r < lx) r = lx;
		if(t > ly) t = ly;
		if(b < ly) b = ly;
	}

	// Okay the above could have been done better... I'll get back to it later

	// Step 2. Work out the mouse cursor in respect to the object
	CPoint mouse = point;
	mouse.Offset(-x, -y);

	// Step 3...we are going to rotate the mouse
	double m[2][2];

	float sin_a, cos_a;
	cr::sincosf(Rad(a), &sin_a, &cos_a);

	m[0][0] = cos_a;
	m[1][0] = -sin_a;
	m[0][1] = sin_a;
	m[1][1] = cos_a;

	double x = mouse.x;
	double y = mouse.y;
	mouse.x = x * m[0][0] + y * m[0][1];
	mouse.y = x * m[1][0] + y * m[1][1];

	// Step 4...test if its in the box

	CRect box(l,t,r,b);
	return box.PtInRect(mouse);

	// TODO: actually program this to check if a point is in a rotated box...not the bounding box
	//return GetBoundingRect().PtInRect(point);
}
#define POW2(x) ((x)*(x))

void CObjectRectangle::CalculateBlitValues(double& _x, double& _y, double& _w, double& _h, double& _a)
{
	double HotSpotAngle = atan2(oy*h, ox*w);
	double HotSpotDist = sqrt((ox*w)*(ox*w) + (oy*h)*(oy*h));

	float sin_a, cos_a;
	cr::sincosf(Rad(a) + HotSpotAngle, &sin_a, &cos_a);
	float dx = HotSpotDist * cos_a;
	float dy = HotSpotDist * sin_a;
		
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

	float sin_a, cos_a;
	cr::sincosf(Rad(-a), &sin_a, &cos_a);

	pointx +=  cos_a * hor;
	pointy += -sin_a * hor;

	// vertical

	double vert = ((-(h) * oy) + (h)*yratio + (distance*(-0.5+yratio))*2)+0.5-yratio;
	pointx += sin_a * vert;
	pointy += cos_a * vert;





	return CPoint(floor(pointx-0.01), floor(pointy-0.01));
}