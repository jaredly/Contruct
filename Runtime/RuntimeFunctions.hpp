#ifndef RUNTIMEFUNCTIONS_H
#define RUNTIMEFUNCTIONS_H

#include "Mmsystem.h"
#include <io.h>

#pragma comment(lib, "winmm.lib")

class CSystemObj
{
public:
	/*
	void AnimateWindow(HWND wnd, int time, LPCSTR flags)
	{
		DWORD flag = NULL;
		if (strstr(flags, "+h") != NULL) flag |= AW_HIDE;
		if (strstr(flags, "+s") != NULL) flag |= AW_SLIDE;
		if (strstr(flags, "+a") != NULL) flag |= AW_ACTIVATE;
		if (strstr(flags, "+b") != NULL) flag |= AW_BLEND;
		if (strstr(flags, "+hp") != NULL) flag |= AW_HOR_POSITIVE;
		if (strstr(flags, "+hn") != NULL) flag |= AW_HOR_NEGATIVE;
		if (strstr(flags, "+vp") != NULL) flag |= AW_VER_POSITIVE;
		if (strstr(flags, "+vn") != NULL) flag |= AW_VER_NEGATIVE;
		::AnimateWindow(wnd, time, flag);
	}
	*/

	static void CopyFile(LPCSTR from, LPCSTR to, LPCSTR flags = "")
	{
		bool flagt = false;
		if (strstr(flags, "f") != NULL) flagt = true;
		::CopyFile(from, to, flagt);
	}

	static void BringWindowToTop(HWND wnd) { ::BringWindowToTop(wnd); }
	static void CreateDirectory(LPCSTR toCreate) { ::CreateDirectory(toCreate, NULL); }
	static void DeleteDirectory(LPCSTR dirPath)  { ::RemoveDirectory(dirPath); }
	static void DeleteFile(LPCSTR fileName) { ::DeleteFile(fileName); }
	static void DestroyWindow(HWND wnd) { ::DestroyWindow(wnd); }
	static HWND FindWindow(LPCSTR text) { return ::FindWindow(NULL, text); }

	static CString GetClipboardText()
	{
		CString ret;

		if (OpenClipboard(NULL)) 
		{
			HANDLE hData = GetClipboardData(CF_TEXT);
			char* buffer = (char*)GlobalLock(hData);
			ret = buffer;
			GlobalUnlock(hData);
			CloseClipboard();
		}

		return ret;
	}

	static int GetNumberOfSubstrings(LPCSTR string, LPCSTR substrings, int startpos)
	{
		LPTSTR toFind = (char*)string + startpos;
		LPTSTR nextString;
		int	stringCount = 0;
		int	stringSize = strlen(substrings);
	
		nextString = strstr(toFind, substrings);
		while (nextString)
		{
			stringCount++;
			toFind = nextString + stringSize;
			nextString = strstr(toFind, substrings);
		}

		return stringCount;
	}

	//static void GetSubstring(LPCSTR string, int substr, CString& ret, TCHAR sep) { AfxExtractSubString(ret, string, substr, sep); }

	static CString GetWindowText(HWND wnd) 
	{ 
		char* buf = new char[1024];
		::GetWindowText(wnd, buf, 1023);
		CString ret = buf;
		delete[] buf;
		return ret;
	}

	//bool IsUserAdmin() { if (IsUserAnAdmin()) return true; return false; }
	static bool IsWindowVisible(HWND wnd) { return ::IsWindowVisible(wnd) ? true : false; }
	static void Lowercase(LPSTR text) { _strlwr(text); }

