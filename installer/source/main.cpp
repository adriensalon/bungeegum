#if !defined(BUNGEEGUM_VERSION)
#error Undefined bungeegum version. Refer to CMake files to find the appropriate value to define.
#endif

#include <bungeegum/glue/cmake.hpp>
#include <bungeegum/glue/console.hpp>
#include <bungeegum/glue/doxygen.hpp>
#include <bungeegum/glue/git.hpp>
#include <bungeegum/glue/tool.hpp>
#include <bungeegum/glue/toolchain.hpp>

constexpr std::string_view bungeegum_github_url = "https://github.com/adriensalon/bungeegum/";
constexpr std::string_view bungeegum_version = BUNGEEGUM_VERSION;

namespace bungeegum {
namespace detail {

    void draw_banner()
    {
        detail::console_log("\n___.                                                             \n");
        detail::console_log("\\_ |__  __ __  ____    ____   ____   ____   ____  __ __  _____   \n");
        detail::console_log(" | __ \\|  |  \\/    \\  / ___\\_/ __ \\_/ __ \\ / ___\\|  |  \\/     \\  \n");
        detail::console_log(" | \\_\\ \\  |  /   |  \\/ /_/  >  ___/\\  ___// /_/  >  |  /  Y Y  \\ \n");
        detail::console_log(" |___  /____/|___|  /\\___  / \\___  >\\___  >___  /|____/|__|_|  / \n");
        detail::console_log("     \\/           \\//_____/      \\/     \\/_____/             \\/  \n");
        detail::console_log("                                                 " + std::string(bungeegum_version.data()) + "\n");
    }

    void verify_existing_install()
    {
        // std::optional<std::string> _tool_version = tool_version();
        // if (_tool_version.has_value()) {
        //     if (detail::is_tag_newer(_tool_version.value(), bungeegum_version.data())) {

        //     } else {
        //     }
        // }
    }

    void verify_git_version()
    {
        detail::console_log("\n[required] git ");
        std::optional<std::string> _git_version = detail::git_version();
        if (!_git_version.has_value()) {
            throw "ERROR TODO";
        } else {
            detail::console_log("found (version = " + _git_version.value() + ")");
        }
    }

    void verify_cmake_version()
    {
        detail::console_log("\n[required] CMake ");
        std::optional<std::string> _cmake_version = detail::cmake_version();
        if (!_cmake_version.has_value()) {
            throw "ERROR TODO";
        } else {
            detail::console_log("found (version = " + _cmake_version.value() + ")");
        }
    }

    static bool has_doxygen = false;

    void verify_doxygen_version()
    {
        detail::console_log("\n[optional] Doxygen ");
        std::optional<std::string> _doxygen_version = detail::doxygen_version();
        if (!_doxygen_version.has_value()) {
            detail::console_log("not found\n");
            has_doxygen = false;
        } else {
            detail::console_log("found (version = " + _doxygen_version.value() + ")\n");
            has_doxygen = true;
        }
    }

    void create_tree_directories(const std::filesystem::path& directory)
    {
        std::filesystem::path _toolchain_directory = directory / TOOLCHAIN_PLATFORM_NAME;
        std::filesystem::path _debug_directory = _toolchain_directory / "debug/";
        std::filesystem::path _release_directory = _toolchain_directory / "release/";
        std::filesystem::create_directory(directory);
        std::filesystem::create_directory(_toolchain_directory);
        std::filesystem::create_directory(_debug_directory);
        std::filesystem::create_directory(_release_directory);
    }

    void create_directories(
        const std::filesystem::path& root_directory,
        const std::filesystem::path& repos_directory,
        const std::filesystem::path& build_directory,
        const std::filesystem::path& install_directory)
    {
        std::filesystem::path _initial_path = root_directory / "bungeegum/";
        if (std::filesystem::is_directory(_initial_path) && !std::filesystem::is_directory(repos_directory)) {
            std::filesystem::rename(_initial_path, repos_directory);
        }
        create_tree_directories(build_directory);
        create_tree_directories(install_directory);
    }

    std::filesystem::path define_root_directory(int argc, char* argv[])
    {
        // todo
        return "C:/users/adri/desktop/bungeelol/";
    }

    bool draw_question(const std::string& text)
    {
        std::cout << text << " [y/n] : " << std::flush;
        char _result = '_';
        while ((_result != 'y') && (_result != 'n')) {
            std::cin >> _result;
        }
        return _result == 'y';
    }

