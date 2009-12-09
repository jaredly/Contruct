#include <windows.h>
#include <mmsystem.h>
#include <DShow.h>

class Mp3
{
private:
	IBaseFilter   *  pif;
	IGraphBuilder *  pigb;
	IMediaControl *  pimc;
	IMediaEventEx *  pimex;
	IMediaSeeking *  pims;
	IBasicAudio*	 piba;

	bool    ready;

public:
	Mp3();
	~Mp3();

	void Load(LPCSTR filename);
	void Cleanup();

	void Play();
	void Pause();
	void Stop();
	void SetVolume(double vol);
	bool IsReady() { return ready; }
	void CheckLoop();

	IMediaSeeking* GetMediaSeeker() { return pims; }
};

