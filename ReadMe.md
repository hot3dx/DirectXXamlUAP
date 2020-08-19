DirectX12UAP c++ template by Hot3dx - Jeff Kubitz
![Screenshot](https://github.com/hot3dx/DirectXXamlUAP/blob/master/Screenshot%20(46).png)

MUST START MSVS 2019 as Administrator or you can create the directory need and save the files with Visual Studio Code!

Designed to be placed in:
1) In Directory:
YOUR_DRIVE:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\ProjectTemplates\VC\Windows UAP\1033\
  A) Will Show in MS VS 2019 Create New Project Templates
  B) After placing all files in Directory Folder below Save All and close and restart VS 2019
or

Second Choice:
Your_DRIVE:\Users\Your_Computer\Documents\Visual Studio 2019\Templates\ProjectTemplates\Visual C++ Project\

1 A) Create Folder:
DirectX12XamlUAPApp

2) Place the files in this folder in DirectX12XamlUAP folder

3) Start Visual Studio 2019

4) Create Project DirectX12XamlUAP

5) Need Nuget Package WinPixEventRuntime.1.0.200127001

6) Suggested Add package id="Microsoft.UI.Xaml" version="2.4.2" targetFramework="native

It is the DirectX 12 Xaml spinning cube

The sample works with a warpadapter or a DirectX12 Capable Graphics Card on Windows 10!
The Sample is tested in Win32(x86), x64, Arm, Arm64

Click the mouse and the cube stops rotating.

Have fun and there is nothing like being able to add a GUI
through Xaml to a program, especially if it is DirectX 12

This template should work in your main Microsoft Templates for Visual Studio 2019
and show up when you want a template for a DirectX 12 UAP, Universal Windows Xaml
 App, where you can use the designer.

Build is Visual Studio 2019, OS Build 1909, tools 142. Upto VS 2019 16.6

The warp adapter will kick if your hardware does not support straight DirectX 12
This means you can use this for the basis of Intel HD Graphics laptops,
tablets, and other fine devices.

Donations Appreciated:
BTC 33DN8TgAe879tDkhsCEpneoVrydHbasuLm

BCHABC qruhxl8y2753fg0qfx7t8zfqzw92rvaxussk7fcnke

LTC MEQVCCxa5ZZkmGqAfYfRvipNcY4BaTRrFm

By: 
![Ico](https://github.com/hot3dx/AppXamlDX12/blob/master/Assets/AutoDraw2.ico)

Should you want to do anything complicated Chuck Walbourn's DirectX 12 Toolkit
That I also have will attach to this with a little work.

8\19\20 Sample3DSceneRender was changed to a ref class and performance is improved

Main.h and Main.cpp were modified to accept this change
