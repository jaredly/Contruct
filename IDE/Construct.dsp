# Microsoft Developer Studio Project File - Name="Construct" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Construct - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Construct.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Construct.mak" CFG="Construct - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Construct - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Construct - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Construct - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Construct Compile\IDE\Release"
# PROP Intermediate_Dir "..\..\Construct Compile\IDE\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "LIBs\UI Headers" /D "WIN32" /D "_WINDOWS" /D "NDEBUG" /D "_MBCS" /D "_AFXDLL" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 d3d9.lib d3dx9.lib shlwapi.lib CxImage.lib JPEG.lib PNG.lib ZLIB.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"library" /out:"Construct.exe" /libpath:"LIBs\\" /IGNORE:4089
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "Construct - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Construct Compile\IDE\Debug"
# PROP Intermediate_Dir "..\..\Construct Compile\IDE\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "LIBs\UI Headers" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "VC6" /Fr /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3d9.lib d3dx9.lib shlwapi.lib CxImage.lib JPEG.lib PNG.lib ZLIB.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"library" /out:"Construct.exe" /pdbtype:sept /libpath:"LIBs\\"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Construct - Win32 Release"
# Name "Construct - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Property Tree CPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\UI Elements\PropertyTree\PropTree.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropertyTree\PropTreeInfo.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropertyTree\PropTreeItem.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropertyTree\PropTreeItemButton.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropertyTree\PropTreeItemButton.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropertyTree\PropTreeItemColor.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropertyTree\PropTreeItemCombo.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropertyTree\PropTreeItemEdit.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropertyTree\PropTreeItemStatic.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropertyTree\PropTreeList.cpp"
# End Source File
# End Group
# Begin Group "Tab Control CPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\UI Elements\Controls\CustomTabCtrl.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Controls\FlatSplitterWnd.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\mfxWhlPan.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Controls\ruler.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\XP\ThemeUtil.cpp"
# End Source File
# End Group
# Begin Group "UI CPP"

# PROP Default_Filter ""
# Begin Group "Preferences CPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\UI Elements\Settings\AutoSaveOptions.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Settings\AutoSaveOptions.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Settings\GeneralOptions.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Settings\GeneralOptions.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Settings\LanguageOptions.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Settings\LanguageOptions.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Settings\PrefsStatic.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Settings\SettingsDlg.cpp"
# End Source File
# End Group
# Begin Group "Drag Bar CPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\UI Elements\Controls\scbarcf.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Controls\scbarg.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Controls\sizecbar.cpp"
# End Source File
# End Group
# Begin Group "Misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\UI Elements\Controls\DialogHeader.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\EditSpinSlider.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Controls\FlatToolTipCtrl.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Controls\ParamTooltipCtrl.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Controls\TitleTip.cpp"
# End Source File
# End Group
# Begin Group "XP Look CPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\UI Elements\Controls\BCMenu.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Controls\BtnST.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\XP\ButtonXP.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\XP\ColourPickerXP.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\XP\ComboBoxXP.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\XP\Draw.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\XP\EditBoxXP.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\XP\StatusBarXP.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\XP\Tools.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\XP\XPTheme.cpp"
# End Source File
# End Group
# Begin Group "HTML CPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\UI Elements\HTML\HTMLAtom.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\HTML\HTMLAtom.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\HTML\HTMLDrawer.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\HTML\HTMLDrawer.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\HTML\HTMLFont.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\HTML\HTMLFont.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\HTML\HTMLStatic.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\HTML\HTMLStatic.h"
# End Source File
# End Group
# Begin Source File

SOURCE=".\UI Elements\FontSize.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\FontSize.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Controls\MyHyperLink.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\ToolTipListCtrl.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\XColorStatic.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\XColorStatic.h"
# End Source File
# End Group
# Begin Group "Dialogs CPP"

# PROP Default_Filter ""
# Begin Group "Install Dialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Dialogs\InstallDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\InstallDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\propPage1.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\propPage1.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\PropPage2.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\PropPage2.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Dialogs\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\addcommentdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\arraypastedlg.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Dialogs\CompareValuesDlg.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ErrorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\FirstStartDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ImageBankDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ImageEditorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\inputdlg.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Dialogs\InsertObjectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ManipulateImageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\MenuEditorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\newmovementdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\NewTriggerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ObjectInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ofilter.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ParametersDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\petoolbox.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Dialogs\PicEdResizeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\SoundBankDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\TemplateDialog.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Dialogs\UsedExtensionsDlg.cpp
# ADD CPP /I ".."
# End Source File
# End Group
# Begin Group "Utilities CPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Utilities\cgfiltyp.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities\ddmgr.cpp
# End Source File
# Begin Source File

SOURCE=".\UI Elements\grouplistctrl.hpp"
# End Source File
# Begin Source File

SOURCE=.\Utilities\Ini.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities\miscfunc.cpp
# End Source File
# Begin Source File

