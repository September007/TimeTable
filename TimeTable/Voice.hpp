#pragma once

/* �������� */

#include <sapi.h> //��������ͷ�ļ�
#pragma comment(lib,"sapi.lib") //��������ͷ�ļ���
#include"inc.hpp"
#include"cvt.h"//����ת��
//com�ӿ�

inline void speak(wstring s) {
    static  bool isInited = false;
	static birthDeath sta_com_initializer_(nullptr, []() {
		if (isInited)
			//�ͷ�com��Դ
			::CoUninitialize();
        });
    if (!isInited) {
        //��ʼ��COM�ӿ�
        if (FAILED(::CoInitialize(NULL)))
            MessageBox(NULL, (LPCWSTR)L"COM�ӿڳ�ʼ��ʧ�ܣ�", (LPCWSTR)L"��ʾ", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
        else
            isInited = true;
    }
    ISpVoice* pVoice = NULL;
    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
    if (SUCCEEDED(hr))
    {
        pVoice->SetVolume((USHORT)100); //������������Χ�� 0 -100
        pVoice->SetRate(0); //�����ٶȣ���Χ�� -10 - 10
        hr = pVoice->Speak(s.data(), 0, NULL);
        pVoice->Release();

        pVoice = NULL;
    }
    return;
}
inline void speak(const string& s) {
    speak(s2ws(s));
}

/*wav����*/
#if defined(WAV_PLAY)
#include<windows.h>
#include<Mmsystem.h>
#pragma comment(lib,"winmm.lib")
#endif
