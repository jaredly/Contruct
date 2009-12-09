// GUIPersistence.cpp
// persists a set of windows related to an application

#include "stdafx.h"
#include "..\LIBS\tinyxml-2.5.3\include\tinyxml.h"
#include "GUIPersistence.h"

#include "..\MainFrm.h"
#include "..\ChildFrame.h"

bool GUIPersistence::SavePersistence(std::string& out)
{
	TiXmlDocument doc;
	TiXmlDeclaration* dec = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(dec);
		
	TiXmlElement* root = new TiXmlElement("persistence");

	TiXmlElement* windows;
	windows = new TiXmlElement("windows");  

	// loop open windows and see what we've got
	HWND client_window = ::GetWindow(main_frame.m_hWndMDIClient, GW_CHILD);
	client_window = ::GetWindow(client_window, GW_HWNDLAST);

	while (client_window != NULL)
	{
		CChildFrame* window = (CChildFrame*)CWnd::FromHandle(client_window);

		if(window->application == application)
		{
			bool layout = false;

			// get the editor we're in
			if (window->m_tabs.ItemGetCount() == 2 && window->m_tabs.SelectionGet() == 0)
				layout = true;

			TiXmlElement* editor;

			if (layout)
			{			
				editor = new TiXmlElement("layout");

				editor->SetAttribute("name", window->layout->GetName());

				editor->SetAttribute("scroll-x", window->layout->scroll_position.x);
				editor->SetAttribute("scroll-y", window->layout->scroll_position.y);
			}

			else
			{
				editor = new TiXmlElement("event-sheet");

				editor->SetAttribute("name", window->event_sheet->GetName());
			}

			if (main_frame.MDIGetActive() == window)
				editor->SetAttribute("active", "yes");
			else
				editor->SetAttribute("active", "no");

			windows->LinkEndChild(editor);
		}

		client_window = :: GetWindow(client_window, GW_HWNDPREV);
	}

	root->LinkEndChild(windows);
	doc.LinkEndChild(root);

	doc.SaveFile(out.c_str());

	return true; // no problems
}

bool GUIPersistence::LoadPersistence(std::string& in)
{
	TiXmlDocument load(in.c_str());
	load.LoadFile();

	TiXmlHandle doc(&load);
	TiXmlHandle root(0);
	
	TiXmlElement* element;
	element = doc.FirstChildElement().Element();
	root = TiXmlHandle(element);

	if (!element)
		return false;

	CChildFrame* open = NULL; // safety first

	TiXmlElement* windows = root.FirstChildElement("windows").Element();
	TiXmlElement* window;

	for (window = windows->FirstChildElement(); window; window = window->NextSiblingElement())
	{
		std::string type = window->Value();
		std::string name = window->Attribute("name");

		if (type == "layout")
		{
			// get a layout for this
			POSITION position = application->layouts.GetHeadPosition();

			while(position)
			{
				CLayout* find_layout = application->layouts.GetNext(position);
				if(find_layout->GetName() == name.c_str())
				{
					// set attributes
					int x, y;
					window->QueryIntAttribute("scroll-x", &x);
					window->QueryIntAttribute("scroll-y", &y);

					find_layout->scroll_position.x = x;
					find_layout->scroll_position.y = y;

					// open layout
					main_frame.project_bar.tree.Select(find_layout->tree, TVGN_CARET);
					main_frame.project_bar.OnDoubleClick(NULL, NULL);
				}
			}
		}

		else
		{
			// get an event_sheet for this
			for (list<EventSheet*>::iterator i = application->event_sheets.begin(); i != application->event_sheets.end(); i++)
			{
				if ((*i)->GetName() == name.c_str())
				{
					// open layout
					main_frame.project_bar.tree.Select((*i)->second_tree, TVGN_CARET);
					main_frame.project_bar.OnDoubleClick(NULL, NULL);
				}
			}
		}

		if (strcmp(window->Attribute("active"), "yes"))
			open = main_frame.project_bar.last_opened;
	}	

	// select the open tab
	if (open)
		main_frame.MDIActivate(open);

	return true; // no problems
}
