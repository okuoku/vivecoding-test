#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <chrono>
#include <getopt.h>

#include "ctransian/public.h"

struct CommandLineOptions {
    std::string input_file;
    std::string output_file;
    std::string header_file;
    bool verbose = false;
    bool debug = false;
    bool validate = true;
    bool show_help = false;
    bool show_version = false;
    
    // Configuration options
    ctransian_config_t config;
    
    CommandLineOptions() {
        // Set default configuration
        ctransian_config_set_defaults(&config);
    }
};

static void printVersion() {
    std::cout << "ctransian " << ctransian_version() << std::endl;
    std::cout << "Advanced WebAssembly to C translation powered by Binaryen" << std::endl;
    std::cout << std::endl;
    std::cout << "Build features:" << std::endl;
    std::cout << "  WASI support: " << (ctransian_has_feature("wasi") ? "Yes" : "No") << std::endl;
    std::cout << "  WasmGC support: " << (ctransian_has_feature("gc") ? "Yes" : "No") << std::endl;
    std::cout << "  SIMD support: " << (ctransian_has_feature("simd") ? "Yes" : "No") << std::endl;
    std::cout << "  Threading support: " << (ctransian_has_feature("threads") ? "Yes" : "No") << std::endl;
}

static void printHelp(const char* program_name) {
    std::cout << "Usage: " << program_name << " [OPTIONS] INPUT.wasm -o OUTPUT.c" << std::endl;
    std::cout << std::endl;
    std::cout << "Advanced WebAssembly to C translation powered by Binaryen." << std::endl;
    std::cout << std::endl;
    std::cout << "Required Options:" << std::endl;
    std::cout << "  -o, --output FILE        Output C source file" << std::endl;
    std::cout << std::endl;
    std::cout << "Optional Options:" << std::endl;
    std::cout << "  -h, --header FILE        Output header file (default: output_file with .h extension)" << std::endl;
    std::cout << "  -O, --optimize-level N   Optimization level (0-3, default: 2)" << std::endl;
    std::cout << "      --enable-simd         Enable SIMD translation with intrinsics" << std::endl;
    std::cout << "      --enable-threads      Enable C11 threading support" << std::endl;
    std::cout << "      --enable-gc           Enable WebAssembly GC types" << std::endl;
    std::cout << "      --enable-wasi         Enable WASI system interface" << std::endl;
    std::cout << "      --bounds-check MODE   Bounds checking: none, relaxed, strict (default: strict)" << std::endl;
    std::cout << "      --runtime TYPE        Runtime: embedded, standalone (default: embedded)" << std::endl;
    std::cout << "      --target-c STD        C standard: c99, c11, gnu11 (default: c11)" << std::endl;
    std::cout << "  -d, --debug              Generate debug information" << std::endl;
    std::cout << "      --no-validate         Skip input validation" << std::endl;
    std::cout << "  -v, --verbose           Verbose output" << std::endl;
    std::cout << "      --version             Show version information" << std::endl;
    std::cout << "  -?, --help               Show this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << program_name << " input.wasm -o output.c" << std::endl;
    std::cout << "  " << program_name << " --optimize-level=3 --enable-simd input.wasm -o output.c" << std::endl;
    std::cout << "  " << program_name << " --enable-threads --bounds-check=relaxed input.wasm -o output.c -h output.h" << std::endl;
    std::cout << std::endl;
    std::cout << "For more information, see: https://github.com/your-org/ctransian" << std::endl;
}

static ctransian_optimize_level_t parseOptimizationLevel(const std::string& level_str) {
    int level = std::stoi(level_str);
    if (level < 0) level = 0;
    if (level > 3) level = 3;
    return static_cast<ctransian_optimize_level_t>(level);
}

static ctransian_bounds_mode_t parseBoundsMode(const std::string& mode_str) {
    if (mode_str == "none") return CTRANIAN_BOUNDS_NONE;
    if (mode_str == "relaxed") return CTRANIAN_BOUNDS_RELAXED;
    if (mode_str == "strict") return CTRANIAN_BOUNDS_STRICT;
    return CTRANIAN_BOUNDS_STRICT; // default
}

static ctransian_runtime_type_t parseRuntimeType(const std::string& type_str) {
    if (type_str == "standalone") return CTRANIAN_RUNTIME_STANDALONE;
    return CTRANIAN_RUNTIME_EMBEDDED; // default
}

static ctransian_c_standard_t parseCStandard(const std::string& std_str) {
    if (std_str == "c99") return CTRANIAN_C99;
    if (std_str == "gnu11") return CTRANIAN_GNU11;
    return CTRANIAN_C11; // default
}

