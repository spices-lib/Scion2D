#include "DirectoryWatcher.h"
#include <Logger.h>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#else

#endif

namespace fs = std::filesystem;
using namespace std::chrono_literals;

namespace SCION_FILESYSTEM {

	struct DirectoryWatcher::Impl
	{
		fs::path rootPath;
		Callback callback;
		std::atomic_bool bStopFlag{ false };
		std::thread watcherThread;

#ifdef _WIN32

		HANDLE directoryHandle{ nullptr };
		HANDLE shutdownHandle{ nullptr };
		OVERLAPPED overlapped{};


#else



#endif

		Impl(const fs::path& path, Callback cb)
			: rootPath{ path }
			, callback{ std::move(cb) }
		{
			watcherThread = std::thread([this] { Run(); });
		}

		~Impl();

		void Run();

#ifdef _WIN32

		void RunWindows();


#else

		void RunLinus();

#endif

	};

	DirectoryWatcher::DirectoryWatcher(const std::filesystem::path& path, Callback callback)
		: m_pImpl{ std::make_unique<Impl>(path, std::move(callback)) }
	{

	}

	DirectoryWatcher::~DirectoryWatcher()
	{

	}

	DirectoryWatcher::Impl::~Impl()
	{
		bStopFlag = true;
		if (watcherThread.joinable())
		{
#ifdef _WIN32
			SetEvent(shutdownHandle);
			CancelIoEx(directoryHandle, &overlapped);
#else

#endif
			watcherThread.join();
		}

#ifdef _WIN32
		if (directoryHandle && directoryHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(directoryHandle);
			directoryHandle = nullptr;
		}
		if (shutdownHandle && shutdownHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(shutdownHandle);
			shutdownHandle = nullptr;
		}
#else

#endif
	}

	void DirectoryWatcher::Impl::Run()
	{
#ifdef _WIN32
		RunWindows();
#else
		RunLinux();
#endif
	}

	void DirectoryWatcher::Impl::RunWindows()
	{
		constexpr DWORD bufferSize = 81192;
		BYTE buffer[bufferSize];

		HANDLE hDir = CreateFileW(
			rootPath.c_str(), 
			FILE_LIST_DIRECTORY, 
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
			nullptr, 
			OPEN_EXISTING, 
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, 
			nullptr
		);

		if (hDir == INVALID_HANDLE_VALUE)
		{
			SCION_ERROR("Failed to open directory, {}", GetLastError());
			return;
		}

		directoryHandle = hDir;

		shutdownHandle = CreateEventW(nullptr, TRUE, FALSE, nullptr);

		HANDLE hEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
		overlapped.hEvent = hEvent;

		HANDLE handles[] = { hEvent, shutdownHandle };

		while (!bStopFlag)
		{
			DWORD bytesReturned{ 0 };

			if (!hEvent)
			{
				SCION_ERROR("Failed to receive directory notifications.");
				break;
			}

			ResetEvent(hEvent);

			BOOL result = ReadDirectoryChangesW(
				hDir, 
				buffer,
				bufferSize,
				TRUE,
				FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
				&bytesReturned,
				&overlapped,
				nullptr
			);

			if (!result && GetLastError() != ERROR_IO_PENDING)
			{
				SCION_ERROR("ReadDirectoryChangeW failed: {}", GetLastError());
				break;
			}

			DWORD waitStatus = WaitForMultipleObjects(2, handles, FALSE, INFINITE);

			if (waitStatus == WAIT_OBJECT_0)
			{
				if (!GetOverlappedResult(hDir, &overlapped, &bytesReturned, FALSE))
				{
					DWORD err = GetLastError();
					if (err == ERROR_OPERATION_ABORTED)
					{
						break;
					}

					SCION_ERROR("GetOverlappedResult failed: {}", err);
					break;
				}

				FILE_NOTIFY_INFORMATION* pNotify = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);

				do
				{
					std::wstring sFilename(pNotify->FileName, pNotify->FileNameLength / sizeof(WCHAR));
					fs::path changedPath = rootPath / sFilename;

					if (callback)
					{
						bool bModified = pNotify->Action == FILE_ACTION_MODIFIED ||
							pNotify->Action == FILE_ACTION_RENAMED_NEW_NAME ||
							pNotify->Action == FILE_ACTION_ADDED;

						callback(changedPath, bModified);
					}

					if (pNotify->NextEntryOffset == 0)
						break;

					pNotify = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<BYTE*>(pNotify) + pNotify->NextEntryOffset);

				} while( true );
			}
			else if (waitStatus == WAIT_OBJECT_0 + 1)
			{
				CancelIoEx(hDir, &overlapped);
				break;
			}
			else
			{
				SCION_ERROR("WaitForMultipleObjects failed: {}", GetLastError());
				break;
			}
		}

		if (hEvent)
		{
			CloseHandle(hEvent);
		}
		if (hDir)
		{
			CloseHandle(hDir);
		}

		directoryHandle = nullptr;
	}

}