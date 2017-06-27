#include <Library/UefiLib.h>

EFI_STATUS
EFIAPI
UefiMain(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{	EFI_STATUS Status;

	EFI_SYSTEM_TABLE 	*gST;
	EFI_BOOT_SERVICES 	*gBS;
	
	gST = SystemTable;
	gBS = gST->BootServices;
	
	UINTN *Buffer;
	UINTN Size = 7;
	Status = gBS->AllocatePool(
		EfiBootServicesData,
		Size,
		(VOID **) &Buffer
	);
	
	if (Status==EFI_SUCCESS) {
		Buffer[0] = 2;
		Buffer[1] = 3;
		Buffer[2] = 4;
		Buffer[3] = 1;
		Buffer[4] = 5;
		Buffer[5] = 6;
		Buffer[6] = 7;
		
		UINTN index;
		for (index = 0; index < sizeof(Buffer); index++)
			Print(L"Buffer[%d] = %d\n", index, Buffer[index]);
	} else {
		Print(L"%r", Status);
	}
	
	gBS->FreePool(Buffer);
	
    return EFI_SUCCESS; 
}