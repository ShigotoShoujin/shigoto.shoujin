@echo off
rd /s /q .vs Win32 x64
rd /s /q ShigotoShoujin\Win32 ShigotoShoujin\x64
rd /s /q ShigotoShoujinBoot\Win32 ShigotoShoujinBoot\x64
rd /s /q ShigotoShoujinUnitTest\Win32 ShigotoShoujinUnitTest\x64 

del ShigotoShoujin\*.user
del ShigotoShoujinBoot\*.user
del ShigotoShoujinUnitTest\*.user
