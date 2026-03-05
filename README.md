# myapp

A C project demonstrating robust logging with log4c and automatic fallback support. This project showcases a clean CMake-based build system with ExternalProject dependencies, code coverage testing, and development workflow automation.

## Features

- **Dual-mode logging**: Automatically uses log4c when available, falls back to stdout/stderr logging
- **Category-based logging**: Organize logs by component (main, network, database, ui, etc.)
- **Self-contained dependencies**: All external libraries built via CMake ExternalProject
- **Code coverage**: Support for both GCC and LLVM native coverage modes
- **Pre-commit hooks**: Git author email validation
- **Conventional commits**: Commitizen integration for consistent commit messages

## Quick Start

### Prerequisites

- CMake >= 3.10
- C compiler (GCC, Clang, or AppleClang)
- Git

### Build

```bash
# Clone and build
git clone <repository-url>
cd blank
./build.sh

# Or manually:
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build
cmake --build build
```

### Run

```bash
./build/output/bin/myapp
```

Output:
```
[main] hello world!
[network] connecting to server
[database] connection slow
[ui] failed to load resource
```

## Project Structure

```
blank/
├── CMakeLists.txt              # Main build configuration
├── CMakePresets.json           # CMake presets for different platforms
├── build.sh                    # Quick build script
├── src/
│   ├── main.c                  # Application entry point
│   └── log.c                   # Logging implementation
├── include/
│   ├── common.h                # Common types and utility macros
│   └── log.h                   # Logging interface
├── resource/
│   └── log4crc                 # Log4c configuration
└── cmake/
    ├── FindLOG4C.cmake         # System log4c finder
    ├── Log4cExternalProject.cmake    # Build log4c from source
    └── ExpatExternalProject.cmake    # Build expat from source
```

## Dependencies

All dependencies are automatically built from source via CMake ExternalProject:

| Dependency | Version | Purpose |
|-----------|---------|---------|
| log4c | master | Logging framework (optional) |
| expat | R_1_95_8 | XML parsing for log4c (old version for API compatibility) |
| trower-base64 | local/1.2.0 | Base64 encoding/decoding |
| cJSON | v1.7.8 | JSON parsing |

**Note**: expat 1.95.8 is used instead of 2.x because log4c 1.2.4 is incompatible with expat 2.x APIs.

## Logging Module

The logging module ([`src/log.c`](src/log.c), [`include/log.h`](include/log.h)) provides:

### Usage

```c
#include "log.h"

int main(void) {
    log_init();

    log_debug("category_name", "debug message: %d", 42);
    log_info("network", "connecting to %s", host);
    log_warn("database", "connection slow: %ld ms", latency);
    log_error("ui", "failed to load: %s", resource);
    log_fatal("system", "critical error occurred");

    log_shutdown();
    return 0;
}
```

### Features

- **Automatic fallback**: If log4c fails to initialize, falls back to simple console logging
- **Lazy registration**: Categories are automatically registered on first use
- **Configurable**: Edit [`resource/log4crc`](resource/log4crc) to adjust log levels and appenders

### Log Levels

| Level | Description |
|-------|-------------|
| `LOG_LEVEL_DEBUG` | Detailed debugging information |
| `LOG_LEVEL_INFO` | General informational messages |
| `LOG_LEVEL_WARN` | Warning messages |
| `LOG_LEVEL_ERROR` | Error conditions |
| `LOG_LEVEL_FATAL` | Critical/fatal errors |

## Code Coverage

### LLVM Native Coverage (Clang/AppleClang)

```bash
cmake -DENABLE_LLVM_COVERAGE=ON -S . -B build
cmake --build build

LLVM_PROFILE_FILE="default.profraw" ./build/output/bin/myapp
llvm-profdata merge -sparse default.profraw -o coverage.profdata
llvm-cov show ./build/output/bin/my \
    -instr-profile=coverage.profdata \
    -format=html \
    -output-dir=coverage/coverage-html
```

View report: `open coverage/coverage-html/index.html`

### GCC Coverage

```bash
cmake -DENABLE_COVERAGE_TEST=ON -S . -B build
cmake --build build
./build/output/bin/myapp
gcov build/CMakeFiles/myapp.dir/src/*.gcda
```

### Using CMake Presets

```bash
cmake --preset coverage-llvm   # Clang/AppleClang
cmake --preset coverage-gcc    # GCC
cmake --build build
```

## Development

### Code Formatting

The project uses [`.clang-format`](.clang-format) with 4-space indentation and 140-column limit:

```bash
clang-format -i src/*.c include/*.h
```

### Git Commit Conventions

This project uses [commitizen](https://commitizen-tools.github.io/commitizen/) for conventional commits:

```bash
# Bump version and update CHANGELOG
cz bump

# Or follow conventional commit format manually
git commit -m "feat: add new feature"
git commit -m "fix: resolve memory leak in logging"
git commit -m "docs: update README with usage examples"
```

Commit types: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

### Pre-commit Hooks

The project uses [pre-commit](https://pre-commit.com/) to validate git author email.

Setup:
```bash
pip install pre-commit
pre-commit install
```

## Build Details

### Output Locations

- **Libraries**: `build/lib/`
- **Headers**: `build/include/`
- **Executable**: `build/output/bin/myapp`

### CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `CMAKE_EXPORT_COMPILE_COMMANDS` | ON | Generate compile_commands.json for LSP |
| `ENABLE_COVERAGE_TEST` | OFF | Enable code coverage |
| `ENABLE_LLVM_COVERAGE` | OFF | Use LLVM native coverage (Clang only) |