	static int MessageBox(LPCSTR text, LPCSTR title, LPCSTR flags)
	{
		DWORD tflags = NULL;
		if (strstr(flags, "+art") != NULL) { tflags |= MB_ABORTRETRYIGNORE; }
		//if (strstr(flags, "+crt") != NULL) { tflags |= MB_CANCELRETRYCONTINUE; }
		if (strstr(flags, "+o") != NULL) { tflags |= MB_OK; }
		if (strstr(flags, "+oc") != NULL) { tflags |= MB_OKCANCEL; }
		if (strstr(flags, "+rc") != NULL) { tflags |= MB_RETRYCANCEL; }
		if (strstr(flags, "+yn") != NULL) { tflags |= MB_YESNO; }
		if (strstr(flags, "+ync") != NULL) { tflags |= MB_YESNOCANCEL; }
		if (strstr(flags, "+ex") != NULL) { tflags |= MB_ICONEXCLAMATION; }
		if (strstr(flags, "+w") != NULL) { tflags |= MB_ICONWARNING; }
		if (strstr(flags, "+i") != NULL) { tflags |= MB_ICONINFORMATION; }
		if (strstr(flags, "+a") != NULL) { tflags |= MB_ICONASTERISK; }
		if (strstr(flags, "+q") != NULL) { tflags |= MB_ICONQUESTION; }
		if (strstr(flags, "+s") != NULL) { tflags |= MB_ICONSTOP; }
		if (strstr(flags, "+er") != NULL) { tflags |= MB_ICONERROR; }
		return ::MessageBox(NULL, text, title, tflags);
	}

	static void SetWindowText(HWND wnd, LPCSTR text) { ::SetWindowText(wnd, text); }
	static void ShowWindow(HWND wnd, bool show) { ::ShowWindow(wnd, show ? SW_SHOW : SW_HIDE); }

	static ULONG SizeOfFile(LPCSTR filepath)
	{
		FILE *pf = fopen(filepath, "rb");
		ULONG size = _filelength(_fileno(pf));
		fclose(pf);
		return size;
	}

	static CString ReadINI(LPCSTR iniFile, LPCSTR section, LPCSTR item)
	{
		char* readBuf = new char[1024];
		GetPrivateProfileString(section, item, "", readBuf, 1023, iniFile);
		CString ret = readBuf;
		delete[] readBuf;
		return ret;
	}

	static void PlaySound(LPCSTR fileToPlay, LPCSTR flags)
	{
		DWORD tflags = SND_FILENAME;
		if (strstr(flags, "+a") != NULL) { tflags |= SND_ASYNC; }
		if (strstr(flags, "+e") != NULL) { tflags |= SND_NOSTOP; }
		if (strstr(flags, "+l") != NULL) { tflags |= SND_LOOP; }
		if (strstr(flags, "+s") != NULL) { tflags |= SND_PURGE; }
		::PlaySound(fileToPlay, NULL, tflags);
	}

	static CString ReadFile(LPCSTR filePath, __int64 startAt, int readBytes)
	{
		char* readBuf = new char[readBytes + 1];
		FILE* op = fopen(filePath, "r");
		fsetpos(op, &startAt);
		fread(readBuf, 1, readBytes, op);
		fclose(op);
		CString ret = readBuf;
		delete[] readBuf;
		return ret;
	}

	static void SetClipboardText(LPCSTR toPut)
	{
		if(OpenClipboard(NULL))
		{
			HGLOBAL clipbuffer;
			char* buffer;
			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(toPut) +1);
			buffer = (char*)GlobalLock(clipbuffer);
			strcpy(buffer, toPut);
			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_TEXT, clipbuffer);
			CloseClipboard();
		}
	}

	static void ShellExecute(LPCSTR file, LPCSTR params, LPCSTR flags)
	{
		int showflags = NULL;
		CString op = "";
		if (strstr(flags, "+ed") != NULL) op = "edit";
		if (strstr(flags, "+ex") != NULL) op = "explore";
		if (strstr(flags, "+f") != NULL) op = "find";
		if (strstr(flags, "+o") != NULL) op = "open";
		if (strstr(flags, "+s") != NULL) showflags = SW_SHOW;
		if (strstr(flags, "+h") != NULL) showflags = SW_HIDE;
		if (strstr(flags, "+mi") != NULL) showflags = SW_MINIMIZE;
		if (strstr(flags, "+ma") != NULL) showflags = SW_MAXIMIZE;
		//::ShellExecute(NULL, op, file, params, NULL, showflags);
	}

	static void WriteFile(LPCSTR filePath, __int64 startAt, int len, const char* toWrite)
	{
		FILE* op = fopen(filePath, "w");
		fsetpos(op, &startAt);
		fwrite(toWrite, 1, len, op);
		fclose(op);
	}

	static void WriteINI(LPCSTR iniFile, LPCSTR section, LPCSTR item, LPCSTR towrite) { WritePrivateProfileString(section, item, towrite, iniFile); }
	static void Uppercase(LPSTR text)	{ _strupr(text); }
};

#endif // Only include once