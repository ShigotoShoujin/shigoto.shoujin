# Contributing
Everyone's welcome to open issues, submit pull requests or otherwise manifest their existence.

## General
### Commit message
All commit messages should be written in the present.  
Prefer "Remove implicit inline" over "Remove**d** implicit inline"

### C++ Core Guidelines
In doubt refer to the [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)

## Code Style

### Naming
- Types use PascalCase (upper CamelCase)
- Macros use UPPER_SNAKE
- Constants have prefix `k` as in `kDefaultSize`
- Everything use lower camelCase

### Write self-documenting code
The naming of classes, functions and variables should be enough to self-document what the code is doing.  
Prioritize for ease of reading.

### Class declaration ordering
1. public / protected / private
2. constructors and assignment operators
3. destructor
4. events variables
5. virtual overrides
6. getters
7. setters
8. virtual functions
9. functions

### Class Accessors
No "get" prefix, `const` and prefer `[[nodiscard]]`  
`[[nodiscard]] Point position() const { return position; }`

### Line Length
Definitely much more than 80 characters. 160 seems nice.
