#include "utility.h"
#ifdef _WIN32
  #include <windows.h>
#elif defined(__linux__)
  #include <unistd.h>
  #include <limits.h>
#elif defined(__APPLE__)
  #include <mach-o/dyld.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image readImage(const char* path) 
{
	Image ret;
	ret.data = stbi_load(path, &ret.width, &ret.height, &ret.nrChannels, 0);
	return ret;
}	

std::filesystem::path getExecutableDir()
{
	namespace fs = std::filesystem;

#ifdef _WIN32
	// Windows: GetModuleFileNameW，NULL 表示当前进程
	std::vector<wchar_t> buf(MAX_PATH);
	for (;;) {
		DWORD n = ::GetModuleFileNameW(nullptr, buf.data(),
					static_cast<DWORD>(buf.size()));
		if (n == 0) return fs::current_path();           // 失败兜底
		if (n < buf.size()) break;                        // 成功且没截断
		buf.resize(buf.size() * 2);                       // 被截断，扩容重试
	}
	return fs::path(std::wstring(buf.data())).parent_path();

#elif defined(__linux__)
	// Linux: /proc/self/exe 是当前进程可执行文件的符号链接
	char buf[PATH_MAX];
	ssize_t n = ::readlink("/proc/self/exe", buf, sizeof(buf) - 1);
	if (n <= 0) return fs::current_path();                // 失败兜底
	buf[n] = '\0';
	return fs::path(buf).parent_path();

#elif defined(__APPLE__)
	// macOS: _NSGetExecutablePath 返回的可能不是绝对路径，用 realpath 规整
	uint32_t size = 0;
	_NSGetExecutablePath(nullptr, &size);                 // 先问需要多大
	std::vector<char> buf(size);
	if (_NSGetExecutablePath(buf.data(), &size) != 0)
		return fs::current_path();
	char resolved[PATH_MAX];
	if (!::realpath(buf.data(), resolved))
		return fs::current_path();
	return fs::path(resolved).parent_path();

#else
	return fs::current_path();
#endif
}