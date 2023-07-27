#include <process.hpp>

#include <iostream>

bool has_git_installed()
{
    return true;
}

// bool

int main()
{

    using namespace std;
    using namespace TinyProcessLib;

    cout << endl
         << "Example 2 - cd into a nonexistent directory" << endl;
    Process process2(
        " --version", "",
        [](const char* bytes, size_t n) {
            cout << "Output from stdout: " << string(bytes, n);
        },
        [](const char* bytes, size_t n) {
            cout << "Output from stderr: " << string(bytes, n);
            // Add a newline for prettier output on some platforms:
            if (bytes[n - 1] != '\n')
                cout << endl;
        });
    auto exit_status = process2.get_exit_status();
    cout << "Example 2 process returned: " << exit_status << " (" << (exit_status == 0 ? "success" : "failure") << ")" << endl;
    this_thread::sleep_for(chrono::seconds(2));

    return 0;
}