    static bool should_build_widgets = false;
    static bool should_build_demo = false;
    static bool should_build_doc = false;
    static bool should_build_test = false;

    std::vector<std::pair<std::string, bool>> draw_options()
    {
        std::vector<std::pair<std::string, bool>> _options = {};
        should_build_widgets = detail::draw_question("Build widgets library?");
        if (should_build_widgets) {
            should_build_demo = detail::draw_question("Build demo application?");
            if (has_doxygen) {
                should_build_doc = detail::draw_question("Build doxygen documentation?");
            }
            should_build_test = detail::draw_question("Build test executable?");
        }

        _options.push_back({ "BUNGEEGUM_BUILD_INSTALLER", false });
        _options.push_back({ "BUNGEEGUM_ENABLE_EMBEDDED", true });
        _options.push_back({ "BUNGEEGUM_ENABLE_STANDALONE", true });
        _options.push_back({ "BUNGEEGUM_BUILD_WIDGETS", should_build_widgets });
        _options.push_back({ "BUNGEEGUM_BUILD_DEMO", should_build_demo });
        _options.push_back({ "BUNGEEGUM_BUILD_DOC", should_build_doc });
        _options.push_back({ "BUNGEEGUM_BUILD_TEST", should_build_test });
        return _options;
    }

    void build_targets(const std::filesystem::path& build_directory)
    {
        detail::console_log("\n[process] building bungeegum_runtime library \n");
        detail::cmake_build(build_directory, "bungeegum_runtime");
        // detail::cmake_build(build_directory, "bungeegum_tool");
        if (detail::should_build_widgets) {
            detail::console_log("\n[process] building bungeegum_widgets library \n");
            detail::cmake_build(build_directory, "bungeegum_widgets");
        }
        if (detail::should_build_demo) {
            detail::console_log("\n[process] building bungeegum_demo executable \n");
            detail::cmake_build(build_directory, "bungeegum_demo");
        }
        if (detail::should_build_test) {
            detail::console_log("\n[process] building bungeegum_test executable \n");
            detail::cmake_build(build_directory, "bungeegum_test");
        }
    }
}
}

int main(int argc, char* argv[])
{
    using namespace bungeegum;

    detail::draw_banner();
    detail::verify_existing_install();
    detail::verify_git_version();
    detail::verify_cmake_version();
    detail::verify_doxygen_version();

    detail::console_log("\n[process] cloning repository \n");
    std::filesystem::path _root_directory = detail::define_root_directory(argc, argv);
    detail::git_clone(bungeegum_github_url.data(), _root_directory, bungeegum_version.data());
    std::filesystem::path _repos_directory = _root_directory / "repos/";
    std::filesystem::path _build_directory = _root_directory / "build/";
    std::filesystem::path _install_directory = _root_directory / "install/";
    detail::create_directories(_root_directory, _repos_directory, _build_directory, _install_directory);

    detail::console_log("\n[process] configuring targets \n");
    std::vector<std::pair<std::string, bool>> _configure_options = detail::draw_options();

    detail::console_log("\n[process] configuring debug variant \n");
    std::filesystem::path _debug_build_directory = _build_directory / TOOLCHAIN_PLATFORM_NAME / "debug/";
    std::filesystem::path _debug_install_directory = _install_directory / TOOLCHAIN_PLATFORM_NAME / "debug/";
    detail::cmake_configure(
        _repos_directory,
        _debug_build_directory,
        _debug_install_directory,
        std::nullopt,
        _configure_options, true);

    // detail::console_log("\n[process] configuring release variant \n");
    // std::filesystem::path _release_build_directory = _build_directory / TOOLCHAIN_PLATFORM_NAME / "release/";
    // std::filesystem::path _release_install_directory = _install_directory / TOOLCHAIN_PLATFORM_NAME / "release/";
    // detail::cmake_configure(
    //     _repos_directory,
    //     _release_build_directory,
    //     _release_install_directory,
    //     std::nullopt,
    //     _configure_options, false);

    detail::console_log("\n[process] building debug variant \n");
    detail::build_targets(_debug_build_directory);

    if (detail::should_build_doc) {
        detail::console_log("\n[process] building documentation \n");
        // DOXYBUILD
    }

    return 0;
}