# How to Contribute to SECSY

Thank you for your interest in contributing! To ensure a consistent and high-quality codebase, please follow the
conventions outlined below.

## 1. Code Formatting

We use `clang-format` to enforce a consistent code style.

- **Configuration:** The formatting rules are defined in the `.clang-format` file at the root of this project.
- **How to Use:**
    - If you are using CLion, the IDE will automatically use this file.
    - If you are using another editor, please configure it to use `clang-format` with our project's configuration file.
    - We highly recommend running the formatter on all changed files before committing your code.

## 2. Naming Conventions

We use a specific naming convention for all C++ symbols. These are enforced in the `.clang-format` file at the root of
this project.

- **Classes, Structs, Enums, Unions, Global Functions:** `UpperCamelCase`
- **Namespaces, Typedefs:** `UpperCamelCase`
- **Class and Struct Methods:** `UpperCamelCase`
- **Global Variables, Local Variables, Union Members:** `all_lower` (snake_case)
- **Parameters:** `_all_lower` (snake_case prefixed with an underscore)
- **Class and Struct Fields (private/protected):** `_m_all_lower` (snake_case prefixed with `_m_`)
- **Class and Struct Public Fields:** `all_lower` (snake_case)
- **Constants (Global/Macro), Template Variables:** `all_lower` (snake_case)
- **Enumerators (Enum Members):** `all_lower` (snake_case)
- **Template Parameters:** `_all_lower` (snake_case prefixed with an underscore)