# exile-olc6502
An experimental minimalist emulation of Exile on the BBC Micro using Javidx9's olc 6502 emulator

**Note: This project has been done for fun and educational purposes only.  To actually play BBC Exile, I recommend using a full BBC emulator such as BeebEm (http://www.mkw.me.uk/beebem/) or JSBeeb (https://bbc.godbolt.org/).**

One of my projects during lockdown in 2020 has been to learn 6502 assembly, and to understand the code from one of my favourite games of all time - Exile on the BBC.

To help with this, I have built a BBC micro emulator (strictly it's just an Exile emulator).  I've been amazed at how little additional code is needed beyond the 6502 emulator.  Currently it's around 95 lines - although this doesn't include sound or the blue background as the water level changes.

I've added this as a public repository, in case others find it useful/interesting.  To use it, you will need to add a ram dump of Exile (Model B version).  I used the B-em emulator (http://b-em.bbcmicro.com/) to generate the ram dump.  There is also an alternative version of main.cpp in the directory that parses the Exile disassembly directly, rather than using a ram dump.

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
  - The keys repeat too quickly - eg, when firing gun.  Great for shooting quickly, but uses loads of energy
  - I've made a small number of "hacky" changes to both the olc6502 and the PGE - I've marked these with TD

TGD
