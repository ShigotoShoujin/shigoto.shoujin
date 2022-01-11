#ifndef SHOUJIN_SOURCE_ENUM_OPERATORS
#define SHOUJIN_SOURCE_ENUM_OPERATORS

#define __SHOUJIN_DEFINE_ENUM_FLAG_OPERATORS_PREFIX__(EnumType, prefix) \
	inline prefix constexpr EnumType operator|(EnumType a, EnumType b) { return static_cast<EnumType>(static_cast<int>(a) | static_cast<int>(b)); } \
	inline prefix EnumType& operator|=(EnumType& a, EnumType b) { return reinterpret_cast<EnumType&>(reinterpret_cast<int&>(a) |= static_cast<int>(b)); } \
	inline prefix constexpr EnumType operator&(EnumType a, EnumType b) { return static_cast<EnumType>(static_cast<int>(a) & static_cast<int>(b)); } \
	inline prefix EnumType& operator&=(EnumType& a, EnumType b) { return reinterpret_cast<EnumType&>(reinterpret_cast<int&>(a) &= static_cast<int>(b)); } \
	inline prefix constexpr EnumType operator~(EnumType a) { return static_cast<EnumType>(~static_cast<int>(a)); } \
	inline prefix constexpr EnumType operator^(EnumType a, EnumType b) { return static_cast<EnumType>(static_cast<int>(a) ^ static_cast<int>(b)); } \
	inline prefix EnumType& operator^=(EnumType& a, EnumType b) { return reinterpret_cast<EnumType&>(reinterpret_cast<int&>(a) ^= static_cast<int>(b)); }

#define SHOUJIN_DEFINE_ENUM_FLAG_OPERATORS(EnumType) \
	__SHOUJIN_DEFINE_ENUM_FLAG_OPERATORS_PREFIX__(EnumType, )

#define SHOUJIN_DEFINE_ENUM_FLAG_OPERATORS_FRIEND(EnumType) \
	__SHOUJIN_DEFINE_ENUM_FLAG_OPERATORS_PREFIX__(EnumType, friend)

#endif
