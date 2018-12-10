#include "stdafx.h"
#include "copyDllHook.h"
#include "ntstatus.h"
#include <string>
#include "../../OutGoingFileTool/OutGoingFileTool/FIlestruct.h"
#include <mutex>

typedef struct _FILE_END_OF_FILE_INFORMATION {
	LARGE_INTEGER EndOfFile;
} FILE_END_OF_FILE_INFORMATION, *PFILE_END_OF_FILE_INFORMATION;

typedef struct _FILE_INTERNAL_INFORMATION {
	LARGE_INTEGER IndexNumber;
} FILE_INTERNAL_INFORMATION, *PFILE_INTERNAL_INFORMATION;

typedef struct _FILE_EA_INFORMATION {
	ULONG EaSize;
} FILE_EA_INFORMATION, *PFILE_EA_INFORMATION;

typedef struct _FILE_ACCESS_INFORMATION {
	ACCESS_MASK AccessFlags;
} FILE_ACCESS_INFORMATION, *PFILE_ACCESS_INFORMATION;

typedef struct _FILE_DIRECTORY_INFORMATION {
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	WCHAR         FileName666[1];
} FILE_DIRECTORY_INFORMATION, *PFILE_DIRECTORY_INFORMATION;

typedef struct _FILE_FULL_DIR_INFORMATION {
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	ULONG         EaSize;
	WCHAR         FileName77[1];
} FILE_FULL_DIR_INFORMATION, *PFILE_FULL_DIR_INFORMATION;



typedef struct _FILE_MODE_INFORMATION {
	ULONG Mode;
} FILE_MODE_INFORMATION, *PFILE_MODE_INFORMATION;

typedef struct _FILE_ALIGNMENT_INFORMATION {
	ULONG AlignmentRequirement;
} FILE_ALIGNMENT_INFORMATION, *PFILE_ALIGNMENT_INFORMATION;

typedef struct _FILE_NAME_INFORMATION {
	ULONG FileNameLength;
	WCHAR FileNames[1];
} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;

typedef struct _FILE_ALL_INFORMATION {
	FILE_BASIC_INFORMATION     BasicInformation;
	FILE_STANDARD_INFORMATION  StandardInformation;
	FILE_INTERNAL_INFORMATION  InternalInformation;
	FILE_EA_INFORMATION        EaInformation;
	FILE_ACCESS_INFORMATION    AccessInformation;
	FILE_POSITION_INFORMATION  PositionInformation;
	FILE_MODE_INFORMATION      ModeInformation;
	FILE_ALIGNMENT_INFORMATION AlignmentInformation;
	FILE_NAME_INFORMATION      NameInformation;
} FILE_ALL_INFORMATION, *PFILE_ALL_INFORMATION;

typedef struct _FILE_BOTH_DIR_INFORMATION {
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	ULONG         EaSize;
	CCHAR         ShortNameLength;
	WCHAR         ShortName[12];
	WCHAR         FileName33[1];
} FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;

typedef struct _FILE_ID_FULL_DIR_INFORMATION {
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	ULONG         EaSize;
	LARGE_INTEGER FileId;
	WCHAR         FileNam2e1[1];
} FILE_ID_FULL_DIR_INFORMATION, *PFILE_ID_FULL_DIR_INFORMATION;




typedef struct _FILE_ID_GLOBAL_TX_DIR_INFORMATION {
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	LARGE_INTEGER FileId;
	GUID          LockingTransactionId;
	ULONG         TxInfoFlags;
	WCHAR         FileName22[1];
} FILE_ID_GLOBAL_TX_DIR_INFORMATION, *PFILE_ID_GLOBAL_TX_DIR_INFORMATION;
typedef struct _FILE_ID_BOTH_DIR_INFORMATION {
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	ULONG         EaSize;
	CCHAR         ShortNameLength;
	WCHAR         ShortName[12];
	LARGE_INTEGER FileId;
	WCHAR         FileNam335e[1];
} FILE_ID_BOTH_DIR_INFORMATION, *PFILE_ID_BOTH_DIR_INFORMATION;

