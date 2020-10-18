# exile-olc6502
A minimalist emulation of Exile on the BBC Micro using Javidx9's olc 6502 emulator

One of my projects during lockdown in 2020 has been to learn 6502 assembly, and to understand the code from one of my favourite games of all time - Exile on the BBC.

To help with this, I have built a BBC micro emulator (strictly it's just an Exile emulator).  I've been amazed at how little additional code is needed beyond the 6502 emulator.  Currently it's around 150 lines, and I know my screen drawing routine is still still pretty verbose, so I'm planning to streamline this further.

I've added this as a public repository, in case others find it useful/interesting.  To use it, you will need to add a ram dump of Exile (Model B version).  I used the B-em emulator (http://b-em.bbcmicro.com/) to generate the ram dump.

Huge respect and thanks to Peter Irvin and Jeremy Smith for creating Exile.  I cannot guess how many hours I spent immersed in this game, and it still amazes me how alive the world feels.

Many thanks to everyone who has produced the excellent resources that I have used for this project, including:


Exile disassemblies:

http://www.level7.org.uk/miscellany/exile-disassembly.txt

https://github.com/tom-seddon/exile_disassembly


One Lone Coder Pixel Game Engine:

https://github.com/OneLoneCoder/olcPixelGameEngine
  

Javidx9's 6502 project (and accompanying YouTube videos):

https://github.com/OneLoneCoder/olcNES

Information about BBC's graphic modes:

https://www.dfstudios.co.uk/articles/retro-computing/bbc-micro-screen-formats/

A few additional notes, and limitations:
  - There are no sounds.  This wasn't an area of focus for me, but I am missing them!
  - I'm not sure that the earthquake would be triggered - I believe this requires the interrupt to be implemented, which I haven't done
  - I've made a small number of "hacky" changes to both the olc6502 and the PGE - I've marked these with TD

TGD
