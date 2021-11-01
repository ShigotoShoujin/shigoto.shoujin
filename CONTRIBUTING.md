# Contributing
Everyone's welcome to open issues, submit pull requests or otherwise manifest their existence.

## Code Style

### Write self-documenting code
The naming of classes, functions and variables should be enough to self-document what the code is doing.  

### Similar to google code style, with some changes

#### Class declaration ordering
Putting private and protected variables first allows for inline accessors and improve understanding by not hiding the privates at the end.

1. private variables
2. protected variables
3. constructors and assignment operators
4. destructor
5. public accessors
6. public functions
7. protected functions
8. private functions

#### Class Data Members
Class Data Members have a prefix _ instead of trailing  
Sorting the class members in alphabetical order will put the private/protected members together.

#### Class Accessors
Same as functions but no "Get" prefix, `[[nodiscard]]`, `inline` and `const`  
They are clearly accessors since they `const` and take no parameters.  
`[[nodiscard]] inline POINT Position() const { return _position; }`

#### Constant Names
ALL_UPPER_CASE

#### Line Length
Definitely much more than 80 characters.
