# Cicada Framework
<img src="https://cdn.discordapp.com/attachments/754798036022067353/1131736330418794597/CicadaFramework.png" style="width:256px"/>
<p>
  <b>Logo Credits (Git Handle): <i><a href="https://github.com/rowandevving">@rowandevving</a></i></b>
  
  Throughout the foreseeable future, I will be working on this framework. Initially, it's compatibility will lie within the Win32 API, allowing cross-compatibility between different Windows Operating System
  distributions of which are of x86_64 architecture of course.

  This project will be maintained solely by myself and is a project that I will be working on just for "fun." Obviously, I will be making it as usable as possible for not just myself but for anyone else that wishes
  to make use of the framework.

  <b><i>
    This Framework, so to say, is intended to be classed as a wrapper around the native APIs that run on different Operating Systems. For now, it will be a wrapper around the Win32 API until I wish to work on
    a variant for another OS.
    </i>
    </br></br>
    In it's current state, this GUI Framework is not exactly, so to speak, "usable" as it is currently under a heavy development process. If you wish to for some reason use the framework for some bizarre reason then..
    You can do so? Good luck. However, I will be committing updates every time I make them at the end of each day I work on this project. For now, it is a read-only type of project, but may be a fully-usable framework
    in the future.
    </br>
  </b>

  You may contact me on Discord at: <b><i><a href="https://discord.com/users/293110261819375616">@sigmaeg</a></i></b>

  Thank you.
</p>

# How-To-Use

<b>Requires VS2022</b></br>

OR</br>

Change the <i><b>Platform Toolset (Compiler)</b></i> in <i><b>Configuration Properties -> Platform Toolset</b></i> to <i><b>Visual Studio 2019 (v142)</b></i>

## Building the Cicada Framework for Implementation

> Clone the Github Repo wherever you wish</br>
> Open <i><b>Cicada\Cicada Framework.sln</b></i></br>
> Build <i><b>Debug/x86</b></i>, <i><b>Debug/x64</b></i>, <i><b>Release/x86</b></i>, <i><b>Release/x64</b></i> <b>(Ctrl + B to Build)</b> </br>

## Including the Cicada Framework in your Project

> Create an <i><b>Empty C++ Project</b></i></br>
> Create a <b>*.c</b>/<b>*.cpp</b> file in <b>Source Files</b></br>
> Right-Click the <b>Project</b> in Visual Studio and Select <b>"Properties"</b> to enter <i><b>Configuration Properties</b></i></br>
> Ensure the <i><b>Platform Toolset (Compiler)</b></i> is the same as the one used for the <b>Cicada Framework Build</b></br>
> Go to <i><b>C/C++ -> General</b></i></b> in <i><b>Configuration Properties</b></i></br>
> Edit <i><b>"Additional Include Directories"</b></i> and include <b>Cicada Framework\include</b> <b>(FOR ALL CONFIGURATIONS AND PLATFORMS)</b></br>
> Go to <i><b>Linker -> Input</b></i> in <i><b>Configuration Properties</b></i></br>
> Edit <i><b>"Additional Dependencies"</b></i> and include <i><b>Cicada Framework\lib\libcic_frmwk_x__.lib</b></i> per <b>Configuration : Platform</b></br>
> Ensure each <i><b>Configuration : Platform</b></i> has its matching <i><b>libcic_frmwk_x__.lib</b></i> file. (Release .lib files don't have "debug" on the end)</br>

> Use the <i><b>example.c</b></i> file to run an example use case of the <b>Cicada Framework</b>

That's all.

Thank you for using the Cicada Framework!
