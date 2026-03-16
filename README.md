# peinjector

A simple educational tool for injecting shellcode into Windows PE (Portable Executable) files using [LIEF](https://lief.re/).

> **For educational purposes only.**

## What it does

1. Parses a target PE (`.exe`) file.
2. Reads a raw shellcode binary.
3. Adds a new `.code` section to the PE containing the shellcode followed by a `JMP` back to the original entry point.
4. Sets the new section as the entry point.
5. Writes the modified PE to an output file.

## Requirements

- C++17 compiler (MSVC, GCC, or Clang)
- [LIEF](https://lief.re/) library

## Usage

```
injector.exe <input.exe> <output.exe> <shellcode.bin>
```

| Argument        | Description                        |
|-----------------|------------------------------------|
| `input.exe`     | The original PE file to modify     |
| `output.exe`    | Path for the modified PE output    |
| `shellcode.bin` | Raw binary shellcode to inject     |

## How it works

- The shellcode is placed into a new section (`.code`) with `MEM_EXECUTE`, `MEM_READ`, and `CNT_CODE` characteristics.
- A relative `JMP` (`0xE9`) instruction is appended after the shellcode to redirect execution back to the original entry point once the shellcode finishes.
- The PE entry point is updated to point to the start of the new section.
