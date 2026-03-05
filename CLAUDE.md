# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a C project (`myapp`) that demonstrates logging with [log4c](https://github.com/rcmadruga/log4c) and automatic fallback to stderr/stdout when log4c is unavailable. The project uses ExternalProject to build all dependencies from source, ensuring reproducible builds across different systems.

## Build Commands

### Standard Build
```bash
./build.sh
# Or manually:
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build && cmake --build build
```

### Run the Executable
```bash
build/output/bin/myapp
```

### Clean Build
```bash
rm -rf build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build && cmake --build build
```

### Using CMake Presets
```bash
# Linux (default)
cmake --preset linux
cmake --build build

# macOS
cmake --preset macOS
cmake --build build

# Coverage builds
cmake --preset coverage-gcc      # GCC coverage
cmake --preset coverage-llvm      # Clang/AppleClang LLVM native coverage
```

## Code Formatting

The project uses `.clang-format` with specific settings (4-space indent, 140 column limit, left pointer alignment). Run formatting before committing:

```bash
clang-format -i src/*.c include/*.h
```

## Git Commit Conventions

This project uses [commitizen](https://commitizen-tools.github.io/commitizen/) with conventional commits. Instead of `git commit`, use:

```bash
cz bump   # Bump version and update CHANGELOG.md
```

Or follow the conventional commit format manually: `type(scope): description`

Types: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

### Allowed Author Emails

Pre-commit hooks verify commits use one of these emails:
- `luke_guangzhong@petalmail.com`
- `luke_guangzhong@qq.com`
- `luke.guangzhong@gmail.com`
- `2115170017@stu.hytc.edu.cn`
- `boogie.board.user@gmail.com`

Set your email for this repo:
```bash
git config user.email "your-email@example.com"
```

## Architecture

### Dependency Management

All dependencies are built via CMake ExternalProject from Gitee mirrors:

| Dependency | Version | Purpose |
|-----------|---------|---------|
| log4c | master | Logging framework (optional, falls back to printf) |
| expat | R_1_95_8 | XML parsing (required by log4c, old version for API compatibility) |
| trower-base64 | local/1.2.0 | Base64 encoding/decoding |
| cJSON | v1.7.8 | JSON parsing |

Dependencies install to `build/lib/` and `build/include/`, not system paths.

### Logging Architecture

The logging module (`src/log.c`, `include/log.h`) provides:

1. **Dual-mode operation**: Tries log4c first, falls back to simple stdout/stderr logging
2. **Category-based logging**: Use string category names like `"main"`, `"network"`, `"database"`
3. **Log levels**: DEBUG, INFO, WARN, ERROR, FATAL
4. **Runtime registration**: Categories are registered on first use via `get_category_ctx()`

Key functions:
- `log_init()` - Initialize logging (call once at startup)
- `log_shutdown()` - Cleanup
- `log_debug(category, format, ...)` - Macros for each level

The `LOG4C_RCPATH` compile flag points to `resource/log4crc` for log4c configuration.

### Source Layout

- `src/main.c` - Application entry point
- `src/log.c` - Logging implementation with fallback
- `include/log.h` - Logging interface
- `include/common.h` - Common types (binary_data_t) and macros (FREE, FCLOSE, IS_NULL)
- `resource/log4crc` - Log4c category/priority configuration
- `cmake/` - CMake modules for ExternalProject setup

## Coverage Testing

### LLVM Native Coverage (Clang/AppleClang only)
```bash
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DENABLE_LLVM_COVERAGE=ON -S . -B build
cmake --build build
LLVM_PROFILE_FILE="default.profraw" build/output/bin/myapp
llvm-profdata merge -sparse default.profraw -o coverage.profdata
mkdir -p coverage
llvm-cov show build/output/bin/myapp -instr-profile=coverage.profdata -format=html -output-dir=coverage/coverage-html
```

### GCC Coverage
```bash
cmake -DENABLE_COVERAGE_TEST=ON -S . -B build
cmake --build build
build/output/bin/myapp
gcov build/CMakeFiles/myapp.dir/src/*.gcda
```

## Common Gotchas

- **LOG4C_RCPATH must be defined**: The main.c enforces this at compile time. It's set via CMake compile definition.
- **Expat version matters**: log4c 1.2.4 requires expat 1.95.x (not 2.x). The build forces use of expat 1.95.8 to avoid API incompatibility.
- **Categories are lazily registered**: First use of a log category registers it with log4c. Max 64 categories.
- **ExternalProject builds are slow**: Dependencies build once, then cached. Use `UPDATE_COMMAND ""` to prevent re-fetching.
