@echo off

IF EXIST bin (
	rmdir /s /q bin
	echo Deleted bin..
)

IF EXIST "Gaia.sln" (
	del /q "Gaia.sln"
	echo Deleted Solution File..
)

IF EXIST .vs (
	rmdir /s /q .vs
	echo Deleted VS Temp Files..
)

FOR %%f IN ("src\Gaia.vcxproj", "src\Gaia.vcxproj.filters", "src\Gaia.vcxproj.user") DO (
	IF EXIST %%f (
		del /q %%f
		echo Deleted %%f
	)
)

echo Cleanup Finished!

PAUSE