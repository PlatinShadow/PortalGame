#include "Asset/Watcher.h"

#include <Windows.h>
#include <array>
#include <filesystem>

#include "Core/Base.h"

namespace PGame {
	namespace Asset {
		Watcher::Watcher() {
			pgDebug("Created Directory Watcher");
		}

		Watcher::Watcher(const std::string& directory)
			: m_Handle(nullptr) {
			pgDebug("Created Directory Watcher - Dir=" << directory);
			m_Directory = std::filesystem::absolute(directory).string();
		}

		Watcher::~Watcher() {
			if (m_WatchThread.joinable()) {
				Stop();
			}
		}

		bool Watcher::Start(const std::string& directory) {
			m_Directory = std::filesystem::absolute(directory).string();
			return Start();
		}

		bool Watcher::Start() {
			if (m_Directory.empty()) {
				pgError("Can't watch empty path!");
				return PG_FAILURE;
			}

			m_Handle = CreateFile(
				m_Directory.c_str(),
				FILE_LIST_DIRECTORY,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_BACKUP_SEMANTICS,
				NULL);

			if (m_Handle == INVALID_HANDLE_VALUE) {
				pgError("Couldn't open handle to watch directory -> " << m_Directory);
				return false;
			}

			m_Running = true;
			m_WatchThread = std::thread(&Watcher::Watch, this);

			pgDebug("Starting to watch " << m_Directory);

			return true;
		}

		void Watcher::Stop() {
			m_Running = false;
			CancelIoEx(m_Handle, NULL);
			m_WatchThread.join();
			CloseHandle(m_Handle);
		}

		void Watcher::Watch() {
			std::array<BYTE, 4069> buffer = { 0 };
			DWORD bytesReturned;

			while (m_Running.load()) {
				BOOL succ = ReadDirectoryChangesW(m_Handle, &buffer[0], (DWORD)buffer.size(), true,
					FILE_NOTIFY_CHANGE_LAST_WRITE, &bytesReturned, NULL, NULL);

				if (succ == false) { 
					if (m_Running) { pgError("IO Error while trying to watch directory" << m_Directory); } 
					continue; 
				}

				FILE_NOTIFY_INFORMATION* info = (FILE_NOTIFY_INFORMATION*)&buffer[0];
				do {
					std::wstring tmpString(info->FileName, info->FileNameLength / sizeof(wchar_t));
					std::string fileName(tmpString.begin(), tmpString.end());
					std::replace(fileName.begin(), fileName.end(), '\\', '/');

					pgDebug("File Changed: " << fileName);

					info = (FILE_NOTIFY_INFORMATION*)((BYTE*)info + info->NextEntryOffset);
				} while (info->NextEntryOffset != 0);
			}

			pgDebug("Stopped watching " << m_Directory);
		}
	}
}