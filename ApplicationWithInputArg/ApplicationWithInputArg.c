#include <Library/UefiLib.h>
#include <Protocol/EfiShellParameters.h>

EFI_STATUS
EFIAPI
UefiMain(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
	EFI_SYSTEM_TABLE 	*gST;
	EFI_BOOT_SERVICES 	*gBS;
	
	gST = SystemTable;
	gBS = gST->BootServices;
	
	EFI_STATUS Status;
	EFI_SHELL_PARAMETERS_PROTOCOL *EfiShellParametersProtocol;
	
	Status = gBS->OpenProtocol(
		ImageHandle,
		&gEfiShellParametersProtocolGuid,
		(VOID **)&EfiShellParametersProtocol,
		ImageHandle,
		NULL,
		EFI_OPEN_PROTOCOL_GET_PROTOCOL
		);
							
	CHAR16 **args = EfiShellParametersProtocol->Argv;
	UINTN arg_count = EfiShellParametersProtocol->Argc;
	
	Print(L"Argc = %d\n", arg_count);
	
	UINTN index;
	for (index = 0; index < arg_count; index++) {
		Print(L"Args = %s, length = %d\n", args[index], StrLen(args[index]));
	}
	
	return EFI_SUCCESS;
}