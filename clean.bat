IF EXIST "x64" (  
    RMDIR /S /Q "x64"
)
IF EXIST "x86" (  
    RMDIR /S /Q "x86"
)
IF EXIST "MSpring_test\x64" (  
    RMDIR /S /Q "MSpring_test\x64"
)
IF EXIST "MSpring_test\x86" (  
    RMDIR /S /Q "MSpring_test\x86"
)

IF EXIST "install_mspring\Debug" (  
    RMDIR /S /Q "install_mspring\Debug"
)
IF EXIST "install_mspring\Release" (  
    RMDIR /S /Q "install_mspring\Release"
)