SOURCE=.\Editors\Scintilla\ScintillaWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Editors\Scintilla\Tokenizer.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities\Undo.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities\w3c.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities\w3c.h
# End Source File
# Begin Source File

SOURCE=.\Utilities\WebGrab.cpp
# End Source File
# End Group
# Begin Group "Editors CPP"

# PROP Default_Filter ""
# Begin Group "Frame Editor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Editors\FrameEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\Editors\FrameEditorDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\Editors\FrameEditorMouse.cpp
# End Source File
# Begin Source File

SOURCE=.\Editors\FrameEditorObject.cpp
# End Source File
# End Group
# Begin Group "Expressions"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\Editors\ChronoEventEditor\ChronoEventEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\Editors\CPictureEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\Editors\EventEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\Editors\ExpressionEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\Editors\ImageEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\Editors\ScriptEditor.cpp
# End Source File
# End Group
# Begin Group "Structure CPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Structure\CApp.cpp
# End Source File
# Begin Source File

SOURCE=.\Structure\CFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\Structure\CObj.cpp
# End Source File
# Begin Source File

SOURCE=.\Structure\ObjectFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\Structure\SystemObject.cpp
# End Source File
# End Group
# Begin Group "Bars CPP"

# PROP Default_Filter "*.cpp"
# Begin Group "Layerbar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Bars\LayerBar.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Bars\LayerDlg.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Bars\LayerListBox.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Source File

SOURCE=.\Bars\ProjectBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Bars\PropertiesBar.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Editors\ChronoEventEditor\ActionTooltip.cpp
# End Source File
# Begin Source File

SOURCE=".\UI Elements\CColourBox.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\CColourBox.H"
# End Source File
# Begin Source File

SOURCE=..\Common\CDisplay.cpp
# End Source File
# Begin Source File

SOURCE=".\UI Elements\CeXDib.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\CeXDib.h"
# End Source File
# Begin Source File

SOURCE=.\ChildFrame.cpp
# End Source File
# Begin Source File

SOURCE=".\Construct Doc.cpp"
# End Source File
# Begin Source File

SOURCE=.\Construct.cpp
# End Source File
# Begin Source File

SOURCE=".\Construct.rc"
# End Source File
# Begin Source File

SOURCE=.\Export.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\FindReplaceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs\FindReplaceDlg.h
# End Source File
# Begin Source File

SOURCE=".\UI Elements\JazzUpTellTaleButton.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\jazzuptelltalebutton.h"
# End Source File
# Begin Source File

SOURCE=.\Languages.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Structure\SaveLoad.cpp
# End Source File
# Begin Source File

SOURCE=".\UI Elements\ShadeButtonST.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\shadebuttonst.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\StackedWndCtrl.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\StackedWndCtrl.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\TelltaleButton.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\TelltaleButton.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\ToolTipListCtrl.h"
# End Source File
# Begin Source File

SOURCE=.\WelcomeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WelcomeDlg.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Property Tree H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PropPage1.h
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropTree.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropTreeInfo.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropTreeItem.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropTreeItemColor.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropTreeItemCombo.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropTreeItemEdit.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropTreeItemStatic.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\PropTreeList.h"
# End Source File
# End Group
# Begin Group "Tab Control H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\UI Elements\CustomTabCtrl.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\FlatSplitterWnd.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\mfxWhlPan.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\mfxWhlPan.inl"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\ruler.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Schemadef.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\ThemeUtil.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Tmschema.h"
# End Source File
# End Group
# Begin Group "UI H"

# PROP Default_Filter ""
# Begin Group "XP Look H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\UI Elements\Controls\BCMenu.h"
# End Source File
# Begin Source File

SOURCE=.\Utilities\BtnST.h
# End Source File
# Begin Source File

SOURCE=".\UI Elements\ComboBoxXP.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\EditBoxXP.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Tools.h"
# End Source File
# End Group
# Begin Group "Preferences H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SettingsDlgSrc\PrefsStatic.h
# End Source File
# End Group
# Begin Group "Drag Bar H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\UI Elements\scbarcf.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\scbarg.h"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\sizecbar.h"
# End Source File
# End Group
# Begin Group "Misc H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\UI Elements\EditSpinSlider.h"
# End Source File
# Begin Source File

SOURCE=.\Utilities\func.h
# End Source File
# Begin Source File

SOURCE=".\UI Elements\Controls\TitleTip.h"
# End Source File
# End Group
# Begin Group "PictureEditorStuff"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\UI Elements\CaptionEditXP.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Elements\captioneditxp.h"
# End Source File
# End Group
# End Group
# Begin Group "Dialogs H"

# PROP Default_Filter ""
# Begin Group "Picture_Editor_Crap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Utilities\Palette\ColorChooser.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities\Palette\ColorChooser.h
# End Source File
# Begin Source File

SOURCE=.\Utilities\Palette\ColorMixer.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities\Palette\colormixer.h
# End Source File
# Begin Source File

SOURCE=.\Utilities\Palette\palettedlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities\Palette\palettedlg.h
# End Source File
# Begin Source File

