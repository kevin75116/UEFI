#include <Uefi.h> 
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Protocol/LoadedImage.h>

EFI_STATUS
EFIAPI
UefiMain(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
	EFI_SYSTEM_TABLE 		*gST;
	EFI_BOOT_SERVICES 	*gBS;
	
	gST = SystemTable;
	gBS = gST->BootServices;
	/*
	CHAR16 *DevicePath = L"PciRoot(0x0)/Pci(0x2,0x2)/Pci(0x0,0x0)/MAC(1402ECCEE002,0x0)";
	Print(L"Device Path Text = %s\n", DevicePath);
	
	Print(L"ConvertTextToDevicePath...\n");
	EFI_DEVICE_PATH_PROTOCOL	*RemainingDevicePath;
	RemainingDevicePath = ConvertTextToDevicePath(DevicePath);
	Print(L"--> Type = %d\n", RemainingDevicePath->Type);
	Print(L"--> SubType = %d\n\n", RemainingDevicePath->SubType);
	
	CHAR16 *Text = NULL;
	Print(L"IsDevicePathEnd...\n");
	EFI_DEV_PATH *Node = (EFI_DEV_PATH*)RemainingDevicePath;
	if (IsDevicePathEnd(Node)) {
		Text = ConvertDeviceNodeToText(&Node->DevPath, FALSE, FALSE);
		Print(L"Yes --> Device Node = %s\n", Text);
	} else {
		Text = ConvertDeviceNodeToText(&Node->DevPath, FALSE, FALSE);
		Print(L"No --> Device Node = %s\n", Text);
		
		EFI_DEVICE_PATH_PROTOCOL *nextNodePath = NULL;
		EFI_DEV_PATH *NextNode = NULL;
		
		nextNodePath = NextDevicePathNode(Node);
		NextNode = (EFI_DEV_PATH*)nextNodePath;
		Text = ConvertDeviceNodeToText(&NextNode->DevPath, FALSE, FALSE);
		Print(L"----> Next Device Node = %s\n", Text);
		
		nextNodePath = NextDevicePathNode(NextNode);
		NextNode = (EFI_DEV_PATH*)nextNodePath;
		Text = ConvertDeviceNodeToText(&NextNode->DevPath, FALSE, FALSE);
		Print(L"----> Next Device Node = %s\n", Text);
		
	}
	
	Print(L"SetDevicePathEndNode and then IsDevicePathEnd...\n");
	SetDevicePathEndNode(Node);
	if (IsDevicePathEnd(Node)) {
		Text = ConvertDeviceNodeToText(&Node->DevPath, FALSE, FALSE);
		Print(L"Yes --> Device Node = %s, ", Text);
		Text = ConvertDevicePathToText(&Node->DevPath, FALSE, FALSE);
		Print(L"Device Path = %s\n", Text);
	} else {
		Text = ConvertDeviceNodeToText(&Node->DevPath, FALSE, FALSE);
		Print(L"No --> Device Node = %s, ", Text);
		Text = ConvertDevicePathToText(&Node->DevPath, FALSE, FALSE);
		Print(L"Device Path = %s\n", Text);
	}
	
	CHAR16 *DeviceNodeText = L"Path(127,255)";
	Print(L"Device Node Text = %s\n", DeviceNodeText);
	RemainingDevicePath = ConvertTextToDeviceNode(DeviceNodeText);
	Node = (EFI_DEV_PATH*)RemainingDevicePath;
	if (IsDevicePathEnd(Node)) {
		Text = ConvertDeviceNodeToText(&Node->DevPath, FALSE, FALSE);
		Print(L"Yes --> Device Node = %s, ", Text);
		Text = ConvertDevicePathToText(&Node->DevPath, FALSE, FALSE);
		Print(L"Device Path = %s\n", Text);
	}									
	*/
	extern EFI_GUID gEfiDevicePathProtocolGuid;
	extern EFI_GUID gEfiDriverBindingProtocolGuid;
	extern EFI_GUID gEfiLoadedImageProtocolGuid;
	
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
			ImageHandle,
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
					ImageHandle,
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
	//gBS->FreePool(DriverBindingProtocolHandleBuffer);
	
	
	/*
	extern EFI_GUID gEfiDriverBindingProtocolGuid;
	extern EFI_GUID gEfiAdapterInformationProtocolGuid;
	extern EFI_GUID gEfiComponentName2ProtocolGuid;
	
	EFI_STATUS                              Status;
	UINTN                                   AdapterInfoHandleIndex, AdapterInfoHandleCount;
	UINTN									DriverHandleIndex, DriverHandleCount;	
	UINTN									ComponentName2HandleIndex, ComponentName2HandleCount;
	
	EFI_HANDLE                              *DriverBindingProtocolHandleBuffer;
	EFI_HANDLE                              *AdapterInfoHandlesBuffer;
	EFI_HANDLE                              *ComponentName2HandleBuffer;
	
	EFI_HANDLE								DriverImageHandle;
	EFI_HANDLE 								ControllerHandle;
	EFI_HANDLE 								ComponentName2Handle;
	
	EFI_DRIVER_BINDING_PROTOCOL 			*DriverBindingProtocol;
	EFI_COMPONENT_NAME2_PROTOCOL			*ComponentName2Protocol;
	
	EFI_DEVICE_PATH_PROTOCOL 				*RemainingDevicePath;
	
	CHAR16									*TestDriverName = L"QLogic FastLinQ Ethernet Driver";
	
	
	
	
	// Get list of all Driver Binding Protocol Instances
	Status = gBS->LocateHandleBuffer(
					ByProtocol,
					&gEfiDriverBindingProtocolGuid,
					NULL,
					&DriverHandleCount,
					&DriverBindingProtocolHandleBuffer);
	
	Status = gBS->LocateHandleBuffer(
							AllHandles,
							NULL,
							NULL,
							&AdapterInfoHandleCount,
							&AdapterInfoHandlesBuffer);
	
	Status = gBS->LocateHandleBuffer(
							ByProtocol,
							&gEfiComponentName2ProtocolGuid,
							NULL,
							&ComponentName2HandleCount,
							&ComponentName2HandleBuffer);
							
	for (DriverHandleIndex = 0; DriverHandleIndex < DriverHandleCount; DriverHandleIndex++) {
		DriverImageHandle = DriverBindingProtocolHandleBuffer[DriverHandleIndex];
		
		//To get a pointer to the Protocol Interface. (EFI_DRIVER_BINDING_PROTOCOL)
		Status = gBS->OpenProtocol (
				DriverImageHandle,
				&gEfiDriverBindingProtocolGuid,
				&DriverBindingProtocol,
				DriverImageHandle,
				NULL,
				EFI_OPEN_PROTOCOL_GET_PROTOCOL);
		
		if (!EFI_ERROR(Status)) {
			for (ComponentName2HandleIndex = 0; ComponentName2HandleIndex < ComponentName2HandleCount; ComponentName2HandleIndex++) {
				ComponentName2Handle = ComponentName2HandleBuffer[ComponentName2HandleIndex];
				if (ComponentName2Handle == DriverImageHandle) {
					Status = gBS->OpenProtocol (
							ComponentName2Handle,
							&gEfiComponentName2ProtocolGuid,
							&ComponentName2Protocol,
							ComponentName2Handle,
							NULL,
							EFI_OPEN_PROTOCOL_GET_PROTOCOL);
							
					if (!EFI_ERROR (Status)) {
						CHAR16 *DriverName;
						Status = ComponentName2Protocol->GetDriverName (
							ComponentName2Protocol,						// (in) Component Name2 Protocol Interface
							ComponentName2Protocol->SupportedLanguages, // (in) Language
							&DriverName									// (out) Driver name
							);
						if (Status == EFI_SUCCESS) {
							if (StrnCmp(DriverName, TestDriverName, 32) == 0) {
								Print(L"Driver Name = %s, Driver Handle = %p\n", DriverName, DriverImageHandle);
								for (AdapterInfoHandleIndex = 0; AdapterInfoHandleIndex < AdapterInfoHandleCount; AdapterInfoHandleIndex++) {
									ControllerHandle = AdapterInfoHandlesBuffer[AdapterInfoHandleIndex];
									RemainingDevicePath = NULL;
									Status = DriverBindingProtocol->Supported (
											DriverBindingProtocol,		// (in) Driver Binding Protocol Interface pointer
											ControllerHandle,			// (in) Controller Handle
											RemainingDevicePath			// (in) Remaining device path
											);
										
									if (Status==EFI_SUCCESS) {
										Print(L"%p --> Driver Handle (%p) supported.\n", ControllerHandle,DriverImageHandle);
									} else if (Status==EFI_ALREADY_STARTED) {
										Print(L"%p --> Driver Handle (%p) already started.\n", ControllerHandle, DriverImageHandle);
									} else if (Status==EFI_ACCESS_DENIED) {
										Print(L"%p --> Driver Handle (%p) access denied.\n", ControllerHandle, DriverImageHandle);
									} else if (Status==EFI_UNSUPPORTED) {
										Print(L"%p --> Driver Handle (%p) unsupported.\n", ControllerHandle, DriverImageHandle);
									} else if (EFI_ERROR (Status)) {
										Print(L"%p --> Driver Handle (%p) support() function has error(s).\n", ControllerHandle, DriverImageHandle);
									}
									
									CHAR16 *ControllerName;
									Status = ComponentName2Protocol->GetControllerName (
										ComponentName2Protocol,						// (in) Component Name2 Protocol Interface
										ControllerHandle,							// (in) Controller Handle
										NULL,										// (in optional) Child Handle
										ComponentName2Protocol->SupportedLanguages, // (in) Language
										&ControllerName									// (out) Driver name
										);
									//Print(L"Controller Name = %s\n", ControllerName);
									
								}
							}
						}
					}
					Status = gBS->CloseProtocol (
							ComponentName2Handle,
							&gEfiComponentName2ProtocolGuid,
							ComponentName2Handle,
							NULL);
					break;
				}
			}
		}
	}
	
	Status = gBS->CloseProtocol (
				DriverImageHandle,
				&gEfiDriverBindingProtocolGuid,
				DriverImageHandle,
				NULL);
	
	gBS->FreePool(DriverBindingProtocolHandleBuffer);
	gBS->FreePool(AdapterInfoHandlesBuffer);
	gBS->FreePool(ComponentName2HandleBuffer);
	*/
    return Status;
}