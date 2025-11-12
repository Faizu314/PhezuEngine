# PhezuEngine

Dependencies

	1) Mono
	2) SDL2

# Windows
Requirements

	1) Visual Studio Installation	
 
Instructions

	1) Download/Clone and double click Setup_Win.bat
	
# Mac (Not officially supported)
Requirements

	1) Mono Installation (brew install mono)
   
Instructions

	1) Download and open terminal at the root directory
	2) run the command ./Setup_Mac.sh
	⚠️ Note: Mac setup has been tested in a local development environment but has not been verified in a production or release environment. macOS support is experimental and may require manual setup or adjustments.

# Description
This is a 2D Game Engine with a sample Breakout game.
This project is for my personal learning but my aim is to make it a good learning resource for people who want to try engine development.

# Goal of the Project
    
    1) Make engine programming easy and accessible to understand
    2) Be able to create extremely simple games like pac man, breakout, tetris, snake, pocket tanks etc
    3) Keep the code base extremely simple
	4) Have as few dependencies as possible

# Engine Features

    1) C# scripting using mono
    2) Prefab system with nesting and overrides
    3) Physics with axis aligned boxes
    4) Rendering boxes with a solid color

# Roadmap
    
    1) Have a separate dll for engine and user scripts
    1) Remove SDL2 as a dependency
    2) Add support for primitive shapes and rotation
    3) Add sprites