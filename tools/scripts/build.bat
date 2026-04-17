@echo off
echo EPUB Ad Cleaner 构建工具
echo ============================
echo.

REM 检查PowerShell版本
powershell -Command "if ($PSVersionTable.PSVersion.Major -lt 5) { Write-Host '需要PowerShell 5.0或更高版本' -ForegroundColor Red; exit 1 }"

REM 运行PowerShell构建脚本
echo 正在启动构建脚本...
echo.

powershell -ExecutionPolicy Bypass -File "build\build.ps1" %*

if %errorlevel% neq 0 (
    echo.
    echo 构建失败！
    pause
    exit /b 1
)

echo.
echo 构建完成！
pause