// pacEngine Test.cpp : Defines the entry point for the console application.
#include <windows.h>
#include "stdafx.h"
#include "audio.cpp"


int _tmain(int argc, _TCHAR* argv[])
{
	char *s, *t;
	PACHANDLE ret1;
	PACHANDLE ret2;
	pacAudioStartService(0x0FFFFFFF, 6, 4096);

	printf("Audio Manufacturer ID: %d\n", pacAudioNumDevices);
	printf("Audio Product Name: %s\n\n", pacAudioDevice.szPname);
	printf("Enter filename: ");
	printf("c.wav\n");
	s = _T("c.wav");
	t = _T("b.wav");
//	scanf("%[^\n]s", s);
//	scanf("%c", &c);

	PACKET_FILE_NAME	pack1;
	pack1.PacketSize = sizeof(PACKET_FILE_NAME);
	pack1.PacketType = 0x0027;
	pack1.DataType = DATA_TYPE_AUDIO_WAVE;
	pack1.FileName = s;
	ret1 = pacAudioOpen((PACKET*)&pack1);
	pack1.FileName = t;
	ret2 = pacAudioOpen((PACKET*)&pack1);
	printf("_tmain::ret = %d\n", ret1);
	while(!(ret1->PacketType));		// Wait to Load
	while(!(ret2->PacketType));
	printf("_tmain::ret = %d\n", ret1);
	pacAudioPlay(ret2);
	pacAudioPlay(ret1);				// Play Sound
	while(1);
	pacAudioStopService();
	system("PAUSE");
	return 0;
}
