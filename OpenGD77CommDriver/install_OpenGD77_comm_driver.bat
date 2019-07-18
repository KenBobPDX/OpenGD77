@echo off

echo Installing GD-77 Serial driver...
"%~dp0wdi-simple" --vid 0x01FC9 --pid 0x0094 --type 3 --name "GD-77 Serial" --dest "%~dp0GD77-serial"
echo.

pause

