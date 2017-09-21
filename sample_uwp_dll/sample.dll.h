#pragma once

#include <windows.h>
#include <initguid.h>
#include <stdio.h>

namespace SampleUwpDll
{
	public ref class SampleUwpNative sealed
	{
	public:
		void start();
		void onClickDown(int x, int y);
		void onClickUp(int x, int y);
		void getFrameBuffer(Platform::WriteOnlyArray<byte>^ data);
		int inputWavBuffer(const Platform::Array<byte>^ data, int sampleRate);
	};
}
