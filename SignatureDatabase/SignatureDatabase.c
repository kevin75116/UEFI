#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/ImageAuthentication.h>
#include <SignatureDatabase.h>


INTN
EFIAPI
ShellAppMain (
  IN UINTN   Argc,
  IN CHAR16  **Argv
  )
{
	extern BOOLEAN CompareSignatureType (IN EFI_GUID  Guid1, IN EFI_GUID  Guid2);
    EFI_STATUS                    Status;
	UINTN                         DataSize                = 0;
	UINTN                         SignatureCount          = 0;
	UINTN                         Index;
	UINT8			  			  *DataBuffer;
	EFI_RUNTIME_SERVICES          *gRT;
	EFI_SIGNATURE_LIST            *SignatureList;
	EFI_SIGNATURE_DATA            *Signature;
	
    gRT = gST->RuntimeServices;
    Status = gRT->GetVariable (
					EFI_IMAGE_SECURITY_DATABASE,
					&gEfiImageSecurityDatabaseGuid, 
					NULL,
					&DataSize,
					NULL);
	
	Print (L"GetVariable returns status = %r, data size = %d\n", Status, DataSize);
	
	if (Status == EFI_BUFFER_TOO_SMALL) {
		DataBuffer = (UINT8 *) AllocateZeroPool (DataSize);
		if (DataBuffer == NULL) {
			Print (L"Data buffer is NULL.\n");
			return FALSE;
		}
	}
	
	//
	// To get signature information from the authorized UEFI signature database
	//
	Status = gRT->GetVariable (
					EFI_IMAGE_SECURITY_DATABASE,
					&gEfiImageSecurityDatabaseGuid,
					NULL,
					&DataSize,
					DataBuffer);
	
	Print (L"GetVariable returns status = %r, data size = %d\n", Status, DataSize);
			 
	SignatureList = (EFI_SIGNATURE_LIST *) DataBuffer;
	
	CHAR16 *CertType;
    while ((DataSize > 0) && (DataSize >= SignatureList->SignatureListSize)) {
		CertType = L"Unknown";
		
		SignatureCount = (SignatureList->SignatureListSize - sizeof (EFI_SIGNATURE_LIST) - SignatureList->SignatureHeaderSize) / SignatureList->SignatureSize;
		
		if (CompareSignatureType (SignatureList->SignatureType, gEfiCertSha256Guid)) {
			CertType = L"EFI_CERT_SHA256_GUID";
		} else if (CompareSignatureType (SignatureList->SignatureType, gEfiCertRsa2048Guid)) {
			CertType = L"EFI_CERT_RSA2048_GUID";
		} else if (CompareSignatureType (SignatureList->SignatureType, gEfiCertRsa2048Sha256Guid)) {
			CertType = L"EFI_CERT_RSA2048_SHA256_GUID";
		} else if (CompareSignatureType (SignatureList->SignatureType, gEfiCertSha1Guid)) {
			CertType = L"EFI_CERT_SHA1_GUID";
		} else if (CompareSignatureType (SignatureList->SignatureType, gEfiCertRsa2048Sha1Guid)) {
			CertType = L"EFI_CERT_RSA2048_SHA1_GUID";
		} else if (CompareSignatureType (SignatureList->SignatureType, gEfiCertX509Guid)) {
			CertType = L"EFI_CERT_X509_GUID";
		} else if (CompareSignatureType (SignatureList->SignatureType, gEfiCertSha224Guid)) {
			CertType = L"EFI_CERT_SHA224_GUID";
		} else if (CompareSignatureType (SignatureList->SignatureType, gEfiCertSha384Guid)) {
			CertType = L"EFI_CERT_SHA384_GUID";
		} else if (CompareSignatureType (SignatureList->SignatureType, gEfiCertSha512Guid)) {
			CertType = L"EFI_CERT_SHA512_GUID";
		} else if (CompareSignatureType (SignatureList->SignatureType, gEfiCertX509Sha256Guid)) {
			CertType = L"EFI_CERT_X509_SHA256_GUID";
		} else if (CompareSignatureType (SignatureList->SignatureType, gEfiCertX509Sha384Guid)) {
			CertType = L"EFI_CERT_X509_SHA384_GUID";
		} else if (CompareSignatureType (SignatureList->SignatureType, gEfiCertX509Sha512Guid)) {
			CertType = L"EFI_CERT_X509_SHA512_GUID";
		} else if (CompareSignatureType (SignatureList->SignatureType, gEfiCertRsa2048Guid)) {
			CertType = L"EFI_CERT_TYPE_PKCS7_GUID";
		}
		
		Print (L"Size of signature list = %d\nSignature type = %s\nSignature list size = %d\nSignature header size = %d\nSignature size = %d\nCertificate count = %d\n",
				 sizeof (EFI_SIGNATURE_LIST),
				 CertType,
				 SignatureList->SignatureListSize,
				 SignatureList->SignatureHeaderSize,
				 SignatureList->SignatureSize,
				 SignatureCount);
		
		Signature = (EFI_SIGNATURE_DATA *) ((UINT8 *) SignatureList + sizeof (EFI_SIGNATURE_LIST) + SignatureList->SignatureHeaderSize);
		
		for (Index = 0; Index < SignatureCount; Index++) {
			EFI_GUID SignatureOwner = Signature->SignatureOwner;
			
			Print (L"Signature owner = { 0x%x, 0x%x, 0x%x, \\{ 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x } }\n",
					SignatureOwner.Data1,
					SignatureOwner.Data2,
					SignatureOwner.Data3,
					SignatureOwner.Data4[0],
					SignatureOwner.Data4[1],
					SignatureOwner.Data4[2],
					SignatureOwner.Data4[3],
					SignatureOwner.Data4[4],
					SignatureOwner.Data4[5],
					SignatureOwner.Data4[6],
					SignatureOwner.Data4[7]
					);
						
			// Move the signature pointer to the end of the signature
			Signature = (EFI_SIGNATURE_DATA *) ((UINT8 *) Signature + SignatureList->SignatureSize);
		}
		
		DataSize -= SignatureList->SignatureListSize;
		// Move the signature list pointer to next signature list
		SignatureList = (EFI_SIGNATURE_LIST *) ((UINT8 *) SignatureList + SignatureList->SignatureListSize);
		
		Print (L"\n");
	}
	
    FreePool (DataBuffer);
	
    return EFI_SUCCESS;
}

BOOLEAN
CompareSignatureType (
  IN EFI_GUID  Guid1,
  IN EFI_GUID  Guid2
  )
{
	BOOLEAN Bool = FALSE;
	if (Guid1.Data1 == Guid2.Data1 &&
		Guid1.Data2 == Guid2.Data2 &&
		Guid1.Data3 == Guid2.Data3 &&
		Guid1.Data4[0] == Guid2.Data4[0] &&
		Guid1.Data4[1] == Guid2.Data4[1] &&
		Guid1.Data4[2] == Guid2.Data4[2] &&
		Guid1.Data4[3] == Guid2.Data4[3] &&
		Guid1.Data4[4] == Guid2.Data4[4] &&
		Guid1.Data4[5] == Guid2.Data4[5] &&
		Guid1.Data4[6] == Guid2.Data4[6] &&
		Guid1.Data4[7] == Guid2.Data4[7]) 
	{
		Bool = TRUE;
	}
	
	return Bool;
}