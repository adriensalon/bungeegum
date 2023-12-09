#include <benchmark/benchmark.h>
#include <bungeegum/bungeegum.hpp>

#include <fstream>

static void BM_memcpy(benchmark::State& state) {
  char* src = new char[state.range(0)];
  char* dst = new char[state.range(0)];
  memset(src, 'x', state.range(0));
  for (auto _ : state)
    memcpy(dst, src, state.range(0));
  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(state.range(0)));
  delete[] src;
  delete[] dst;
}
BENCHMARK(BM_memcpy)->Arg(8)->Arg(64)->Arg(512)->Arg(4<<10)->Arg(8<<10);

int main(int argc, char** argv)
{
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
	std::vector<const char*> new_argv(argv, argv + argc);
	new_argv.push_back("--benchmark_out=benchmark_output.json");
	new_argv.push_back("--benchmark_out_format=json");
	new_argv.push_back(nullptr);
	argv = const_cast<char**>(new_argv.data());
	argc = argc + 2;
#endif

    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) {
        return 1;
    };
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

	
	std::ifstream myfile("benchmark_output.json"); // this is equivalent to the above method
	std::string mystring;
	std::string mystring2;
	while ( myfile ) { // always check whether the file is open
		myfile >> mystring2; // pipe file's content into stream
		mystring += mystring2;
	}

	bungeegum::detail::download_file_from_string(mystring, "myfile.txt", "application/txt");
	return 0;
}