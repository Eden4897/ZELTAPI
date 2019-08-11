@echo off

:start
set /P ans=[91mUse Make All?[0m (y/n)
if %ans% == y (
    goto:make-bin
) else (
    goto:make-template
)

:make-bin
prosv5 make all
if errorlevel 1 (
    echo [91mFailed to build[0m
    echo [91mPress any key to build again[0m
    pause >nul
    goto:make-bin
)

:make-template
prosv5 make template
if errorlevel 1 (
    echo [91mFailed to make template[0m
    echo [91mPress any key to make again[0m
    pause >nul
    goto:make-template
)

echo [91mWant to fetch the template? [0m
:fetch-template
set /P version=[91mWrite the version name: [0m
prosv5 c f "../../vexLibrary/ZELTAPI@%version%.zip"

echo [91m--------------------Done--------------------[0m
echo [91mWant to apply the template to other project?[0m
:apply
set /P project=[91mWrite the project location: [0m
cd "../%project%"
if errorlevel 1 (
    echo [91mFailed to find the project[0m
    goto:apply
)
prosv5 c a "ZELTAPI@%version%" --force-apply
makeproject.cmd once