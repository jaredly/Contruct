[Setup]
AppName=Construct
AppVerName=Construct 0.99.7
AppPublisher=Scirra
AppPublisherURL=http://www.scirra.com
AppSupportURL=http://www.scirra.com
AppUpdatesURL=http://www.scirra.com
DefaultDirName={pf}\Scirra\Construct
DefaultGroupName=Construct
AllowNoIcons=yes
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes
WindowShowCaption = no
WindowVisible = yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"; LicenseFile: "gpl-2.0.txt"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "D:\Construct 0.x SVN\IDE\Construct.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\SciLexer.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Scintilla.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\LiteZip.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Data\*"; DestDir: "{app}\Data\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\Construct 0.x SVN\IDE\Effects\*"; DestDir: "{app}\Effects\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\Construct 0.x SVN\IDE\Images\stub.txt"; DestDir: "{app}\Images\"; Flags: ignoreversion

Source: "D:\Construct 0.x SVN\IDE\Families\Blue.bmp"; DestDir: "{app}\Families\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Families\Enemy.bmp"; DestDir: "{app}\Families\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Families\Friendly.bmp"; DestDir: "{app}\Families\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Families\Green.bmp"; DestDir: "{app}\Families\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Families\Items.bmp"; DestDir: "{app}\Families\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Families\Powerups.bmp"; DestDir: "{app}\Families\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Families\Red.bmp"; DestDir: "{app}\Families\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Families\Terrain.bmp"; DestDir: "{app}\Families\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Families\Yellow.bmp"; DestDir: "{app}\Families\"; Flags: ignoreversion

Source: "D:\Construct 0.x SVN\IDE\Languages\*"; DestDir: "{app}\Languages\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\Construct 0.x SVN\IDE\Plugins\*"; DestDir: "{app}\Plugins\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\Construct 0.x SVN\IDE\PyAutoC\*"; DestDir: "{app}\PyAutoC\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\Construct 0.x SVN\IDE\Templates\*"; DestDir: "{app}\Templates\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Transitions\*"; DestDir: "{app}\Transitions\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\dxwebsetup.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\vcredist_x86.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\python26.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\capico.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Snippets\*"; DestDir: "{app}\Snippets\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\Construct 0.x SVN\IDE\dot.png"; DestDir: "{app}"; Flags: ignoreversion

Source: "D:\Construct 0.x SVN\IDE\Tools\Mesh Editor\Mesh Editor.exe"; DestDir: "{app}\Tools\Mesh Editor\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Tools\Mesh Editor\Mesh Editor.cap"; DestDir: "{app}\Tools\Mesh Editor\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Tools\Mesh Editor\readme.pdf"; DestDir: "{app}\Tools\Mesh Editor\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Tools\Mesh Editor\3Dslant.mesh"; DestDir: "{app}\Tools\Mesh Editor\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Tools\Mesh Editor\arch.mesh"; DestDir: "{app}\Tools\Mesh Editor\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Tools\Mesh Editor\bump.mesh"; DestDir: "{app}\Tools\Mesh Editor\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Tools\Mesh Editor\crazy.mesh"; DestDir: "{app}\Tools\Mesh Editor\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Tools\Mesh Editor\pinch.mesh"; DestDir: "{app}\Tools\Mesh Editor\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Tools\Mesh Editor\punch.mesh"; DestDir: "{app}\Tools\Mesh Editor\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Tools\Mesh Editor\spikey.mesh"; DestDir: "{app}\Tools\Mesh Editor\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Tools\Mesh Editor\tunnel.mesh"; DestDir: "{app}\Tools\Mesh Editor\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Tools\Mesh Editor\twist.mesh"; DestDir: "{app}\Tools\Mesh Editor\"; Flags: ignoreversion
Source: "D:\Construct 0.x SVN\IDE\Tools\Mesh Editor\twist2.mesh"; DestDir: "{app}\Tools\Mesh Editor\"; Flags: ignoreversion

[Registry]
Root: HKCU; Subkey: "Software\Construct"; Flags: deletekey uninsdeletekey

[Icons]
Name: "{group}\Construct"; Filename: "{app}\Construct.exe"
Name: "{group}\{cm:UninstallProgram,Construct}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Construct"; Filename: "{app}\Construct.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Construct"; Filename: "{app}\Construct.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\Construct.exe"; Description: "{cm:LaunchProgram,Construct}"; Flags: nowait postinstall skipifsilent
Filename: "{app}\vcredist_x86.exe"; StatusMsg: "Installing Visual Studio 2005 redistributable files..."; Flags: hidewizard
Filename: "{app}\dxwebsetup.exe"; StatusMsg: "Installing DirectX additional components..."; Flags: hidewizard