static bool parseCommandLine(int argc, char* argv[], CommandLineOptions& options) {
    static struct option long_options[] = {
        {"output", required_argument, 0, 'o'},
        {"header", required_argument, 0, 'h'},
        {"optimize-level", required_argument, 0, 'O'},
        {"enable-simd", no_argument, 0, 1001},
        {"enable-threads", no_argument, 0, 1002},
        {"enable-gc", no_argument, 0, 1003},
        {"enable-wasi", no_argument, 0, 1004},
        {"bounds-check", required_argument, 0, 1005},
        {"runtime", required_argument, 0, 1006},
        {"target-c", required_argument, 0, 1007},
        {"debug", no_argument, 0, 'd'},
        {"no-validate", no_argument, 0, 1008},
        {"verbose", no_argument, 0, 'v'},
        {"version", no_argument, 0, 1009},
        {"help", no_argument, 0, '?'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c;

    while ((c = getopt_long(argc, argv, "o:h:O:dv?", long_options, &option_index)) != -1) {
        switch (c) {
            case 'o':
                options.output_file = optarg;
                break;
            case 'h':
                options.header_file = optarg;
                break;
            case 'O':
                options.config.optimization_level = parseOptimizationLevel(optarg);
                break;
            case 'd':
                options.debug = true;
                options.config.generate_debug_info = true;
                break;
            case 'v':
                options.verbose = true;
                break;
            case '?':
                options.show_help = true;
                break;
            case 1001:
                options.config.enable_simd = true;
                break;
            case 1002:
                options.config.enable_threads = true;
                break;
            case 1003:
                options.config.enable_gc = true;
                break;
            case 1004:
                options.config.enable_wasi = true;
                break;
            case 1005:
                options.config.bounds_checking = parseBoundsMode(optarg);
                break;
            case 1006:
                options.config.runtime_type = parseRuntimeType(optarg);
                break;
            case 1007:
                options.config.target_c_standard = parseCStandard(optarg);
                break;
            case 1008:
                options.validate = false;
                options.config.validate_input = false;
                break;
            case 1009:
                options.show_version = true;
                break;
            default:
                return false;
        }
    }

    // Get input file
    if (optind < argc) {
        options.input_file = argv[optind];
    }

    return true;
}

static std::vector<uint8_t> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Cannot read file: " + filename);
    }

    return buffer;
}

static void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }
    
    file << content;
    if (!file.good()) {
        throw std::runtime_error("Failed to write file: " + filename);
    }
}

static int runTranslation(const CommandLineOptions& options) {
    if (options.input_file.empty()) {
        std::cerr << "Error: No input file specified" << std::endl;
        return 1;
    }

    if (options.output_file.empty()) {
        std::cerr << "Error: No output file specified (use -o)" << std::endl;
        return 1;
    }

    try {
        // Read input file
        if (options.verbose) {
            std::cout << "Reading input file: " << options.input_file << std::endl;
        }
        auto wasm_data = readFile(options.input_file);

        // Create translation context
        ctransian_context_t* ctx = ctransian_create(&options.config);
        if (!ctx) {
            std::cerr << "Error: Failed to create translation context" << std::endl;
            return 1;
        }

        // Perform translation
        ctransian_result_t result;
        ctransian_result_init(&result);

        auto start_time = std::chrono::high_resolution_clock::now();
        ctransian_error_t error = ctransian_translate_binary(ctx, wasm_data.data(), wasm_data.size(), &result);
        auto end_time = std::chrono::high_resolution_clock::now();

        if (error != CTRANIAN_SUCCESS) {
            std::cerr << "Translation failed: " << ctransian_error_string(error) << std::endl;
            if (result.error_message) {
                std::cerr << "Details: " << result.error_message << std::endl;
            }
            ctransian_result_cleanup(&result);
            ctransian_destroy_context(ctx);
            return 1;
        }

        // Calculate translation time
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        result.translation_time_ms = duration.count();

        // Write output files
        if (options.verbose) {
            std::cout << "Writing output files..." << std::endl;
            std::cout << "  Source: " << options.output_file << std::endl;
            if (!options.header_file.empty()) {
                std::cout << "  Header: " << options.header_file << std::endl;
            }
            std::cout << "Functions translated: " << result.functions_translated << std::endl;
            std::cout << "Instructions translated: " << result.instructions_translated << std::endl;
            std::cout << "Translation time: " << result.translation_time_ms << " ms" << std::endl;
        }

        writeFile(options.output_file, std::string(result.source_code, result.source_size));
        
        if (!options.header_file.empty() && result.header_code) {
            writeFile(options.header_file, std::string(result.header_code, result.header_size));
        }

        // Cleanup
        ctransian_result_cleanup(&result);
        ctransian_destroy_context(ctx);

        if (options.verbose) {
            std::cout << "Translation completed successfully!" << std::endl;
        }

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

int main(int argc, char* argv[]) {
    CommandLineOptions options;
    
    if (!parseCommandLine(argc, argv, options)) {
        printHelp(argv[0]);
        return 1;
    }

    if (options.show_help) {
        printHelp(argv[0]);
        return 0;
    }

    if (options.show_version) {
        printVersion();
        return 0;
    }

    // Generate default header file name if not specified
    if (!options.output_file.empty() && options.header_file.empty()) {
        options.header_file = options.output_file;
        size_t dot_pos = options.header_file.find_last_of('.');
        if (dot_pos != std::string::npos) {
            options.header_file = options.header_file.substr(0, dot_pos);
        }
        options.header_file += ".h";
    }

    return runTranslation(options);
}