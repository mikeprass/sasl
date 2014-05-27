set CXXFLAGS=/DSNAPSHOT=201405261
set CXXFLAGS=/DNDEBUG=1
cd libavionics
del *.o
del *.obj
del libavionics.a
del avionics.lib
mingw32-make -f Makefile.vc
cd ../libaccgl
del *.o
del *.obj
del libaccgl.a
del accgl.lib
mingw32-make -f Makefile.vc
cd ../alsound
del *.o
del *.obj
del libalsound.a
del alsound.lib
mingw32-make -f Makefile.vc
cd ../xap
del *.o
del *.obj
del win.xpl
mingw32-make -f Makefile.vc.64
@rem cd ../slava
@rem del *.o
@rem del *.obj
@rem del slava.exe
@rem mingw32-make -f Makefile.vc
cd ..