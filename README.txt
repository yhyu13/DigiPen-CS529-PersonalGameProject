/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: README.txt
Purpose: Project Description
Language: C++ 11
Platform: Visual Studio 2017 Win32 x86/x64
Project: CS529 2019 Fall
Author: Hang Yu, hang.yu, 60001119
Creation date: Dec.10.2019
- End Header --------------------------------------------------------*/

├───CS529_Game
│   ├───Resource
│   │   ├───Audio
│   │   ├───Configuration
│   │   │   ├───Audio
│   │   │   ├───Object
│   │   │   ├───Pack
│   │   │   ├───UI
│   │   │   └───Weapon
│   │   ├───Level
│   │   ├───Shader
│   │   └───Sprite
│   │       ├───Source
│   │       └───UI
│   ├───src
│   │   ├───Component
│   │   │   ├───Gameplay
│   │   │   ├───Graphics
│   │   │   └───Logic
│   │   ├───Entity
│   │   │   └───Gameplay
│   │   │       ├───Interface
│   │   │       ├───Objects
│   │   │       ├───Pack
│   │   │       ├───UI
│   │   │       └───Weapon
│   │   ├───Event
│   │   └───Platform
│   │       └───Win32_SDL
├───GameEngine
│   ├───Common
│   │   ├───Component
│   │   │   ├───Graphics
│   │   │   ├───Interface
│   │   │   ├───Logic
│   │   │   └───Physics
│   │   ├───Entity
│   │   ├───Event
│   │   ├───Geommath
│   │   │   ├───2D
│   │   │   └───Shape
│   │   ├───Interface
│   │   └───Utility
├───SDL2.0 Image
│   ├───include
│   └───lib
│       ├───x64
│       │   └───dll
│       └───x86
│           └───dll
├───SDL2.0 Lib
│   ├───include
│   └───lib
│       ├───x64
│       │   └───dll
│       └───x86
│           └───dll
├───SDL2.0 Mixer
│   ├───include
│   └───lib
│       ├───x64
│       │   └───dll
│       └───x86
│           └───dll
└───Test_GameEngine


1. In this project, I have made a game of 2D Vertical Scroll Shooter. 

You play as a Space Ranger to defeat waves of enemy forces including one BOSS fight which has multiple attack mode. 
You are equiped with our best gears, totaling one lazer gun, one kinetic coil gun and automatic missiles. 

Executables are avaiable in "./CS529_Game/Win32" and "./CS529_Game/x64".

Arrow keys to maneuver.
"A" "D" to switch weapon.
"Z" "X" to switch weapon.
"Shift" to slow down your ship.

Pilot guide is avaiable in the "How To Play" menu in the game. 

"./Resouce" under "./CS529_Game" contains all the serialization files for game objects and levels. 
├───CS529_Game
│   ├───Resource
│   │   ├───Audio
│   │   ├───Configuration
│   │   │   ├───Audio
│   │   │   ├───Object
│   │   │   ├───Pack
│   │   │   ├───UI
│   │   │   └───Weapon
│   │   ├───Level
│   │   ├───Shader
│   │   └───Sprite
│   │       ├───Source
│   │       └───UI
The BOSS setting is avaiable in "./CS529_Game/Resource/Level/Level1_Wave7.txt". You can modify its healthpoint component change the total HP of the BOSS.

2. User interface
(1)
The main menu contains:

"New Game"
"How To Play"
"Quit"

and a music volume icon on the top right corner.
(2)
In game menu:

Press "ESC" to toggle pause in the game.

3. Known Bugs:

(1) Switching weapons rapidly (pressing "A","S" repeatedly) may cause crash.
(2) I wrote test for my custom memory allocator in the "Test_GameEngine" project. But tests were not discovered by VS when I finished the project. (Does not affect gameplay)

4. Credit:
(1)
Sound:

Blast.wav http://soundbible.com/538-Blast.html

Komiku_-_64_-_First_Dance https://files.freemusicarchive.org/storage-freemusicarchive-org/music/Music_for_Video/Komiku/Poupis_incredible_adventures_/Komiku_-_64_-_First_Dance.mp3
Komiku_-_58_-_Universe_big_takedown https://files.freemusicarchive.org/storage-freemusicarchive-org/music/Music_for_Video/Komiku/Poupis_incredible_adventures_/Komiku_-_58_-_Universe_big_takedown.mp3
Komiku_-_07_-_Battle_of_Pogs https://files.freemusicarchive.org/storage-freemusicarchive-org/music/Music_for_Video/Komiku/Poupis_incredible_adventures_/Komiku_-_07_-_Battle_of_Pogs.mp3
Monplaisir_-_05_-_Level_2 https://files.freemusicarchive.org/storage-freemusicarchive-org/music/WFMU/Monplaisir/Turbo_Rallye_Clicker_4000_Soundtrack/Monplaisir_-_05_-_Level_2.mp3

Defeat https://files.freemusicarchive.org/storage-freemusicarchive-org/music/WFMU/Monplaisir/Turbo_Rallye_Clicker_4000_Soundtrack/Monplaisir_-_09_-_Defeat.mp3
Victory https://files.freemusicarchive.org/storage-freemusicarchive-org/music/WFMU/Monplaisir/Turbo_Rallye_Clicker_4000_Soundtrack/Monplaisir_-_08_-_Victory.mp3

雷霆战机BGM http://sc.chinaz.com/tag_yinxiao/LeiZuoZhanJi.html

(2) Sprite:
https://fontmeme.com/pixel-fonts/
https://onlinepngtools.com/create-transparent-png
