# Etch-A-Sketch

Etch-A-Sketch made with raylib and C++.

## Controls / Customization

Press H for help menu.

Controls can be changed in `constants.h`.

# Building

## VSCode (all platforms)

*Note* You must have a compiler toolchain installed in addition to vscode.

1. Run the build task ( CTRL+SHIFT+B or F5 )
2. You are good to go

## Linux / Windows (MingGW)
* Rename the folder to your game name
* CD into the build folder
* Run `./premake5 gmake`
* CD back to the root
* Run `make`
* You are good to go

## MacOS
* Rename the folder to your game name
* CD into the build folder
* Run `./premake5.osx gmake`
* CD back to the root
* Run `make`
* You are good to go
<!--
add flag `--cc=clang` if using using clang?
`.\premake5.osx --cc=clang gmake`
-->

# Output files
The built code will be in the bin dir
