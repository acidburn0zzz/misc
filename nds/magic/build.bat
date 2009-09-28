@echo off
goto build

:clean
make clean || goto fail
goto build

:build
make || goto fail
goto exec_no_cash

:exec_no_cash
C:\Jeux\nintendo_ds\nogba-2.6a\nogba.exe C:\Users\mathieu\Desktop\magic\magic.nds
exit

:exec_dsemume
C:\Jeux\nintendo_ds\desmume-0.9.4\desmume_dev.exe C:\Users\mathieu\Desktop\magic\magic.nds
exit

:fail
pause
