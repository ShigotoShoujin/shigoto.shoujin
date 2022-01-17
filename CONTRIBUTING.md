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
01. public / protected / private
02. internal types
03. constructors and assignment operators
04. destructor
05. events variables
06. virtual overrides
07. getters
08. setters
09. virtual functions
10. functions

### Class Accessors
No "get" prefix, `const` and prefer `[[nodiscard]]`  
`[[nodiscard]] Point position() const { return position; }`

### Line Length
Definitely much more than 80 characters. 160 seems nice.
