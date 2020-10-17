# exile-olc6502
A minimalist emulation of Exile on the BBC Micro using Javidx9's olc 6502 emulator

One of my projects during lockdown in 2020 has been to learn 6502 assembly, and to understand the code from one of my favourite games of all time - Exile on the BBC.

To help with this, I have built a BBC micro emulator (strictly it's just an Exile emulator).  I've been amazed at how little additional code is needed beyond the 6502 emulator.  Currently it's around 150 lines, and I know my screen drawing routine is still still pretty verbose, so I'm planning to streamline this further.

I've added this as a public repository, in case others find it useful/interesting.  To use it, you will need to add a ram dump of Exile (Model B version).  I used the B-em emulator (http://b-em.bbcmicro.com/) to generate the ram dump.

Many thanks to everyone who has produced the excellent resources that I have used for this project, including:


Exile disassemblies:

http://www.level7.org.uk/miscellany/exile-disassembly.txt

https://github.com/tom-seddon/exile_disassembly


One Lone Coder Pixel Game Engine:

https://github.com/OneLoneCoder/olcPixelGameEngine
  

Javidx9's 6502 project (and accompanying YouTube videos):

https://github.com/OneLoneCoder/olcNES


TGD2
