
set MyConfig=Release
set Winaux=win\%MyConfig%\winaux

REM Create config.h
copy inc\config_win.h inc\config.h

REM Build SAC

"C:\Program Files\MSBuild\12.0\Bin\MSBuild.exe" win\sac.sln /p:configuration=%MyConfig% /m /flp1:logfile=errors.txt;errorsonly /flp2:logfile=warnings.txt;warningsonly

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

"C:\Program Files\Inno Setup 5\ISCC" /cc /dMyConfig=%MyConfig% win\sac_win_installer.iss






