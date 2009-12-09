#include "StdAfx.h"
#include "mp3.h"

Mp3::Mp3()
{
	pif = NULL;
	pigb = NULL;
	pimc = NULL;
	pimex = NULL;

	ready = false;

	CoInitialize(NULL);
}

Mp3::~Mp3()
{
	Cleanup();
	CoUninitialize();
}

void Mp3::Cleanup()
{
	if (pimc)
		pimc->Stop();

	if(pimc)
	{
		pimc->Release();
		pimc = NULL;
	}

	if(pimex)
	{
		pimex->Release();
		pimex = NULL;
	}

	if(pigb)
	{
		pigb->Release();
		pigb = NULL;
	}
}

void Mp3::Load(LPCSTR szFile)
{
	WCHAR wFile[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, szFile, -1, wFile, MAX_PATH);

	if (SUCCEEDED(CoCreateInstance( CLSID_FilterGraph,
		 NULL,
		 CLSCTX_INPROC_SERVER,
		 IID_IGraphBuilder,
		 (void **)&this->pigb)))
	{
		pigb->QueryInterface(IID_IMediaControl, (void **)&pimc);
		pigb->QueryInterface(IID_IMediaEventEx, (void **)&pimex);
		pigb->QueryInterface(IID_IMediaSeeking, (void **)&pims);
		pigb->QueryInterface(IID_IBasicAudio, (void **)&piba);

		if (SUCCEEDED(pigb->RenderFile(wFile, NULL)))
			ready = true;
	}
}

void Mp3::Play()
{
	if (ready) pimc->Run();
}

void Mp3::Pause()
{
	if (ready) pimc->Pause();
}

void Mp3::Stop()
{
	if (ready) pimc->Stop();
}

void Mp3::SetVolume(double vol)
{
	if (ready) piba->put_Volume((vol - 100.0) * 100);
}

void Mp3::CheckLoop()
{
	if (ready) {
		long eventCode;
		long p1, p2;
		if (pimex->GetEvent(&eventCode, &p1, &p2, 0) == S_OK) {
			if (eventCode == EC_COMPLETE) {
				LONGLONG pos = 0;
				pims->SetPositions(&pos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
				pimc->Run();
			}
		}
	}
}