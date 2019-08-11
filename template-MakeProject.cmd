@echo off
set ans=%1%
set action=0
set once=0
:setting
    if "%ans%"=="once" (
        set once=1
        set ans=%2%
    ) else (
        if "%2%"=="once" (
            set once=1
        )
    )
    if "%ans%"=="normal" goto:set-normal
    if "%ans%"=="all" goto:set-all
    if "%ans%"=="auto" goto:set-auto
    if "%ans%"=="user" goto:set-user
    goto:user-set-action

    :set-normal
    set action=0
    goto:exit-setting

    :set-all
    set action=1
    goto:exit-setting

    :set-auto
    set action=2
    goto:exit-setting

    :set-user
    set action=3
    goto:exit-setting

    :user-set-action
    echo [91mAction menu:
    echo normal - default make command
    echo all    - clean project and make
    echo auto   - delete autonomous cache and make
    echo user   - delete opcontrol cache and make
    echo Or press enter again to set 'normal'
    set /p ans="Enter action: [0m" || set ans=normal
    goto:setting
    
    :exit-setting

echo [91m------------------Make Project------------------[0m
:FOREVER
    :make
    echo [91mStart make[0m
    if %action%==1 (
        prosv5 make all
    ) else (
        if %action%==2 (
            del .\bin\autonomous.cpp.o
        ) else if %action%==3 (
            del .\bin\opcontrol.cpp.o
        )
        prosv5 make
    )
    if errorlevel 1 (
        echo [91mFailed to build[0m
        goto:finish
    ) 
    :waitting
    prosv5 lsusb > temp.log
    find "COM" temp.log >nul
    if errorlevel 1 (
        echo [91mV5 product not found, retrying...[0m
        goto:waitting
    )
    del .\temp.log
    :upload
    echo [91mStart upload[0m
    prosv5 upload --execute
    if errorlevel 1 (
        echo [91mFailed to upload
        echo Press any key to upload again[0m
        pause >nul
        goto:upload
    )
    echo [91mUpload done![0m
    :finish
    if %once%==1 goto:end
    echo [91mPress any key to start again
    echo Current action: %ans%[0m
    pause >nul
    goto:FOREVER
:end
