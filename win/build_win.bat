
set MyConfig=Release
set MSBuild="C:\Program Files\MSBuild\12.0\Bin\MSBuild.exe"
REM set MSBuild="C:\Program Files\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild.exe"
set InnoSetup="C:\Program Files\Inno Setup 5\ISCC"
set Winaux=win\%MyConfig%\winaux


REM Create config.h
copy inc\config_win.h inc\config.h

REM Build SAC

%MSBuild% win\sac.sln /p:configuration=%MyConfig% /m /flp1:logfile=errors.txt;errorsonly /flp2:logfile=warnings.txt;warningsonly

if %ERRORLEVEL% GEQ 1 EXIT /B 1
echo %ERRORLEVEL%

del inc\config.h

REM Copy AUX to WINAUX

rmdir %Winaux% /s /q
mkdir %Winaux%
copy aux\* %Winaux%
FOR %%A in ( ctables datagen external fir fonts help macros mat tables ) DO (
    mkdir %Winaux%\%%A
    xcopy aux\%%A %Winaux%\%%A /e
)

REM Create Installer

%InnoSetup% /cc /dMyConfig=%MyConfig% win\sac_win_installer.iss