FILE_ALL_INFORMATION fsialls;
NTSTATUS NTAPI Fake_ZwQueryDirectoryFile(
	_In_     HANDLE                 FileHandle,
	_In_opt_ HANDLE                 Event,
	_In_opt_ PIO_APC_ROUTINE        ApcRoutine,
	_In_opt_ PVOID                  ApcContext,
	_Out_    PIO_STATUS_BLOCK       IoStatusBlock,
	_Out_    PVOID                  FileInformation,
	_In_     ULONG                  Length,
	_In_     FILE_INFORMATION_CLASS FileInformationClass,
	_In_     BOOLEAN                ReturnSingleEntry,
	_In_opt_ PUNICODE_STRING        FileName0,
	_In_     BOOLEAN                RestartScan
)
{
	NTSTATUS status;
	IO_STATUS_BLOCK iostatus = { 0 };
	NTSTATUS ntStatus = 0;
	FILE_STANDARD_INFORMATION fsi = { 0 };
	std::mutex mutexObj;
	bool bRet;
	int HeadFlaglength = sizeof(RjFileSrtuct) + 1;
	//OutputDebugStringEx("Fake_ZwQueryDirectoryFile::获取的文件长度:%d", fsi.EndOfFile.QuadPart);
	status = m_pfnOriginalZwQueryDirectoryFile(FileHandle,
		Event,
		ApcRoutine,
		ApcContext,
		IoStatusBlock,
		FileInformation,
		Length,
		FileInformationClass,
		ReturnSingleEntry,
		FileName0,
		RestartScan);
	if (!NT_SUCCESS(ntStatus))
		return ntStatus;
	/*if (FileInformationClass == FileStandardInformation)
	{
		bRet = !m_handleList.empty();
		if (bRet)
		{
			mutexObj.lock();
			for (handleListNode = m_handleList.begin(); handleListNode != m_handleList.end(); handleListNode++)
			{
				if (handleListNode->FileHandle == FileHandle)
				{
					fsi = *(FILE_STANDARD_INFORMATION*)FileInformation;
					OutputDebugStringEx("QueryDirectoryFile：：FileStandardInformation获取的文件长度:%d", fsi.EndOfFile.QuadPart);
					if (fsi.EndOfFile.QuadPart >= HeadFlaglength)
						((FILE_STANDARD_INFORMATION*)FileInformation)->EndOfFile.QuadPart -= HeadFlaglength;
				}
			}
			mutexObj.unlock();
		}
	}
	if (FileInformationClass == FileEndOfFileInformation)
	{
		bRet = !m_handleList.empty();
		if (bRet)
		{
			mutexObj.lock();
			for (handleListNode = m_handleList.begin(); handleListNode != m_handleList.end(); handleListNode++)
			{
				if (handleListNode->FileHandle == FileHandle)
				{
					auto fsiend = *(FILE_END_OF_FILE_INFORMATION*)FileInformation;
					OutputDebugStringEx("QueryDirectoryFile：：FileEndOfFileInformation获取的文件长度:%d", fsiend.EndOfFile.QuadPart);
					((FILE_END_OF_FILE_INFORMATION*)FileInformation)->EndOfFile.QuadPart -= HeadFlaglength;
				}
			}
			mutexObj.unlock();
		}
	}
	if (FileInformationClass == FileAllInformation)
	{
		bRet = !m_handleList.empty();
		if (bRet)
		{
			mutexObj.lock();
			for (handleListNode = m_handleList.begin(); handleListNode != m_handleList.end(); handleListNode++)
			{
				if (handleListNode->FileHandle == FileHandle)
				{
					fsialls = *(FILE_ALL_INFORMATION*)FileInformation;
					OutputDebugStringEx("QueryDirectoryFile：：FileAllInformation获取的文件长度:%d", fsialls.StandardInformation.EndOfFile.QuadPart);
					((FILE_ALL_INFORMATION*)FileInformation)->StandardInformation.EndOfFile.QuadPart -= HeadFlaglength;
				}
			}
			mutexObj.unlock();
		}
	}
	if (FileInformationClass == FileBothDirectoryInformation)
	{
		bRet = !m_handleList.empty();
		if (bRet)
		{
			mutexObj.lock();
			for (handleListNode = m_handleList.begin(); handleListNode != m_handleList.end(); handleListNode++)
			{
				if (handleListNode->FileHandle == FileHandle)
				{
					auto fsiBOTH = *(FILE_BOTH_DIR_INFORMATION*)FileInformation;
					OutputDebugStringEx("QueryDirectoryFile：：FILE_BOTH_DIR_INFORMATION获取的文件长度:%d", fsiBOTH.EndOfFile.QuadPart);
					((FILE_BOTH_DIR_INFORMATION*)FileInformation)->EndOfFile.QuadPart -= HeadFlaglength;
				}
			}
			mutexObj.unlock();
		}
	}
	if (FileInformationClass == FileIdGlobalTxDirectoryInformation)
	{
		bRet = !m_handleList.empty();
		if (bRet)
		{
			mutexObj.lock();
			for (handleListNode = m_handleList.begin(); handleListNode != m_handleList.end(); handleListNode++)
			{
				if (handleListNode->FileHandle == FileHandle)
				{
					auto fsiGlobalTxDirectory = *(FILE_ID_GLOBAL_TX_DIR_INFORMATION*)FileInformation;
					OutputDebugStringEx("QueryDirectoryFile：：FileIdGlobalTxDirectoryInformation获取的文件长度:%d", fsiGlobalTxDirectory.EndOfFile.QuadPart);
					((FILE_BOTH_DIR_INFORMATION*)FileInformation)->EndOfFile.QuadPart -= HeadFlaglength;
				}
			}
			mutexObj.unlock();
		}
	}
	if (FileInformationClass == FileIdFullDirectoryInformation)
	{
		bRet = !m_handleList.empty();
		if (bRet)
		{
			mutexObj.lock();
			for (handleListNode = m_handleList.begin(); handleListNode != m_handleList.end(); handleListNode++)
			{
				if (handleListNode->FileHandle == FileHandle)
				{
					auto fsiGlobalTxDirectory = *(FILE_ID_FULL_DIR_INFORMATION*)FileInformation;
					OutputDebugStringEx("QueryDirectoryFile：：FileIdFullDirectoryInformation获取的文件长度:%d", fsiGlobalTxDirectory.EndOfFile.QuadPart);
					((FILE_ID_FULL_DIR_INFORMATION*)FileInformation)->EndOfFile.QuadPart -= HeadFlaglength;
				}
			}
			mutexObj.unlock();
		}
	}
	if (FileInformationClass == FileNetworkOpenInformation)
	{
		bRet = !m_handleList.empty();
		if (bRet)
		{
			mutexObj.lock();
			for (handleListNode = m_handleList.begin(); handleListNode != m_handleList.end(); handleListNode++)
			{
				if (handleListNode->FileHandle == FileHandle)
				{
					auto fsiGlobalTxDirectory = *(FILE_NETWORK_OPEN_INFORMATION*)FileInformation;
					OutputDebugStringEx("QueryDirectoryFile：：FileNetworkOpenInformation获取的文件长度:%d", fsiGlobalTxDirectory.EndOfFile.QuadPart);
					((FILE_NETWORK_OPEN_INFORMATION*)FileInformation)->EndOfFile.QuadPart -= HeadFlaglength;
				}
			}
			mutexObj.unlock();
		}
	}
	if (FileInformationClass == FileIdBothDirectoryInformation)
	{
		bRet = !m_handleList.empty();
		if (bRet)
		{
			mutexObj.lock();
			for (handleListNode = m_handleList.begin(); handleListNode != m_handleList.end(); handleListNode++)
			{
				if (handleListNode->FileHandle == FileHandle)
				{
					auto fsiGlobalTxDirectory = *(FILE_ID_BOTH_DIR_INFORMATION*)FileInformation;
					OutputDebugStringEx("QueryDirectoryFile：：FileIdBothDirectoryInformation获取的文件长度:%d", fsiGlobalTxDirectory.EndOfFile.QuadPart);
					((FILE_ID_BOTH_DIR_INFORMATION*)FileInformation)->EndOfFile.QuadPart -= HeadFlaglength;
				}
			}
			mutexObj.unlock();
		}
	}
	if (FileInformationClass == FilePositionInformation)
	{
		bRet = !m_handleList.empty();
		if (bRet)
		{
			mutexObj.lock();
			for (handleListNode = m_handleList.begin(); handleListNode != m_handleList.end(); handleListNode++)
			{
				if (handleListNode->FileHandle == FileHandle)
				{
					auto fsiGlobalTxDirectory = *(FILE_POSITION_INFORMATION*)FileInformation;
					OutputDebugStringEx("QueryDirectoryFile：：FilePositionInformation获取的文件长度:%d", fsiGlobalTxDirectory.CurrentByteOffset.QuadPart);
					if (fsiGlobalTxDirectory.CurrentByteOffset.QuadPart >= HeadFlaglength)
					{
						OutputDebugStringEx("改长度大于文件头");
						((FILE_POSITION_INFORMATION*)FileInformation)->CurrentByteOffset.QuadPart -= HeadFlaglength;
					}
				}
			}
			mutexObj.unlock();
		}
	}
	if (FileInformationClass == FileDirectoryInformation)
	{
		bRet = !m_handleList.empty();
		if (bRet)
		{
			mutexObj.lock();
			for (handleListNode = m_handleList.begin(); handleListNode != m_handleList.end(); handleListNode++)
			{
				if (handleListNode->FileHandle == FileHandle)
				{
					auto fsiGlobalTxDirectory = *(FILE_DIRECTORY_INFORMATION*)FileInformation;
					OutputDebugStringEx("QueryDirectoryFile：：FileDirectoryInformation获取的文件长度:%d", fsiGlobalTxDirectory.EndOfFile.QuadPart);
					((FILE_DIRECTORY_INFORMATION*)FileInformation)->EndOfFile.QuadPart -= HeadFlaglength;
				}
			}
			mutexObj.unlock();
		}
	}
	if (FileInformationClass == FileFullDirectoryInformation)
	{
		bRet = !m_handleList.empty();
		if (bRet)
		{
			mutexObj.lock();
			for (handleListNode = m_handleList.begin(); handleListNode != m_handleList.end(); handleListNode++)
			{
				if (handleListNode->FileHandle == FileHandle)
				{
					auto fsiGlobalTxDirectory = *(FILE_FULL_DIR_INFORMATION*)FileInformation;
					OutputDebugStringEx("QueryDirectoryFile：：FileDirectoryInformation获取的文件长度:%d", fsiGlobalTxDirectory.EndOfFile.QuadPart);
					((FILE_FULL_DIR_INFORMATION*)FileInformation)->EndOfFile.QuadPart -= HeadFlaglength;
				}
			}
			mutexObj.unlock();
		}
	}
*/
	return ntStatus;
}
