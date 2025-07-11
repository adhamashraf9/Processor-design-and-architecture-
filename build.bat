@echo off
echo Building processor simulator...
gcc -o simulator.exe main.c instruction.c memory.c pipeline.c execute.c utils.c -Wall -Wextra -O2 -Wno-unused-parameter
if errorlevel 1 (
    echo Build failed!
    pause
    exit /b 1
)
echo Build successful!
echo Running simulator...
simulator.exe
pause 