SOURCE=.\Utilities\Palette\PaletteWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities\Palette\PaletteWnd.h
# End Source File
# Begin Source File

SOURCE=.\Utilities\Palette\ToolSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities\Palette\ToolSettings.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Dialogs\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\AddCommentDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\arraypastedlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\CompareValuesDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\errordlg.h
# End Source File
# Begin Source File

SOURCE=.\FirstStartDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ImageBankDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ImageEditorDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\InsertObjectDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ManipulateImageDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\MenuEditorDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\newmovementdlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\NewTriggerDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\objectinfodlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ofilter.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ParametersDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\petoolbox.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\PicEdResizeDlg.h
# End Source File
# Begin Source File

SOURCE=.\SettingsDlgSrc\SettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\TemplateDialog.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\UsedExtensionsDlg.h
# End Source File
# End Group
# Begin Group "Structure H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Structure\SystemObject.h
# End Source File
# End Group
# Begin Group "Bars H"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\Bars\LayerBar.h
# End Source File
# Begin Source File

SOURCE=.\Bars\ProjectBar.h
# End Source File
# Begin Source File

SOURCE=.\Bars\PropertiesBar.h
# End Source File
# End Group
# Begin Group "Editors H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Editors\ChronoEventEditor\ChronoEventEditor.h
# End Source File
# Begin Source File

SOURCE=.\Editors\CPictureEditor.h
# End Source File
# Begin Source File

SOURCE=.\Editors\EventEditor.h
# End Source File
# Begin Source File

SOURCE=.\Editors\ExpressionEditor.h
# End Source File
# Begin Source File

SOURCE=.\Editors\FrameEditor.h
# End Source File
# Begin Source File

SOURCE=.\Editors\ImageEditor.h
# End Source File
# Begin Source File

SOURCE=.\Editors\ScriptEditor.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Editors\ChronoEventEditor\ActionTooltip.h
# End Source File
# Begin Source File

SOURCE=.\Utilities\anchor.h
# End Source File
# Begin Source File

SOURCE=..\Common\CDisplay.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrame.h
# End Source File
# Begin Source File

SOURCE=".\Construct Doc.h"
# End Source File
# Begin Source File

SOURCE=.\Construct.h
# End Source File
# Begin Source File

SOURCE=.\Utilities\dlgman.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\inputdlg.h
# End Source File
# Begin Source File

SOURCE=.\Languages.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# PROP Ignore_Default_Tool 1
# End Source File
# Begin Source File

SOURCE=.\Editors\Scintilla\ScintillaWnd.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Structure.h
# End Source File
# Begin Source File

SOURCE=..\Common\VEdittime.h
# End Source File
# Begin Source File

SOURCE=..\Common\VRuntime.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\about.bmp
# End Source File
# Begin Source File

SOURCE=.\res\addcondition.bmp
# End Source File
# Begin Source File

SOURCE=.\res\addtoolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\autopan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp12814.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ColumnHeaderEnd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ColumnHeaderSpan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ColumnHeaderStart.bmp
# End Source File
# Begin Source File

SOURCE=.\res\comment.bmp
# End Source File
# Begin Source File

SOURCE=.\res\commentmenu.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Construct Doc.ico"
# End Source File
# Begin Source File

SOURCE=".\res\Construct.ico"
# End Source File
# Begin Source File

SOURCE=".\res\Construct.rc2"
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\custom.bmp
# End Source File
# Begin Source File

SOURCE=.\res\default1.bin
# End Source File
# Begin Source File

SOURCE=.\res\dot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\event1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\event2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fpoint.cur
# End Source File
# Begin Source File

SOURCE=.\res\fs.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hammer.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\number.bmp
# End Source File
# Begin Source File

SOURCE=.\res\objecttoolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pan_all.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_down.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_downfast.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_downleft.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_downleftfast.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_downright.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_downrightfast.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_left.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_leftfast.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_leftright.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_right.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_rightfast.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_up.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_updown.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_upfast.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_upleft.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_upleftfast.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_upright.cur
# End Source File
# Begin Source File

SOURCE=.\res\pan_uprightfast.cur
# End Source File
# Begin Source File

SOURCE=.\res\PictureEditorToolbarMain.bmp
# End Source File
# Begin Source File

SOURCE=.\res\project.bmp
# End Source File
# Begin Source File

SOURCE=.\res\runtime1.bin
# End Source File
# Begin Source File

SOURCE=.\res\small1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SpeedFusion.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\res\spliter.cur
# End Source File
# Begin Source File

SOURCE=.\res\string.bmp
# End Source File
# Begin Source File

SOURCE=.\res\system1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\systemtoolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\temptexture.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tools1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\trigger.bmp
# End Source File
# End Group
# Begin Group "CxImage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Utilities\CxImage\CxImage\ximage.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\Construct.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\res\Filter.png
# End Source File
# Begin Source File

SOURCE=.\Runtime.exe
# End Source File
# End Target
# End Project
