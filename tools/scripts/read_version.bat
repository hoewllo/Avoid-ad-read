@echo off
REM 从VERSION文件读取版本号的Windows批处理脚本（简单格式）

set VERSION_FILE=VERSION

REM 检查文件是否存在
if not exist "%VERSION_FILE%" (
    echo Error: %VERSION_FILE% not found
    exit /b 1
)

REM 读取版本号（简单格式：只包含版本号）
for /f "delims=" %%v in ('type "%VERSION_FILE%"') do (
    echo %%v
    exit /b 0
)

echo Error: Could not read version
exit /b 1

:show_name
echo %PROJECT_NAME%
goto :eof

:show_all
echo VERSION=%VERSION%
echo PROJECT_NAME=%PROJECT_NAME%
goto :eof