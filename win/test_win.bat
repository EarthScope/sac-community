
@echo off

set TS=..\..\t\testsuite
set PYTEST=..\..\t\pytest.py
set SAC=..\Release\sac.exe


cd win

rmdir tests /s /q

mkdir tests
cd tests

xcopy %TS%\input\*.m /y /q
xcopy %TS%\input\*.am /y /q
xcopy %TS%\input\RESP.* /y /q
xcopy %TS%\input\*.sac /y /q
xcopy %TS%\input\*.SAC /y /q
xcopy %TS%\input\*.GSE /y /q
xcopy %TS%\input\2002.054.*  /y /q
xcopy %TS%\*.txt  /y /q

REM ALL TESTS
for /F "tokens=*" %%A in (..\tests.txt) do python %PYTEST% -s %SAC% -o %TS%\output -t %TS%\input\%%A

REM SINGLE TEST
REM python %PYTEST% -s %SAC% -o %TS%\output -t %TS%\input\help.sm

cd ..
cd ..


