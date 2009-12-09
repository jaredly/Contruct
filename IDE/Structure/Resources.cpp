#include "StdAfx.h"
#include "Resources.h"
#include "..\MainFrm.h"

extern CMainFrame* pMainWnd;

void ApplicationResource::SetName(LPCSTR name_)
{
	name = name_;

	pMainWnd->project_bar.tree.SetItemText(tree, name);
}

CString ApplicationResource::GetName()
{
	return name;
}

ResourcesType ApplicationResource::GetType()
{
	return type;
}

void BinaryResource::Preview()
{
	int begin = path.ReverseFind('.');
	CString extension = path.Right(path.GetLength() - begin);

	if (extension == ".wav" || extension == ".ogg")
	{
		CXAudio2SetupParameters setup;
		setup.cacheMode = CXAudio2::CACHE_OFF;

		CXAudio2 audio;

		// initialise audio
		audio.Init(setup);

		audio.LoadFile((const char*)path, 0, false);
		audio.Play(0);

		CExtMsgBox box(pMainWnd->m_hWnd, "Click OK to end the preview", "Previewing sound", __EXT_MB_ICONINFORMATION | __EXT_MB_OK | __EXT_MB_NO_SOUND);
		box.DoModal();

		audio.Stop(0);
		audio.FreeChannel(0);
	}

	if (extension == ".mp3" || extension == ".wma")
	{
		pMainWnd->mp3.Load(path);
		pMainWnd->mp3.Play();

		CExtMsgBox box(pMainWnd->m_hWnd, "Click OK to end the preview", "Previewing music", __EXT_MB_ICONINFORMATION | __EXT_MB_OK | __EXT_MB_NO_SOUND);
		box.DoModal();

		pMainWnd->mp3.Stop();
		pMainWnd->mp3.Cleanup();
	}
}

bool BinaryResource::Serialize(CArchive& ar)
{
	CString expected_name = "CBinaryResource";
	int version = 2;

	if (!SerializeClassHeader(expected_name, version, ar))
		return false;

	if (ar.IsLoading())
	{
		ar >> name >> (int&)type >> path;

		if (version >= 2)
			ar >> id;
	}

	else
		ar << name << (int)type << path << id;

	return true;
}

bool FontResource::Serialize(CArchive& ar)
{
	CString expected_name = "CFontResource";
	int version = 1;

	if (!SerializeClassHeader(expected_name, version, ar))
		return false;

	if (ar.IsLoading())
		ar >> name >> (int&)type;
	else
		ar << name << (int)type;

	return true;
}

bool IconResource::Serialize(CArchive& ar)
{
	CString expected_name = "CIconResource";
	int version = 1;
	
	if (!SerializeClassHeader(expected_name, version, ar))
		return false;

	if (ar.IsLoading())
	{
		ar >> name >> (int&)type;

		long size;
		ar >> size;

		BYTE* buffer = new BYTE[size];
		ar.Read(buffer,size);
		icon.Decode(buffer, size, CXIMAGE_FORMAT_PNG);

		delete[] buffer;
	}

	else
	{
		ar << name << (int)type;

		BYTE* buffer = 0;
		long size;

		icon.Encode(buffer, size, CXIMAGE_FORMAT_PNG);
		ar << size;
		ar.Write(buffer,size);

		icon.FreeMemory(buffer);
	}

	return true;
}

void MenuResource::CreateDefault()
{
}

bool MenuResource::Serialize(CArchive& ar)
{
	CString expected_name = "CMenuResource";
	int version = 1;
	
	if (!SerializeClassHeader(expected_name, version, ar))
		return false;

	if (ar.IsLoading())
	{
		ar >> name >> (int&)type;

		int count = 0;
		ar >> count;

		for (int i = 0; i < count; i++)
		{
			MenuResourceItem item;

			if (!item.Serialize(ar))
				return false;

			items.push_back(item);
		}
	}

	else
	{
		ar << name << (int)type;

		ar << items.size();

		for (int i = 0; i < items.size(); i++)
			items[i].Serialize(ar);
	}

	return true;
}

bool MenuResourceItem::Serialize(CArchive& ar)
{
	CString expected_name = "CMenuResourceItem";
	int version = 1;
	
	if (!SerializeClassHeader(expected_name, version, ar))
		return false;

	if (ar.IsLoading())
	{
		ar >> text;
		ar >> id;

		int count = 0;
		ar >> count;

		for (int i = 0; i < count; i++)
		{
			MenuResourceItem item;
			if (!item.Serialize(ar))
				return false;

			items.push_back(item);
		}
	}

	else
	{
		ar << text;
		ar << id;

		ar << items.size();

		for (int i = 0; i < items.size(); i++)
			items[i].Serialize(ar);
	}

	return true;
}