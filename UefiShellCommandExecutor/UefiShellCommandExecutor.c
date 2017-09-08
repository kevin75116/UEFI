#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/ShellLib.h>

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_SYSTEM_TABLE 		*gST;
  EFI_BOOT_SERVICES 	*gBS;
  
  gST = SystemTable;
  gBS = gST->BootServices;
  
  EFI_STATUS  CmdStat;
  EFI_STATUS  OpStat;
  CHAR16      *Command=L"drivers";
  OpStat = ShellExecute ( 
             &ImageHandle, 
			 Command, 
			 TRUE, 
			 NULL, 
			 &CmdStat
			 );
  return OpStat;
}