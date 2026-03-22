# Introduction

In this section I will talk about how you can download and compile PhezuEngine on your local machine and then play a sample game.

!!! note
    PhezuEngine is only supported on Windows for now.
    Linux and Android support is planned for the future.

## Clone PhezuEngine

In order to download the engine you will need to have git installed on your system. Here is the official link: [https://git-scm.com/install/windows](https://git-scm.com/install/windows)

To ensure git has installed properly run this command in the command prompt or powershell:

	git --version

You should see something like `git version 2.38.1.windows.1`, if instead you see something like `'git' is not recognized as an internal or external command` then git was not properly installed.

Now in order to download the repository on your local machine, open the terminal at your desired directory and run this command:

	git clone "https://github.com/Faizu314/PhezuEngine.git"


## Build PhezuEngine

Once you have successfully downloaded the repository, you will see that a child folder called PhezuEngine has been created. This is the root folder of our project. Inside the root folder there will be a batch script at `Scripts/win32/Setup_Win.bat` Double click this script to build and compile the entire project. Note that this script automatically fetches the following dependencies:

- **Visual Studio Build Tools** - If a compatible Visual Studio IDE is not present
- **Cmake** - If cmake is not already installed on your system this will be downloaded inside the root folder
- **Mono** - Will be downloaded in any case inside the root folder

Note that Visual Studio Build Tools installation will be the only system installation. Cmake and Mono will be downloaded internally (meaning deleting the PhezuEngine folder will completely remove them from your machine).

After the script has successfully run you will have a Build folder containing the Editor executable, engine library, Runtime executable and the core scripting dll. Don't worry you don't need to understand what these are in order to run the sample game but The Learner Docs sections contains a high level explanations for these modules.

## Clone Sample Game

Now that you have built project, you can now create games using it. In this tutorial we will be downloading and building a sample breakout game that was built using the PhezuEngine. Here is the link to the repository of the game [Phezu-Breakout](https://github.com/Faizu314/Phezu-Breakout). Create another folder for projects preferably outside the root folder of the engine and run this command to clone the game:

	 git clone "https://github.com/Faizu314/Phezu-Breakout.git"

This folder contains all of the assets and scripts of the Breakout game.

## Build Sample Game

!!! note
    The graphical editor application is not available yet.

    For now, building and running the sample game requires using terminal commands.
    This process will be replaced by a proper editor workflow in the future.

For now, the editor is an interactive CLI application on the terminal. It is only capable of running two commands `open` and `build` and we will be using them to build our games.

In order to build the game we will need two paths, one is the path to the editor application and the other one is the path to your game. The path to the editor application is inside the PhezuEngine repository that you cloned: `PhezuEngine/Build/Editor/EditorShell/Release` and the path to the game is where you cloned the sample game (namely the path to the Phezu-Breakout folder). Once you have these paths run the following commands:

	cd "<path to editor>"
	./EditorShell.exe
	open "<path to game>"
	build "<path to game>/Build"

You should now be able to see a "Runtime.exe" inside the `<path to game>/Build` directory, double click it to run the game.