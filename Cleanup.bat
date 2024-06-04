@echo off

IF EXIST bin (
	rmdir /s /q bin
	echo Deleted bin..
)

IF EXIST "Gaia.sln" (
	del /q "Gaia.sln"
	echo Deleted Solution File..
)

FOR %%f IN ("src\Gaia.vcxproj", "src\Gaia.vcxproj.filters") DO (
	IF EXIST %%f (
		del /q %%f
		echo Deleted %%f
	)
)

echo Cleanup Finished!

PAUSE