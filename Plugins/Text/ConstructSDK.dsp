# Microsoft Developer Studio Project File - Name="ConstructSDK" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ConstructSDK - Win32 Debug Runtime
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ConstructSDK.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ConstructSDK.mak" CFG="ConstructSDK - Win32 Debug Runtime"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ConstructSDK - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ConstructSDK - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ConstructSDK - Win32 Runtime" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ConstructSDK - Win32 Debug Runtime" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ConstructSDK - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Construct Compile\Template\Release\"
# PROP Intermediate_Dir "..\..\..\Construct Compile\Template\Release\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /FR /Fp"Debug/ConstructSDK.pch" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:IX86 /out:"..\..\IDE\Plugins\Text.csx" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:yes /debug

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\Construct Compile\Template\Debug\"
# PROP Intermediate_Dir "..\..\..\Construct Compile\Template\Debug\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /Fr /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\IDE\Plugins\Text.csx" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no /nodefaultlib

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Runtime"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ConstructSDK___Win32_Runtime"
# PROP BASE Intermediate_Dir "ConstructSDK___Win32_Runtime"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Construct Compile\Template\Runtime\"
# PROP Intermediate_Dir "..\..\..\Construct Compile\Template\Runtime\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /FR /Fp"Debug/ConstructSDK.pch" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RUN_ONLY" /D "_WINDLL" /FR /Fp"Debug/ConstructSDK.pch" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:IX86 /out:"../IDE/Plugins/Object.csx" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /subsystem:windows /dll /machine:IX86 /out:"..\..\IDE\Plugins\Runtime\Text.csx" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /debug /nodefaultlib

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Debug Runtime"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ConstructSDK___Win32_Debug_Runtime"
# PROP BASE Intermediate_Dir "ConstructSDK___Win32_Debug_Runtime"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\Construct Compile\Template\Debug Runtime\"
# PROP Intermediate_Dir "..\..\..\Construct Compile\Template\Debug Runtime\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D "RUN_ONLY" /Fr /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\IDE\Plugins\Object.csx" /pdbtype:sept
# SUBTRACT BASE LINK32 /incremental:no /nodefaultlib
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\IDE\Plugins\Runtime\Text.csx" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no /nodefaultlib

!ENDIF 

# Begin Target

# Name "ConstructSDK - Win32 Release"
# Name "ConstructSDK - Win32 Debug"
# Name "ConstructSDK - Win32 Runtime"
# Name "ConstructSDK - Win32 Debug Runtime"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ConstructSDK.cpp
# End Source File
# Begin Source File

SOURCE=.\ConstructSDK.def

!IF  "$(CFG)" == "ConstructSDK - Win32 Release"

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Debug"

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Runtime"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Debug Runtime"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConstructSDK.rc
# End Source File
# Begin Source File

SOURCE=.\ConstructSDKRun.def

!IF  "$(CFG)" == "ConstructSDK - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Runtime"

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Debug Runtime"

# PROP BASE Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Edittime.cpp

!IF  "$(CFG)" == "ConstructSDK - Win32 Release"

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Debug"

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Runtime"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Debug Runtime"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FontSize.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Runtime.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ViewTextDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XColorStatic.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AggressiveOptimize.h
# End Source File
# Begin Source File

SOURCE=.\ConstructSDK.h
# End Source File
# Begin Source File

SOURCE=.\Main.h
# End Source File
# Begin Source File

SOURCE=.\properties.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ViewTextDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap1.bmp

!IF  "$(CFG)" == "ConstructSDK - Win32 Release"

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Debug"

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Runtime"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Debug Runtime"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\res\ConstructSDK.rc2
# End Source File
# Begin Source File

SOURCE=.\ICON.BMP

!IF  "$(CFG)" == "ConstructSDK - Win32 Release"

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Debug"

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Runtime"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Debug Runtime"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\res\ICON.BMP

!IF  "$(CFG)" == "ConstructSDK - Win32 Release"

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Debug"

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Runtime"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ConstructSDK - Win32 Debug Runtime"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\res\SICON.BMP
# End Source File
# Begin Source File

SOURCE=.\res\SmallIcon.bmp
# End Source File
# End Group
# End Target
# End Project
