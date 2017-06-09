#include <stdio.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Protocol/LoadedImage.h>
#include <Library/UefiBootServicesTableLib.h>

int
main (
  IN int Argc,
  IN char **Argv
  )
{	
	if (Argc < 3) {
		printf("Too few input arguments. Usage: REQ0208 [Controller Handle][Controller Path]\n");
		return 0;
	} else if (Argc > 3) {
		printf("Too many input arguments. Usage: REQ0208 [Controller Handle][Controller Path]\n");
		return 0;
	} else {
		printf("Argv[1]%s\n", Argv[1]);
		printf("Argv[2]%s\n", Argv[2]);
	}
	
	EFI_STATUS                              Status;
	
	UINTN                                   DevicePathHandleIndex, DevicePathHandleCount;
	UINTN									LoadedImageHandleIndex, LoadedImageHandleCount;
	//UINTN									DriverHandleIndex, DriverHandleCount;
	
	EFI_HANDLE                              *DevicePathHandlesBuffer;
	EFI_HANDLE                              *LoadedImageHandlesBuffer;
	//EFI_HANDLE                              *DriverBindingProtocolHandleBuffer;
	
	EFI_HANDLE								DevicePathHandle;
	EFI_HANDLE								LoadedImageHandle;
	EFI_HANDLE								ControllerHandle;
	EFI_HANDLE								ChildHandle;
	EFI_HANDLE								DriverHandle;
	//EFI_HANDLE								DriverHandle;
	
	EFI_DEVICE_PATH_PROTOCOL 				*DevicePathProtocol;
	EFI_DEVICE_PATH_PROTOCOL				*RemainingDevicePath;
	EFI_LOADED_IMAGE_PROTOCOL				*LoadedImageProtocol;
	EFI_DRIVER_BINDING_PROTOCOL 			*DriverBindingProtocol;
	
	//CHAR16 									*DriverName = L"QLogic FastLinQ Ethernet Driver";
	//CHAR16 									*ControllerPathText = L"PciRoot(0x0)/Pci(0x2,0x2)/Pci(0x0,0x0)";//Qlogic
	CHAR16 									*ControllerPathText = L"PciRoot(0x0)/Pci(0x1C,0x4)/Pci(0x0,0x3)";//Broadcom
	//CHAR16 									*ChildPathText = L"PciRoot(0x0)/Pci(0x2,0x2)/Pci(0x0,0x0)/MAC(1402ECCEE002,0x0)";
	CHAR16 									*ChildPathText = L"PciRoot(0x0)/Pci(0x1C,0x4)/Pci(0x0,0x3)/MAC(1402EC3E1423,0x0)";
	
	BOOLEAN									TestSuccess = TRUE;
	BOOLEAN									Test1_1_Scenario1_PASS = FALSE;
	BOOLEAN									Test1_1_Scenario2_PASS = FALSE;
	BOOLEAN									Test1_1_PASS = FALSE;
	BOOLEAN									Test1_2_PASS = FALSE;
	
	
	//--------------------------------------------------------------------------------
	//                    Get the Controller Handle & Child Handle                    
	//--------------------------------------------------------------------------------
	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiDevicePathProtocolGuid,
		NULL,
		&DevicePathHandleCount,
		&DevicePathHandlesBuffer);
	
	for (DevicePathHandleIndex = 0; DevicePathHandleIndex < DevicePathHandleCount; DevicePathHandleIndex++) {
		DevicePathHandle = DevicePathHandlesBuffer[DevicePathHandleIndex];
		//To get a pointer to the Protocol Interface. (EFI_DEVICE_PATH_PROTOCOL)
		Status = gBS->OpenProtocol (
			DevicePathHandle,
			&gEfiDevicePathProtocolGuid,
			&DevicePathProtocol,
			DevicePathHandle,
			NULL,
			EFI_OPEN_PROTOCOL_GET_PROTOCOL);
		
		if (!EFI_ERROR(Status)) {
			CHAR16 *Text = NULL;
			EFI_DEV_PATH *Node = (EFI_DEV_PATH*)DevicePathProtocol;
			Text = ConvertDevicePathToText(&Node->DevPath, FALSE, FALSE);
			
			if (StrLen(ControllerPathText) == StrLen(Text)) {
				if (StrnCmp(ControllerPathText, Text, StrLen(ControllerPathText)) == 0) {
					ControllerHandle = DevicePathHandle;
					Print(L"%p = controller handle\n", ControllerHandle);
					//Print(L"Handle (%p) is the controller handle, Device Path = %s\n", ControllerHandle, Text);
				}
			}
			
			if (StrLen(ChildPathText) == StrLen(Text)) {
				if (StrnCmp(ChildPathText, Text, StrLen(ChildPathText)) == 0) {
					ChildHandle = DevicePathHandle;
					Print(L"%p = child handle\n", ChildHandle);
					//Print(L"Handle (%p) is the child handle, Device Path = %s\n", ChildHandle, Text);
				}
			}
		}
	}
	
	//--------------------------------------------------------------------------------
	//                                Get the Driver Handle //--------------------------------------------------------------------------------
	Status = gBS->LocateHandleBuffer(
					ByProtocol,
					&gEfiLoadedImageProtocolGuid,
					NULL,
					&LoadedImageHandleCount,
					&LoadedImageHandlesBuffer);
	
	for (LoadedImageHandleIndex = 0; LoadedImageHandleIndex < LoadedImageHandleCount; LoadedImageHandleIndex++) {
		LoadedImageHandle = LoadedImageHandlesBuffer[LoadedImageHandleIndex];
		
		//To get a pointer to the Protocol Interface. (EFI_DEVICE_PATH_PROTOCOL)
		Status = gBS->OpenProtocol (
				LoadedImageHandle,
				&gEfiLoadedImageProtocolGuid,
				&LoadedImageProtocol,
				LoadedImageHandle,
				NULL,
				EFI_OPEN_PROTOCOL_GET_PROTOCOL);
		
		DevicePathProtocol = LoadedImageProtocol->FilePath;
		DevicePathHandle = LoadedImageProtocol->DeviceHandle;
		if (!EFI_ERROR(Status)) {
			CHAR16 *Text = NULL;
			EFI_DEV_PATH *Node = (EFI_DEV_PATH*)DevicePathProtocol;
			Text = ConvertDevicePathToText(&Node->DevPath, FALSE, FALSE);
			//Print(L"Image Handle (%p) is loaded from device handle (%p): Device Path = %s\n", LoadedImageHandle, DevicePathHandle, Text);
			if (DevicePathHandle == ControllerHandle) {
				DriverHandle = LoadedImageHandle;
				Print(L"%p = Driver handle\n", DriverHandle);
				//Print(L"-->loaded from device handle (%p): Device Path = %s\n", DevicePathHandle, Text);
			}
		}
	}
	
	//--------------------------------------------------------------------------------
	//              Set RemainingDevicePath as End of Device Path //--------------------------------------------------------------------------------
	CHAR16 *DeviceNodeText = L"Path(127,255)";
	RemainingDevicePath = ConvertTextToDeviceNode(DeviceNodeText);
	EFI_DEV_PATH *Node = (EFI_DEV_PATH*)RemainingDevicePath;
	if (!IsDevicePathEnd(Node)) {
		Print(L"Not an end device path node");
	}

	Print(L" \n");
	
	//--------------------------------------------------------------------------------
	//                             Start Testing
	//--------------------------------------------------------------------------------
	// If driver and controller are found, then start tests. Otherwise, failed to test.
	if (DriverHandle!=NULL && ControllerHandle!=NULL) {
		Status = gBS->OpenProtocol (
			DriverHandle,
			&gEfiDriverBindingProtocolGuid,
			&DriverBindingProtocol,
			DriverHandle,
			NULL,
			EFI_OPEN_PROTOCOL_GET_PROTOCOL);
		
		if(!EFI_ERROR(Status)) {
			EFI_STATUS Status1_1, Status1_2;
			
			// Scenario 1 - Driver is managing the controller.
			// Step 1: Test Supported() with NULL RemainingDevicePath
			Status1_1 = DriverBindingProtocol->Supported (
				DriverBindingProtocol,		// (in) Driver Binding Protocol Interface pointer
				ControllerHandle,			// (in) Controller Handle
				NULL						// (in) Remaining device path
				);
			Print(L"Test with NULL RemainingDevicePath --> %r\n", Status1_1);
			
			// Step 2: Test Supported() with NULL RemainingDevicePath
			Status1_2 = DriverBindingProtocol->Supported (
				DriverBindingProtocol,		// (in) Driver Binding Protocol Interface pointer
				ControllerHandle,			// (in) Controller Handle
				RemainingDevicePath			// (in) Remaining device path
				);
			
			Print(L"Test with End of device path node as RemainingDevicePath --> %r\n", Status1_2);
			
			if (Status1_1 == Status1_2) {
				Test1_1_Scenario1_PASS = TRUE;				
			}
			
			Print(L" \n");
			
			// Scenario 2 - Driver is not managing the controller.
			// Disconnect the driver and the controller
			if (Status1_1 == EFI_ALREADY_STARTED) {
				Status = gBS->DisconnectController (
					ControllerHandle,
					DriverHandle,
					NULL
					);
				if (EFI_ERROR(Status)) {
					Print(L"Disconnect returns %r\n", Status);
				}
			}
			
			EFI_STATUS Status2_1, Status2_2;
			// Test case 1.1
			// Step 1: Test Supported() with NULL RemainingDevicePath
			Status2_1 = DriverBindingProtocol->Supported (
				DriverBindingProtocol,		// (in) Driver Binding Protocol Interface pointer
				ControllerHandle,			// (in) Controller Handle
				NULL						// (in) Remaining device path
				);
			Print(L"Test with NULL as RemainingDevicePath --> %r\n", Status2_1);
			
			// Step 2: Test Supported() with NULL RemainingDevicePath
			Status2_2 = DriverBindingProtocol->Supported (
				DriverBindingProtocol,		// (in) Driver Binding Protocol Interface pointer
				ControllerHandle,			// (in) Controller Handle
				RemainingDevicePath			// (in) Remaining device path
				);
			
			Print(L"Test with End of device path node as RemainingDevicePath --> %r\n", Status2_2);
			
			if (Status2_1 == Status2_2) {
				Test1_1_Scenario2_PASS = TRUE;
			}
			
			if (Test1_1_Scenario1_PASS && Test1_1_Scenario2_PASS) {
				Test1_1_PASS = TRUE;
				Print(L"Test case 1.1 passed.\n");
			} else {
				Print(L"Test case 1.1 failed.\n");
			}
			
			Print(L" \n");
			
			// Test case 1.2
			BOOLEAN ChildHandleExist = FALSE;
			Status = DriverBindingProtocol->Start (
				DriverBindingProtocol,				// (in) Driver Binding Protocol Interface pointer
				ControllerHandle,					// (in) Controller Handle
				RemainingDevicePath					// (in) Remaining device path
				);
			
			if (EFI_ERROR(Status)) {
				Print(L"Start() return %r\n", Status);				
			}
			
			Status = gBS->LocateHandleBuffer(
				ByProtocol,
				&gEfiDevicePathProtocolGuid,
				NULL,
				&DevicePathHandleCount,
				&DevicePathHandlesBuffer);
			
			for (DevicePathHandleIndex = 0; DevicePathHandleIndex < DevicePathHandleCount; DevicePathHandleIndex++) {
				DevicePathHandle = DevicePathHandlesBuffer[DevicePathHandleIndex];
				Status = gBS->OpenProtocol (
					DevicePathHandle,
					&gEfiDevicePathProtocolGuid,
					&DevicePathProtocol,
					DevicePathHandle,
					NULL,
					EFI_OPEN_PROTOCOL_GET_PROTOCOL);
				
				if (!EFI_ERROR(Status)) {
					CHAR16 *Text = NULL;
					Node = (EFI_DEV_PATH*)DevicePathProtocol;
					Text = ConvertDevicePathToText(&Node->DevPath, FALSE, FALSE);
					
					if (StrLen(ChildPathText) == StrLen(Text)) {
						if (StrnCmp(ChildPathText, Text, StrLen(ChildPathText)) == 0) {
							ChildHandle = DevicePathHandle;
							ChildHandleExist = TRUE;
							Print(L"Child handle (%p) is listed.\n", ChildHandle);
							break;
						}
					}
				}
			}
			
			if (!ChildHandleExist) {
				Test1_2_PASS = TRUE;
				Print(L"Test case 1.2 passed.\n");
			} else {
				Print(L"Test case 1.2 failed.\n");
			}
			
		} else {
			TestSuccess = FALSE;
		}
	} else {
		TestSuccess = FALSE;
	}
	
	if (TestSuccess) {
		if (Test1_1_PASS && Test1_2_PASS) {
			Print(L"REQ0208,Pass,\n");
		} else {
			Print(L"REQ0208,Fail\n");
		}
	} else {
		Print(L"REQ0208,,Fail to test\n");
	}
	
	gBS->FreePool(DevicePathHandlesBuffer);
	gBS->FreePool(LoadedImageHandlesBuffer);

  return 0;
}