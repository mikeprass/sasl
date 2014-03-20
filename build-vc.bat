set CXXFLAGS=/DSNAPSHOT=201201031
cd libavionics
del *.o
del *.obj
del libavionics.a
del xavionics.lib
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
mingw32-make -f Makefile.vc
@rem cd ../slava
@rem del *.o
@rem del *.obj
@rem del slava.exe
@rem mingw32-make -f Makefile.vc
