# calc

> A small virtual machine for executing `.bcalc` bytecode.

`calc` executes binary bytecode files produced by [calcc](https://github.com/Inramag/calcc).

## Features

- Binary `.bcalc` bytecode execution
- Floating-point values
- Arithmetic operations
- Variables
- String output
- Optional newline flag for `print`
- Low memory usage
- ASCII-compatible bytecode data

## Usage

```text
calc <path/to/file.bcalc>
```

Example:

```bash
calc program.bcalc
```

## Bytecode

The VM expects `.bcalc` files with the following structure:

```text
bcalc
instructions
\0
variables
```

The file starts with the `bcalc` magic followed by the instruction stream. The instruction stream ends at the null byte separator.

Variable names are stored after the separator. Each variable consists of:

```text
[length: uint8_t][ASCII name]
```

The VM uses variable indices stored in instructions to access their values.

## Building

The VM uses CMake.

```bash
cmake -B build -G Ninja
cmake --build build
```

## License

MIT
