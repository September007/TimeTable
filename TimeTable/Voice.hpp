#pragma once

/* 语音播放 */

#include <sapi.h> //导入语音头文件
#pragma comment(lib,"sapi.lib") //导入语音头文件库
#include"inc.hpp"
#include"cvt.h"//编码转换
//com接口

inline void speak(wstring s) {
    static  bool isInited = false;
	static birthDeath sta_com_initializer_(nullptr, []() {
		if (isInited)
			//释放com资源
			::CoUninitialize();
        });
    if (!isInited) {
        //初始化COM接口
        if (FAILED(::CoInitialize(NULL)))
            MessageBox(NULL, (LPCWSTR)L"COM接口初始化失败！", (LPCWSTR)L"提示", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
        else
            isInited = true;
    }
    ISpVoice* pVoice = NULL;
    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
    if (SUCCEEDED(hr))
    {
        pVoice->SetVolume((USHORT)100); //设置音量，范围是 0 -100
        pVoice->SetRate(0); //设置速度，范围是 -10 - 10
        hr = pVoice->Speak(s.data(), 0, NULL);
        pVoice->Release();

        pVoice = NULL;
    }
    return;
}
inline void speak(const string& s) {
    speak(s2ws(s));
}

/*wav播放*/
#if defined(WAV_PLAY)
#include<windows.h>
#include<Mmsystem.h>
#pragma comment(lib,"winmm.lib")
#endif
