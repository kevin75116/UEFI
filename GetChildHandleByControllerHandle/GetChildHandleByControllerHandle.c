#include <Library/UefiLib.h>
#include <Library/HandleParsingLib.h>
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
	
	CHAR16 *DriverHandleHexId = args[1];
	UINTN DriverHandleId = StrHexToUintn(DriverHandleHexId);
	Print(L"%d\n", DriverHandleId);
	
	CHAR16 *CtrlHandleHexId = args[2];
	UINTN CtrlHandleId = StrHexToUintn(CtrlHandleHexId);
	Print(L"%d\n", CtrlHandleId);
	
	EFI_HANDLE *HandlesBuffer;
	UINTN HandlesCount;
	Status = gBS->LocateHandleBuffer(
		AllHandles,
		NULL,
		NULL,
		&HandlesCount,
		&HandlesBuffer);
		
	EFI_HANDLE DriverHandle;
	DriverHandle = HandlesBuffer[DriverHandleId-1];
	Print(L"DriverHandle = %p\n", DriverHandle);
	
	EFI_HANDLE ControllerHandle;
	ControllerHandle = HandlesBuffer[CtrlHandleId-1];
	Print(L"ControllerHandle = %p\n", ControllerHandle);
	
	gBS->FreePool(HandlesBuffer);
	
	UINTN MatchingHandleCount;
	EFI_HANDLE *MatchingHandleBuffer;
	ParseHandleDatabaseForChildControllers(ControllerHandle, &MatchingHandleCount, &MatchingHandleBuffer);
	
	UINTN HandleIndex;	
	for (HandleIndex = 0; HandleIndex < MatchingHandleCount; HandleIndex++) {
		Print(L"Child Handle = %p\n", MatchingHandleBuffer[HandleIndex]);
	}
	
	return EFI_SUCCESS;
}