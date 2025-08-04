# Contribution Guidelines

Thank you for your interest in contributing! To ensure a consistent and high-quality codebase, please follow the conventions outlined below.

## 1. Code Formatting

We use `clang-format` to enforce a consistent code style.

- **Configuration:** The formatting rules are defined in the `.clang-format` file at the root of this project.
- **How to Use:**
    - Please configure your IDE/editor to use `clang-format` with our project's configuration file.
    - We highly recommend running the formatter on all changed files before committing your code.

## 2. Naming Conventions

We use strict naming conventions. Please make sure all symbol names follow the rules below:

- **Classes, Structs, Enums, Unions, Typedefs, Namespaces:** `CamelCase`
- **Enum Constants (Enumerators):** `CamelCase`
- **Functions and Methods (including free/global functions):** `CamelCase`
- **Macros and Global Constants:** `UPPER_CASE`
- **Function and Template Parameters:** `snake_case_` (trailing underscore)
- **Local Variables, Global Variables:** `snake_case`
- **Private Member Variables:** `m_snake_case` (snake_case prefixed with `m_`)
- **Public Member Variables:** `snake_case`

## 3. Code Quality

All code must compile without warnings under Clang-Tidy and Clang/GCC with `-Wall -Wextra -Wpedantic`. Warnings will be treated as errors.

## 4. Legal

By contributing, you agree that your code may be modified or redistributed under this project's [license](LICENSE).
