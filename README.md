# UEFI
UEFI-related Programming

REQ0208 is exactly the same programm as GptExample_Support. The difference between them is Standard C Library. The REQ0208 is using the Standard C Library to provide a main() entry point and it allows user-input arguments; GptExample_Support is a EDK II native application that doesn't use any features of the Standard C Library.

[Prerequest]
1. Follow the guidelines in the following link to setup your EDK II environment.
   https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-with-EDK-II
   Recommand: To install the EDK II dev tool in the path C:\edk2 if the OS is Windows.
   
2. Follow the guidelines in the following link to make sure you are able to build a simple UEFI application.
   https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-Writing-Simple-Application

[Build]
REQ0208
1. Download folder REQ0208 and place it in the C:\edk2
2. Add the file path of REQ0208.inf (C:\edk2\REQ0208\REQ0208.inf) under [Components] section in the C:\edk2\AppPkg\AppPkg.dsc. Below is what it looks like.
   [Components]
   #### Sample Applications.
   AppPkg/Applications/Hello/Hello.inf        # No LibC includes or functions.
   AppPkg/Applications/Main/Main.inf          # Simple invocation. No other LibC functions.
   AppPkg/Applications/Enquire/Enquire.inf    #
   AppPkg/Applications/ArithChk/ArithChk.inf  #
   REQ0208/REQ0208.inf
3. Execute command "build -a X64 -p AppPkg\AppPkg.dsc" in command line window.
4. REQ0208.efi will be generated under edk2\Build\AppPkg\DEBUG_VS2015x86\X64\REQ0208\REQ0208\DEBUG and edk2\Build\AppPkg\DEBUG_VS2015x86\X64\REQ0208\REQ0208\OUTPUT.
