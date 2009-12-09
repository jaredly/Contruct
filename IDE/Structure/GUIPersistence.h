// GUIPersistence.h
// persists a set of windows related to an application

#pragma once

class GUIPersistence
{
public:

	GUIPersistence(CMainFrame* main_frame_, CApplication* application_) :
		main_frame(*main_frame_),
		application(application_)
	{
	}

	bool SavePersistence(std::string&);
	bool LoadPersistence(std::string&);

	CMainFrame& main_frame;
	CApplication* application;
};
