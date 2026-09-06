#include "Version.hh"
#include <sstream>

std::string star::Version::GetVersion()
{
	return "in development";
}

std::string star::Version::GetCompilerNameAndVersion()
{
    std::stringstream ss;
#if defined(_MSC_VER)
    uint32_t major = _MSC_FULL_VER / 10'000'000;
    uint32_t minor = (_MSC_FULL_VER % 10'000'000) / 100'000;
    uint32_t build = (_MSC_FULL_VER % 100'000);
    ss << "Compiler: MSVC (cl) " << major << "." << minor << "." << build;
#elif defined(__clang__)
    ss << "Compiler: clang++ " << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__;
#elif defined(__GNUC__)
    ss << "Compiler: g++ "
        << __GNUC__ << "."
        << __GNUC_MINOR__ << "."
        << __GNUC_PATCHLEVEL__;
#else
    ss << "Compiler: Unknown / Generic Compiler\n";
#endif
    std::string compilerName = ss.str();
    return compilerName;
}