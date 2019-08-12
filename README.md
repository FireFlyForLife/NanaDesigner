# NanaDesigner
A GUI Designer built with and for the NanaPro gui library

![picture of the ui editor](https://i.imgur.com/M0735lo.png)

## Features
- Quick prototyping of `div`s.
- Dynamic adding and removing of widgets
- Scheme editor, change all the scheme colours and other settings per widget.
- Save/Open projects as Json
- Export to C++ header/source file using a text template.
- Set resolution of the window.

![picture of a heavily modified form](https://i.imgur.com/ZXfzVKG.png)
![picture of that same form, with export window](https://i.imgur.com/k2DTJNV.png)

## How to get it
Download the latest release from https://github.com/FireFlyForLife/NanaDesigner/releases.
The program is portable so just run the .exe!

## Compiling from source
Firstly, make sure to clone all the git submodules for the dependencies.
To do this, run the following command: `git submodule update --init --recursive` .

### Windows
After you got the submodules cloned, you should be able to just launch NanaDesigner.sln and hit Build&Run.
It will automatically build Nana as dependency and link with it. The rest of the dependencies are all header only
Both x64 and x86 are supported.

### Linux
The program should be fully portable, however I haven't setup any build steps for linux yet (no Cmake/make or whatever).
The easiest way I can think of is by using the [Visual studio tools for linux](https://devblogs.microsoft.com/cppblog/linux-development-with-c-in-visual-studio/) and crosscompile that way. 
