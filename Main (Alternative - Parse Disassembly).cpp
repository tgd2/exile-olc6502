#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "olc6502.h"
#include "Bus.h"

#include <string>
#include <vector>

float fTimeSinceLastFrame = 0;

class Exile_olc6502 : public olc::PixelGameEngine
{
public:
	Exile_olc6502() { sAppName = "olc6502 Exile"; }

	Bus BBC;

	bool OnUserCreate()
	{
		// Load Exile RAM from disassembly
		std::string sLine;
		std::ifstream fileExileDisassembly("exile-disassembly.txt"); // www.level7.org.uk/miscellany/exile-disassembly.txt
		if (fileExileDisassembly.is_open())
		{
			uint16_t nRam = 0x0100; // We will load in starting at 0x0100
			while (getline(fileExileDisassembly, sLine))
			{
				// Split line, assuming space delimited
				std::istringstream iss(sLine);
				std::vector<std::string> sLineParsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

				if (sLineParsed.size() > 0) {
					if (sLineParsed[0] == "#0100:" || nRam > 0x0100) {

						for (int nWord = 0; nWord < sLineParsed.size(); nWord++) {
							std::string sWord = sLineParsed[nWord];
							if (sWord == ";" || sWord == "#") nWord = sLineParsed.size(); // Skip line
							else if (nWord == 0) {
								if (sWord.length() != 6) nWord = sLineParsed.size(); // Skip line, as first word not length 6
								else if (sWord.front() != '#' && sWord.front() != '&'); // Skip line, as RAM location needs to start with # or &
								else {
									// Trim first word, and update RAM counter
									sWord.erase(0, 1); 
									sWord.erase(4, 1); // 4, as now length 5
									nRam = std::stoi(sWord, nullptr, 16);
								}
							}
							else {
								if (sWord.length() != 2) nWord = sLineParsed.size(); // Skip line, as reached end of hex
								else if (sWord == "--") nWord = sLineParsed.size(); // Skip line, as hex not defined
								else {
									// Read byte into RAM
									BBC.ram[nRam] = (uint8_t)std::stoi(sWord, nullptr, 16);
									nRam++;
								}
							}
						}
					}
				}
			}
			fileExileDisassembly.close();
		}
		else {
			std::cout << "Exile disassembly file missing";
		}

		// Patch some RAM bytes, to prevent (reduce?) crashes:
		BBC.ram[0x14B2] = 0xFF;  BBC.ram[0x14B4] = 0x60;  BBC.ram[0x215A] = 0x60;  BBC.ram[0x2628] = 0xA5;
		BBC.ram[0x2629] = 0xC0;  BBC.ram[0x29F3] = 0x1C;  BBC.ram[0x29F4] = 0x22;  BBC.ram[0x29F5] = 0x32;
		BBC.ram[0x29F6] = 0x38;  BBC.ram[0x3E99] = 0x3C;  BBC.ram[0x442C] = 0x20;  BBC.ram[0x442D] = 0xB9;
		BBC.ram[0x442E] = 0x4A;
		
		// Set stack pointer and PC:
		BBC.cpu.stkp = 0xff;  BBC.cpu.pc = 0x19b6;

		//Acquire all keys and equipment - optional! :)
		for (long i = 0; i < (0x0818 - 0x0806); i++) BBC.ram[0x0806 + i] = 0xFF;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		if ((fTimeSinceLastFrame += fElapsedTime) < 0.025) return true;
		else fTimeSinceLastFrame = 0.0;

		do {
			do BBC.cpu.clock();
			while (!BBC.cpu.complete());
		} while (BBC.cpu.pc != 0x19b6); //0x19b6 = Start of game loop

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

			int nScreenOffsetX = BBC.ram[0x0b95]; 		int nScreenOffsetY = BBC.ram[0x0b97];
			int nScreenOffsetX_Low = BBC.ram[0x0b91];	int nScreenOffsetY_Low = BBC.ram[0x0b93];

			uint16_t ram_counter = nScreenOffsetX_Low / 4 + (nScreenOffsetX + 104) * 64;

			//Loop 16x16 character squares, row by row
			for (int char_j = 0; char_j < 16; char_j++) {
				for (int char_i = 0; char_i < 16; char_i++) {

					//Loop 8x8 pixels in each character, column by column
					for (int pixel_i = 0; pixel_i < 4; pixel_i++) {
	 					for (int pixel_j = 0; pixel_j < 8; pixel_j++) {

							int ram_byte = BBC.ram[0x6000 + (ram_counter++ % 0x2000)]; // 0x2000 = size of VRam: 16*8 x 16*8 screen /2 as 2 pixels per byte
							int ram_bits[8];
							for (int bit_i = 0; bit_i < 8; bit_i++) {ram_bits[bit_i] = (ram_byte & (1 << bit_i)) != 0;}

							//Each byte contains two interleaved pixels
							for (int k = 0; k < 2; k++) {
								int screen_i = char_i * 8 + pixel_i * 2 + k;
								int screen_j = ((char_j + nScreenOffsetY * 12) * 8 + pixel_j + (104 - nScreenOffsetY_Low / 8)) % (16 * 8); // 16 * 8 = height of game screen
								Draw(8 + screen_i, 16 + screen_j, olc::Pixel(ram_bits[1 - k] * 0xFF, ram_bits[3 - k] * 0xFF, ram_bits[5 - k] * 0xFF));
							}
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