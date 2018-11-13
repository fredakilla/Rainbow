set ROOTDIR=%CD%
set LIBDIR=%1
set OUTDIR=%2

echo "Amalgamating target dir: %LIBDIR%"

md "%LIBDIR%"

cd %LIBDIR%
del rainbow-deps.lib
md tmp
xcopy *.lib tmp

LIB.EXE /OUT:rainbow-deps.lib tmp\*

move rainbow-deps.lib %OUTDIR%

rmdir /s /q "tmp\"
cd %ROOTDIR%
