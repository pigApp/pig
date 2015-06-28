@echo off

echo updating...
ping 192.0.22.22 -n 1 -w 3000 > nul
move /Y C:\PIG\.pig\tmp\pig.exe C:\PIG\deploy\pig.exe
start "" C:\PIG\deploy\pig.exe
exit
