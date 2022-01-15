// Use GitHub Issues instead of this file

// Reconsider removing the limitation to have only one handler per event,
// even though this makes events handler always return void.

// Change SHOUJIN_ASSERT* to use C++20 std::source_location and get rid of macros

// TODO Enable visual styles
// https://docs.microsoft.com/en-us/windows/win32/controls/cookbook-overview

// TODO Get rid of the requirement to use the TEXT macro
// Instead use L"" and allow for implicit convertion from L"" to "",
// so that in UNICODE no conversion occurs since we give L"" and the functions expects L""
// and in non-unicode our L"" gets automatically converted to ""

// TODO Rewrite Layout / LayoutParam / Window concept so that getters / setters
// exists for all properties and that they work before and after handle creation.
// - Replace SetLayout with GetDefaultLayout/GetDefaultCreateProperties
// - Layout, Min/Max WindowSize and Min/Max ClientSize

// TODO Try to make WindowHandle Parent a WindowHandle* instead of HWND, or remove WindowHandle class to replate with direct HWND

// TODO Add back #ifdef NDEBUG check in assert to remove the extra assertion code in release mode

// TODO Setup auto documentation from code to docs folder.
//      Maybe https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake/
