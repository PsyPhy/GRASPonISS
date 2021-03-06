@echo on
REM
REM		Grasp.bat
REM

REM
REM A batch file that executes Grasp.exe and then transfers the data files to DEX.
REM

REM
REM Execute Grasp.exe with all the arguments.
REM
Executables\Grasp.exe %*
set GRASPERRORLEVEL=%ERRORLEVEL%

REM Now write the files to a directory on DEX, using the root filename to select which files to write.
REM Do not attempt to send if the NoFTP.flg cookie is present.
if exist NoFTP.flg goto NOFTP

REM Find the filename root amongst the parameters.
REM The GraspGUI shell puts the results file root in the form --output=FNROOT.
REM Note that the '=' is treated as a blank by the command processor.
REM We therefore look for '--ouput' and then take the next parameter as the rootname.
:FIND
if "%1"=="" goto NOTFOUND
echo Leading argument is %1
if "%1"=="--output" goto FOUND
shift
goto FIND

:NOTFOUND 
echo Could not find filename root.
exit -10

:FOUND
set root=%2
echo File root is %root%

REM
REM Strip off the filename root, leading only the path to the local results trajectory.
REM
set local_directory=%2
:shrink
if "%local_directory:~-1%" == "\" goto :next
set local_directory=%local_directory:~0,-1%
goto :shrink
:next
echo Local directory is "%local_directory%"

REM
REM Compute the archive name from the specified local directory.
REM Per CADMOS request, it should be YYDOY.tar.
REM
set GRASPROOT=DATA1/DATA/glog
set yy=%local_directory:~-7,2%
set mm=%local_directory:~-5,2%
set dd=%local_directory:~-3,2%
call :DateToOrdinal %yy% %mm% %dd% year day
echo %yy% %mm% %dd% Year: %year% Day: %day%
set DIRECTORY=%year%%day%

REM
REM Copy the data files to DEX.
REM
set LOGONID=speedy
set PASSWORD=dex
set HOST=10.80.12.103

REM
REM Make the directory on the host. This might fail if the directory is already there, so ignore any errors.
REM
Utils\WinSCP.com /command "open ftp://%LOGONID%:%PASSWORD%@%HOST%" "cd %GRASPROOT%" "mkdir %DIRECTORY%"  "exit" 
if ERRORLEVEL == 1 GOTO :IGNORE
:IGNORE
Utils\WinSCP.com /command "open ftp://%LOGONID%:%PASSWORD%@%HOST%" "cd %GRASPROOT%" "cd %DIRECTORY%" "mput %2.*"  "exit" 
if ERRORLEVEL == 1 GOTO :FTPERR

:NOFTP
echo GRASPERRORLEVEL %GRASPERRORLEVEL%
exit %GRASPERRORLEVEL%

:FTPERR
echo GRASPERRORLEVEL %GRASPERRORLEVEL%
if "%GRASPERRORLEVEL%"=="0" exit -1
exit %GRASPERRORLEVEL%


:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:DateToOrdinal %yy% %mm% %dd% year day
::
:: By: Ritchie Lawrence, updated 2002-11-22. Version 1.1
::
:: Func: Returns an ISO 8601 Ordinal date from a calendar date.
:: For NT4/2K/XP.
:: 
:: Args: %1 year component to be converted, 2 or 4 digits (by val)
::       %2 month component to be converted, leading zero ok (by val)
::       %3 day of month to be converted, leading zero ok (by val)
::       %4 var to receive year, 2 digits (by ref)
::       %5 var to receive day of year, 001 to 366 (by ref)
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
setlocal ENABLEEXTENSIONS
set yy=%1&set mm=%2&set dd=%3
if 1%yy% LSS 200 if 1%yy% LSS 170 (set yy=20%yy%) else (set yy=19%yy%)
set /a dd=100%dd%%%100,mm=100%mm%%%100
set /a z=14-mm,z/=12,y=yy+4800-z,m=mm+12*z-3,j=153*m+2
set /a j=j/5+dd+y*365+y/4-y/100+y/400-32045
set /a y=yy+4799,k=y*365+y/4-y/100+y/400-31738,o=j-k+1
set /a yy=yy-2000
if %o% LSS 100 set o=0%o%&if %o% LSS 10 set o=00%o%
endlocal&set %4=%yy%&set %5=%o%&goto :EOF
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:EOF