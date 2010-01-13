#include "StdAfx.h"
#include "CAnimation.h"

CAnimation::CAnimation()
{
	m_FixedID = -1;	// when adding animations, remember to give a unique number

	m_Name = "Animation";
	m_Description = "";

	m_Repeat = 1;
	m_bForever = true;
	m_bPingPong = false;

	m_Speed = 50;
	m_RepeatTo = 1;

	m_AllowRemove = true;
	m_AllowSubanimations = true;
	m_AllowAddRemoveFrame = true;
	m_AllowRenameFrame = false;
	m_AllowAutoButton = false;

	m_IsAngle = false;
	m_Angle = 0;

	application = 0;

	tag = 0;

	m_Expanded = true;
}

void CAnimation::SerializeFromOtherApplicationStep(CApplication* app)
{
		application = app;

		m_FixedID = app->m_AnimationFixID++;
		// This function is used when an animation has been serialized for use in another application
		// Basically the images themselves get loaded later, but we need to allocate blank images first.
		//If the application isn't already in the map, we create it...it doesn't matter
		for(int f = 0; f < m_Images.size(); f++)
		{
			int& newimage = m_Images[f];
			map<int, int>& imagefromto = app->m_imagefrom_imageto_map[app->m_drag_app_from];

			// now the important part...we need to see if we've done this before
			if( imagefromto.find(newimage) == imagefromto.end())
			{
				//damn it hasn't been done before...we have work to do
				app->resources.images.push_back(CImageResource());
				app->resources.images_changed = true;

				if(app->resources.FindImageResourceFromNumber(newimage))
				{
					app->images_to_save[newimage] = 0; 
				}
				newimage = imagefromto[newimage] = app->m_ImageFixID++;
				app->resources.images.back().m_FixedID = newimage;
				// we use the imagefromto map later to put our bitmaps that we serialized at the end of CDragObjects into
				// our blank imageBank spots
			}
			else
			{
				// Yeah! we've been here and done it! 
				newimage = imagefromto[newimage];
			}
		}
		for(list<CAnimation>::iterator a = m_SubAnimations.begin(); a != m_SubAnimations.end(); a++)
			a->SerializeFromOtherApplicationStep(app);

	}

CAnimation* CAnimation::GetAngle(int angle)
{
	for(list<CAnimation>::iterator i = m_SubAnimations.begin(); i!= m_SubAnimations.end(); i++)
	{
		if(i->m_Angle == angle && i->IsAngle())
			return &*i;
	}

	return NULL;

}

void CAnimation::GetImagesToSave(map<int, CObj*>& images)
{
	for (vector<int>::iterator i = m_Images.begin(); i != m_Images.end(); i++)
	{
		if(application->resources.FindImageResourceFromNumber(*i))
		{
			images[*i] = 0;
		}
	}
	for( list<CAnimation>::iterator i = m_SubAnimations.begin(); i!= m_SubAnimations.end(); i++)
	{
		i->GetImagesToSave(images);
	}
}

bool CAnimation::Serialize(CArchive& Archive)
{
	CString ExpectedName = "CAnimation";
	int     Version      = 3;

	if (!SerializeClassHeader(ExpectedName, Version, Archive))
		return false;

	if(Archive.IsLoading())
	{
		Archive >> m_FixedID >> m_Name;
		if(Version >= 3)
			Archive >> m_Speed;
		else
		{
			int speed;
			Archive >> speed;
			m_Speed = speed;
		}
		
		Archive >> m_Repeat >> m_RepeatTo >> m_bForever >> m_bPingPong >> m_IsAngle >> m_Angle >> m_Expanded;
		
		int Count = 0;
		Archive >> Count;
		for (int i = 0; i < Count; i++)
		{
			int frame;
			Archive >> frame;
			m_Images.push_back(frame);
		}

		Archive >> Count;
		for (int i = 0; i < Count; i++)
		{
			float time;
			Archive >> time;
			m_FrameTimes.push_back(time);
		}

		Archive >> Count;
		for (int i = 0; i < Count; i++)
		{
			m_SubAnimations.push_back(CAnimation());
			m_SubAnimations.back().application = application;
			m_SubAnimations.back().Serialize(Archive);
		}

		if (Version >= 2)
			Archive >> tag;
	}

	else
	{
		Archive << m_FixedID << m_Name << m_Speed << m_Repeat << m_RepeatTo << m_bForever << m_bPingPong << m_IsAngle << m_Angle << m_Expanded;
		
		int Count = m_Images.size();
		Archive << Count;
		for (vector<int>::iterator i = m_Images.begin(); i != m_Images.end(); i++)
		{
			Archive << *i;
			if(application->resources.FindImageResourceFromNumber(*i))
			{
				application->images_to_save[*i] = 0;
			}

		}

		Count = m_FrameTimes.size();
		Archive << Count;
		for (vector<float>::iterator i = m_FrameTimes.begin(); i != m_FrameTimes.end(); i++)
		{
			Archive << *i;
		}

		Count = m_SubAnimations.size();
		Archive << Count;
		for( list<CAnimation>::iterator i = m_SubAnimations.begin(); i!= m_SubAnimations.end(); i++)
		{
			i->Serialize(Archive);
		}

		Archive << tag;
	}

	return true;
}

bool CAnimation::supportsFrames()
{
	// 3 years ago I made a nice system where animations could specify if they were suppost to have 
	// frames added to them or not, but sadly some code was lost and the information wasn't serialized
	// and the animation editor that originally supported it was replaced with the animation bar which 
	// didn't support it...so now I simply determine if an animation supports frames by simply checking
	// it already has at least one.

	if(m_Images.size() > 0)
		return true;
	else
		return false;

}