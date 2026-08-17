# calcc

> A small compiler for a simple calculation scripting language.

calcc translates `.calc` source files into `.bcalc` bytecode for execution by the calcc virtual machine.

## Features

- Simple variable assignments
- Floating-point constants
- Arithmetic operations
- Variables as operands
- String literals
- `print` command
- Optional newline flag for `print`
- Binary `.bcalc` bytecode format
- Line-by-line source processing with low memory usage
- ASCII-only source data

## Syntax

### Assignment

A variable can be assigned either another variable or a numeric constant:

```text
a = 42.5
b = a
c = -10.25
```

### Arithmetic assignment

An assignment can contain one arithmetic operation:

```text
a = b + 10
b = 20 - a
c = 2.5 * 4
d = a / 2
```

Both operands can be variables or constants:

```text
a = b + c
a = b + 10
a = 10 + b
a = 10 + 20
```

Only one operation is allowed in a single assignment.

Supported operators:

```text
+  -  *  /
```

### Variables

Variable names can contain only English letters (`A-Z`, `a-z`) and `_`.

Examples:

```text
value = 10
my_value = value
```

Other characters and non-ASCII characters are not allowed in variable names.

### Print

`print` accepts either a variable or a string literal:

```text
print a
print "Hello"
```

String literals must be enclosed in double quotes.

The optional `nl` flag adds a newline after the output:

```text
print a nl
print "Hello" nl
```

Without `nl`, no newline is added.

### ASCII restriction

The source language is ASCII-only. This restriction also applies to string literals.

For example:

```text
print "Hello"
```

is valid, while non-ASCII characters in a literal are not supported.

## Example

Source:

```text
a = 10.5
b = -2.5
c = a + b

print c nl
print "Hello" nl
```

## Bytecode

calcc produces binary `.bcalc` files.

The bytecode contains:

- `bcalc` magic
- Compiled instructions
- Variable names

Variable names are stored separately from the instruction stream and are intended primarily for VM diagnostics and logging.

## Building

calcc uses CMake.

```bash
cmake -B build -G Ninja
cmake --build build
```

## Usage

```text
calcc <input.calc> [output.bcalc]
```

If the output file is omitted, calcc uses the input filename with its extension replaced by `.bcalc`.

For example:

```bash
calcc program.calc
```

produces:

```text
program.bcalc
```

An explicit output file can also be specified:

```bash
calcc program.calc output.bcalc
```

## License

MIT
