
@echo off
setlocal enabledelayedexpansion
set arg1=%1
set argCount=0
for %%x in (%*) do (
   set /A argCount+=1
   set "argVec[!argCount!]=%%~x"
)

echo Number of processed arguments: %argCount%

for /L %%i in (2,1,%argCount%) do (
    echo %%i- "!argVec[%%i]!"
    C:\VulkanSDK\1.1.130.0\Bin32\glslc.exe ./glsl/%arg1%."!argVec[%%i]!" -o ./spv/%arg1%."!argVec[%%i]!".spv
)
