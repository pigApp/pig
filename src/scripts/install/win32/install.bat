@echo off
set currentPath=%~dp0
mkdir C:\PIG
move %currentPath%\deploy C:\PIG\deploy
move %currentPath%\.pig C:\pig\.pig
%currentPath%\nircmd-x64\nircmd.exe shortcut "C:\PIG\deploy\pig.exe" "~$folder.desktop$" "PIG"