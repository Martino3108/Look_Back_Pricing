# Building and Distributing LookBack Dynamic Library (macOS)

This document describes how to **build**, **install**, and **distribute** the `libLookBack.dylib`
dynamic library on macOS, with special attention to **Microsoft Excel sandboxing** and
**distribution (codesigning + notarization)**.

---

## 1. Requirements

### System
- macOS 13+
- Xcode Command Line Tools
- clang++

### Dependencies
- Homebrew
- OpenMP (`libomp`)

Install OpenMP via Homebrew:
```bash
brew install libomp
```

## 2. Building the Dynamic Library

The library is built as a macOS .dylib using clang++ in C++20 mode, with optimizations enabled
and OpenMP support.

### Build command

```bash
clang++ -std=c++20 -O3 -fPIC -dynamiclib \
  Look_Back.cpp Date_Dealing.cpp LookBackDll.cpp \
  -Xpreprocessor -fopenmp \
  -I"$(brew --prefix libomp)/include" \
  -L"$(brew --prefix libomp)/lib" \
  -Wl,-rpath,"$(brew --prefix libomp)/lib" \
  -lomp \
  -Wl,-install_name,@rpath/libLookBack.dylib \
  -o libLookBack.dylib
```

### Notes
- -Xpreprocessor -fopenmp enables OpenMP for clang.
- -I and -L point to Homebrew’s OpenMP headers and libraries.
- -Wl,-rpath,... embeds the runtime search path for libomp.
- -Wl,-install_name,@rpath/libLookBack.dylib sets the dynamic loader install name.

## 3. Excel Sandbox Installation (Manual)

Microsoft Excel on macOS runs sandboxed. The library must be placed inside Excel’s container:

```bash
~/Library/Containers/com.microsoft.Excel/Data/
```

Open the folder with:

```bash
open ~/Library/Containers/com.microsoft.Excel/Data/
```

Copy the library:

```bash
cp libLookBack.dylib ~/Library/Containers/com.microsoft.Excel/Data/
```

## 4. OpenMP Runtime Fix (Critical)

Excel cannot reliably resolve Homebrew paths at runtime.
To make the library self-contained, libomp.dylib must be copied next to libLookBack.dylib
and the dependency rewritten to use @loader_path.

### Step 1: Copy OpenMp runtime

```bash
cp "$(brew --prefix libomp)/lib/libomp.dylib" \
   ~/Library/Containers/com.microsoft.Excel/Data/
```

### Step 2: Rewrite dependency

```bash
install_name_tool -change \
  /opt/homebrew/opt/libomp/lib/libomp.dylib \
  @loader_path/libomp.dylib \
  ~/Library/Containers/com.microsoft.Excel/Data/libLookBack.dylib
```

(For Intel Macs, also try /usr/local/opt/libomp/lib/libomp.dylib.)

### Step 3: Verify

```bash
otool -L ~/Library/Containers/com.microsoft.Excel/Data/libLookBack.dylib
```

Expected output must include:

@loader_path/libomp.dylib
