#if !defined(BUNGEEGUM_VERSION)
#error Undefined bungeegum version. Refer to CMake files to find the appropriate value to define.
#endif

#include <bungeegum/glue/cmake.hpp>
#include <bungeegum/glue/console.hpp>
#include <bungeegum/glue/git.hpp>
#include <bungeegum/glue/tool.hpp>

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
            detail::console_log("found (version = " + _cmake_version.value() + ")\n");
        }
    }

    std::filesystem::path define_parent_directory(int argc, char* argv[])
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

    std::vector<std::string> draw_options()
    {
        std::vector<std::string> _options = {};
        _options.push_back("-DBUNGEEGUM_IN_TREE_HEADERS=ON");
        _options.push_back("-DBUNGEEGUM_BUILD_INSTALLER=OFF");
        if (detail::draw_question("Build widgets library?")) {
            _options.push_back("-DBUNGEEGUM_BUILD_WIDGETS=ON");
        } else {
            _options.push_back("-DBUNGEEGUM_BUILD_WIDGETS=OFF");
        }
        if (detail::draw_question("Build demo application?")) {
            _options.push_back("-DBUNGEEGUM_BUILD_DEMO=ON");
        } else {
            _options.push_back("-DBUNGEEGUM_BUILD_DEMO=OFF");
        }
        if (detail::draw_question("Build doxygen documentation?")) {
            _options.push_back("-DBUNGEEGUM_BUILD_DOC=ON");
        } else {
            _options.push_back("-DBUNGEEGUM_BUILD_DOC=OFF");
        }
        if (detail::draw_question("Build test executable?")) {
            _options.push_back("-DBUNGEEGUM_BUILD_TEST=ON");
        } else {
            _options.push_back("-DBUNGEEGUM_BUILD_TEST=OFF");
        }
        return _options;
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

    std::filesystem::path _source_directory = detail::define_parent_directory(argc, argv);

    detail::console_log("\n[process] cloning repository \n");
    // detail::git_clone(bungeegum_github_url.data(), _source_directory, bungeegum_version.data());

    detail::console_log("\n[process] configuring targets \n");
    std::vector<std::string> _options = detail::draw_options();
    detail::cmake_configure(
        _source_directory,
        _source_directory.append("build/"),
        std::nullopt,
        _options);

    detail::console_log("\n[process] building targets \n");
    // build

    return 0;
}