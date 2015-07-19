call "D:/Program Files/VS2012/Common7/Tools/vcvars32.bat"
set CFG=release-dll-ssl-dll-zlib-dll
set OPENSSL_PATH=../../openssl-1.0.0c
set ZLIB_PATH=../../zlib-1.2.5
nmake -f Makefile.vc10
