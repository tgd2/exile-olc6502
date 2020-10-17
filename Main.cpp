#include <iostream>
#include <sstream>

#include "Bus.h"
#include "olc6502.h"
#include "ram_dump.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

float fTimeSinceLastFrame = 0;
bool bAtStartOfGameLoop = false;

class Exile_olc6502 : public olc::PixelGameEngine
{
public:
	Exile_olc6502() { sAppName = "olc6502 Exile"; }

	Bus BBC;

	bool OnUserCreate()
	{
		// Load Exile RAM dump
		for (long i = 0; i < 65536; i++) {
			BBC.ram[i] = (uint8_t)Exile[i];
		}

		//Acquire all keys and equipment :)
		for (long i = 0; i < (0x0818 - 0x0806); i++) BBC.ram[0x0806 + i] = 0xFF;

		BBC.cpu.a = 0x01;
		BBC.cpu.x = 0xff;
		BBC.cpu.y = 0x51;

		BBC.cpu.stkp = 0xfb;
		BBC.cpu.pc = 0x1f63;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		if ((fTimeSinceLastFrame += fElapsedTime) < 0.025) return true;
		else fTimeSinceLastFrame = 0;

		do {
			do BBC.cpu.clock();
			while (!BBC.cpu.complete());
			bAtStartOfGameLoop = (BBC.cpu.pc == 0x19b6);
		} while (!bAtStartOfGameLoop);

		bAtStartOfGameLoop = false;

		olc::Key Keys[39] = { olc::ESCAPE, olc::F10, olc::F1, olc::F2, olc::F3, olc::F4, olc::F5, olc::F6, olc::F7, olc::F8, olc::F9, olc::F9,
							  olc::Key::G, olc::SPACE, olc::Key::I, olc::LEFT, olc::RIGHT, olc::UP, olc::DOWN, olc::Key::K, olc::Key::O, olc::Key::TD_LeftSquareBracket,
							  olc::CTRL, olc::TAB, olc::Key::Y, olc::Key::U, olc::Key::T, olc::Key::R, olc::TD_FUllStop, olc::Key::M, olc::Key::TD_Comma,
							  olc::Key::S, olc::Key::V, olc::Key::Q, olc::Key::W, olc::Key::P, olc::Key::P, olc::Key::L, olc::SHIFT };

		for (int nKey = 0; nKey < 39; nKey++) {
			if (GetKey(Keys[nKey]).bPressed || GetKey(Keys[nKey]).bHeld)
			BBC.ram[0x126b + nKey] = BBC.ram[0x126b + nKey] | 0b10000000; else
			BBC.ram[0x126b + nKey] = 0b00000000;
		}

			Clear(olc::VERY_DARK_GREY);

			int nScreenOffsetX_Low = BBC.ram[0x0b91]; 
			int nScreenOffsetX = BBC.ram[0x0b95];
			int nScreenOffsetY_Low = BBC.ram[0x0b93]; 
			int nScreenOffsetY = BBC.ram[0x0b97];

			int ram_pixels = 0;
			int ram_bits[8];
			int pixel_i = 0;
			int pixel_j = 0;
			olc::Pixel pixel_col;


			uint16_t ram_counter = 0x6000 + nScreenOffsetX_Low / 4 + (nScreenOffsetX - 152) * 64;

			while (ram_counter < 0x6000) ram_counter += (16 * 16 * 8 * 4);
			while (ram_counter >= (0x6000 + 16 * 16 * 8 * 4)) ram_counter -= (16 * 16 * 8 * 4);

			for (int char_j = 0; char_j < 16; char_j++) {
				for (int char_i = 0; char_i < 16; char_i++) {

					for (int iPixelOffset_i = 0; iPixelOffset_i < 4; iPixelOffset_i++) {
	 					for (int iPixelOffset_j = 0; iPixelOffset_j < 8; iPixelOffset_j++) {

							ram_pixels = BBC.ram[ram_counter];

							if ((ram_pixels & 0b10000000) == 0b10000000) ram_bits[0] = 1; else ram_bits[0] = 0;
							if ((ram_pixels & 0b01000000) == 0b01000000) ram_bits[1] = 1; else ram_bits[1] = 0;
							if ((ram_pixels & 0b00100000) == 0b00100000) ram_bits[2] = 1; else ram_bits[2] = 0;
							if ((ram_pixels & 0b00010000) == 0b00010000) ram_bits[3] = 1; else ram_bits[3] = 0;
							if ((ram_pixels & 0b00001000) == 0b00001000) ram_bits[4] = 1; else ram_bits[4] = 0;
							if ((ram_pixels & 0b00000100) == 0b00000100) ram_bits[5] = 1; else ram_bits[5] = 0;
							if ((ram_pixels & 0b00000010) == 0b00000010) ram_bits[6] = 1; else ram_bits[6] = 0;
							if ((ram_pixels & 0b00000001) == 0b00000001) ram_bits[7] = 1; else ram_bits[7] = 0;

							for (int k = 0; k < 2; k++) {
								//	0	0	0	0	Black
								//	0	0	1	1	Red
								//	0	1	0	2	Green
								//	0	1	1	3	Yellow
								//	1	0	0	4	Blue
								//	1	0	1	5	Magenta
								//	1	1	0	6	Cyan
								//	1	1	1	7	White

								pixel_col = olc::BLACK;
								if ((ram_bits[2 + k] == 0) && (ram_bits[4 + k] == 0) && (ram_bits[6 + k] == 0)) pixel_col = olc::BLACK;
								if ((ram_bits[2 + k] == 0) && (ram_bits[4 + k] == 0) && (ram_bits[6 + k] == 1)) pixel_col = olc::RED;
								if ((ram_bits[2 + k] == 0) && (ram_bits[4 + k] == 1) && (ram_bits[6 + k] == 0)) pixel_col = olc::GREEN;
								if ((ram_bits[2 + k] == 0) && (ram_bits[4 + k] == 1) && (ram_bits[6 + k] == 1)) pixel_col = olc::YELLOW;
								if ((ram_bits[2 + k] == 1) && (ram_bits[4 + k] == 0) && (ram_bits[6 + k] == 0)) pixel_col = olc::BLUE;
								if ((ram_bits[2 + k] == 1) && (ram_bits[4 + k] == 0) && (ram_bits[6 + k] == 1)) pixel_col = olc::MAGENTA;
								if ((ram_bits[2 + k] == 1) && (ram_bits[4 + k] == 1) && (ram_bits[6 + k] == 0)) pixel_col = olc::CYAN;
								if ((ram_bits[2 + k] == 1) && (ram_bits[4 + k] == 1) && (ram_bits[6 + k] == 1)) pixel_col = olc::WHITE;

								pixel_i = char_i * 8 + iPixelOffset_i * 2 + k;
								pixel_j = char_j * 8 + iPixelOffset_j;

								while (pixel_i < 0) pixel_i += (16 * 8);
								while (pixel_i >= 16 * 8) pixel_i -= (16 * 8);
								
								pixel_j += -nScreenOffsetY_Low / 8 - nScreenOffsetY * 32 - 24;
								while (pixel_j < 0) pixel_j += (16 * 8);
								while (pixel_j >= 16 * 8) pixel_j -= (16 * 8);

								Draw(8 + pixel_i, 16 + pixel_j, pixel_col);
							}

							ram_counter++;
							if (ram_counter == 0x6000 + 16 * 16 * 8 * 4) ram_counter = 0x6000;

						}
					}
				}
			}

		return true;
	}
};


int main()
{
	Exile_olc6502 exile;
	//16*8 x 16*8 screen, with 2:1 pixels, plus a border
	exile.Construct(16 * 8 + 16, 16 * 8 + 32, 6, 3, false, true); 

	exile.Start();
	return 0;
}