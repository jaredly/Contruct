// stdafx.cpp : source file that includes just the standard includes

#include "stdafx.h"

/*
	Construct - DirectX 9 Game and Application creation
    Copyright (C) 2007 Scirra

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#pragma warning(disable: 4786) // debug info truncated, nobody cares kthx

CPrecisionTimer Pres;

void CheckSerial(CArchive& ar)
{
	if(ar.IsLoading())
	{
		int a;
		ar >> a;
		if(a != 123456789)
			__asm int 3;
	}
	else
		ar << (int)123456789;
}

/*
void SerializeACESEntry(ACESEntry* entry,  CArchive& ar )
{
	if(ar.IsLoading())
	{
		ar >> entry->resourceID;
		ar >> entry->aceName;
		ar >> entry->aceDisplayText;
		ar >> entry->retrn;
		int size;
		ar >> size;
		for(int x = 0; x < size; x++)
		{
			PARAM parameter;
			ar >> parameter.type;
			ar >> parameter.name;
			ar >> parameter.desc;
			ar >> parameter.initStr;

			entry->params.push_back(parameter);
		}

	}
	else
	{
		ar << entry->resourceID;
		ar << entry->aceName;
		ar << entry->aceDisplayText;
		ar << entry->retrn;
		int size = entry->params.size();
		ar << size;
		for(int x = 0; x < size; x++)
		{
			PARAM& parameter = entry->params[x];
			ar << parameter.type;
			ar << parameter.name;
			ar << parameter.desc;
			ar << parameter.initStr;
		}


	}

};
*/

void RemapACEMenu(CMenu* pMenu, int& count, ACEMenuMap& map, int type, int child)
{


	CString string;

	int x = -1;	
	MENUITEMINFO MenuItemInfo;
	MenuItemInfo.cbSize = sizeof (MENUITEMINFO); // must fill up this field
   MenuItemInfo.fMask = MIIM_ID;             // get the state of the menu item


	do
	{
		count++;
		x++;

		MenuMapItem myitem;
		myitem.type = type;
		myitem.menuid = pMenu->GetMenuItemID(x);
		myitem.child = child;
		
		// loop for submenu
		CMenu* pSubMenu = pMenu->GetSubMenu(x);
		if(pSubMenu)
			RemapACEMenu(pSubMenu, count, map, type, child);
		
		map.SetAt(count, myitem);

		pMenu->GetMenuItemInfo(x,&MenuItemInfo,TRUE);
		MenuItemInfo.wID = count;


	}while(pMenu->SetMenuItemInfo(x,&MenuItemInfo,TRUE));



}


void CopyMenu(CMenu* src, CMenu* dest)
{

    // get the state of the menu item
	dest->CreatePopupMenu();


	int size = src->GetMenuItemCount();
	for(int x = 0; x < size; x++)
	{
		// loop for submenu
		CMenu* pSrcSubMenu = src->GetSubMenu(x);
		if(pSrcSubMenu)
		{
			CMenu* pDestSubMenu = new CMenu;
			
			CopyMenu(pSrcSubMenu, pDestSubMenu);

			CString string;
			src->GetMenuString(x,string,MF_BYPOSITION);
			UINT state = src->GetMenuState(x,MF_BYPOSITION) & 0xff;
			dest->AppendMenuA(state,(UINT_PTR)pDestSubMenu->GetSafeHmenu(), string);


		}
		else
		{
			CString string;
			src->GetMenuString(x,string,MF_BYPOSITION);
			UINT state = src->GetMenuState(x,MF_BYPOSITION);
			dest->AppendMenuA(state,src->GetMenuItemID(x), string);
		}


	}




}