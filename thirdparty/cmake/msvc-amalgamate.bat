set ROOTDIR=%CD%
set LIBDIR=%1
set OUTDIR=%2

echo "Amalgamating target dir: %LIBDIR%"

md "%LIBDIR%"

cd %LIBDIR%
del prana-deps.lib
md tmp
xcopy *.lib tmp

LIB.EXE /OUT:prana-deps.lib tmp\*

move prana-deps.lib %OUTDIR%

rmdir /s /q "tmp\"
cd %ROOTDIR%
