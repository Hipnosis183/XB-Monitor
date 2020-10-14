// XB-Monitor: Open-source Examu eX-BOARD loader by Romhack and Hipnosis.

#include <vector>

#include "JVSEmu.h"

static int isAddressed = 0;

int IsAddressed()
{
	return isAddressed;
}

void ResetAddressed()
{
	isAddressed = 0;
}

static std::vector<char> Reply(1024);

VOID SaveSRAM()
{
	FILE* Stream = NULL;
	Stream = fopen(SRAM_NAME, "wb");

	if (!Stream)
	{
		return;
	}

	fwrite(SRAM, 1, SRAM_SIZE, Stream);
	fclose(Stream);
}

VOID LoadSRAM()
{
	FILE* Stream = NULL;
	Stream = fopen(SRAM_NAME, "rb");

	memset(SRAM, 0, SRAM_SIZE);

	if (!Stream)
	{
		return;
	}

	fread(SRAM, 1, SRAM_SIZE, Stream);
	fclose(Stream);
}

DWORD ProcessStream(UINT8* Stream, DWORD srcSize, BYTE* dAddr, DWORD dSize)
{
	Reply.clear();

	switch (Stream[1])
	{
		case 0xFA:
		{
			unsigned int Addr = 0;
			unsigned int Size = 0;

			Reply.push_back(0x76);
			Reply.push_back(0xFA);
			Reply.push_back(0x05);
			Reply.push_back(0x70);
			Reply.push_back(0x42);

			Addr = (Stream[3] << 8) | (Stream[4]);
			Size = Stream[5];

			if (Size >= (srcSize - 1))
			{
				Size = srcSize - 1;
			}

			if ((Addr + Size) >= 0xFFFF)
			{
				Size = 0xFFFF - Addr;
			}

			memcpy(&SRAM[Addr], &Stream[6], Size);
			break;
		}

		case 0xFB:
		{
			Reply.push_back(0x76);
			Reply.push_back(0xFB);

			unsigned int Addr = 0;
			unsigned int Size = Stream[5];
			unsigned rPos = Reply.size();

			Reply.push_back(5);

			Addr = (Stream[3] << 8) | (Stream[4]);
			Size = Stream[5];

			if ((Addr + Size) >= 0xFFFF)
			{
				Size = 0xFFFF - Addr;
			}

			for (int i = 0; i < Size; i++)
			{
				Reply.push_back(SRAM[Addr++]);
			}

			Reply[rPos] += Size;

			Reply.push_back(0x70);
			Reply.push_back(0x42);

			break;
		}

		case 0xFE:
		{
			Reply.push_back(0x76);
			Reply.push_back(0xFE);
			Reply.push_back(0x06);
			Reply.push_back(Stream[3] ? 0x01 : 0x00);
			Reply.push_back(0x70);
			Reply.push_back(0x42);

			break;
		}

		case 0x01:
		{
			Reply.push_back(0x76);
			Reply.push_back(0x01);
			Reply.push_back(0x06);
			Reply.push_back(0x00);
			Reply.push_back(0x70);
			Reply.push_back(0x42);

			isAddressed = 1;
			break;
		}

		case 0x08:
		{
			isAddressed = 0;
		}

		default:
		{
			break;
		}
	}

	BYTE* pDest = dAddr;

	unsigned i = 0;
	unsigned rMax = Reply.size();

	if (rMax > dSize)
	{
		rMax = dSize;
	}

	for (i = 0; i < rMax; i++)
	{
		*pDest++ = Reply[i];
	}

	unsigned rSize = Reply.size();

	return rSize;
}