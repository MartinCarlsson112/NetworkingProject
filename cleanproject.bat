FOR /d /r %%d IN ("Binaries","Intermediate","Saved") DO @IF EXIST "%%d" rd /s /q "%%d"
