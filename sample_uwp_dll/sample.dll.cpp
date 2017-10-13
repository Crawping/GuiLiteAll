#include "pch.h"
#include "sample.dll.h"
#include <assert.h>

using namespace SampleUwpDll;
using namespace concurrency;

typedef struct
{
	unsigned int dwMsgId;
	unsigned int dwParam1;
	unsigned int dwParam2;
}OUTMSGINFO;

extern int run(int main_cnt, int sub_cnt);
extern "C" void* get_frame_buffer(int display_id);
extern "C" int send_hid_msg(void* buf, int len, int display_id);

void SampleUwpNative::start()
{
	create_async([this]() {
		run(1, 0);
	});
}

void SampleUwpNative::getFrameBuffer(Platform::WriteOnlyArray<byte>^ data)
{
	unsigned short* raw_data = (unsigned short*)get_frame_buffer(0);

	for(int i = 0; i < data->Length; i += 4)
	{
		unsigned short rgb = *raw_data++;

		data[i + 3] = 0xff;//transport
		data[i] = ((rgb << 3) & 0xF8); 
		data[i + 1] = ((rgb >> 3) & 0xFC);
		data[i + 2] = ((rgb >> 8) & 0xF8);
	}
}

int SampleUwpNative::inputWavBuffer(const Platform::Array<byte>^ data, int sampleRate)
{
	return 0;
}

void SampleUwpNative::onClickDown(int x, int y)
{
	OUTMSGINFO msg;
	msg.dwMsgId = 0x4700;
	msg.dwParam1 = x | (y << 16);
	send_hid_msg(&msg, sizeof(msg), 0);
}

void SampleUwpNative::onClickUp(int x, int y)
{
	OUTMSGINFO msg;
	msg.dwMsgId = 0x4600;
	msg.dwParam1 = x | (y << 16);
	send_hid_msg(&msg, sizeof(msg), 0);
}

void do_assert(const char* file, int line)
{
	assert(false);
}

void log_out(const char* log)
{
	OutputDebugStringA(log);
}