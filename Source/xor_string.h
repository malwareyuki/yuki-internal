#pragma once

/*____________________________________________________________________________________________________________

Original Author: skadro
Github: https://github.com/skadro-official
License: See end of file

skCrypter
		Compile-time, Usermode + Kernelmode, safe and lightweight string crypter library for C++11+

							*Not removing this part is appreciated*
____________________________________________________________________________________________________________*/

#ifdef _KERNEL_MODE
namespace std
{
	// STRUCT TEMPLATE remove_reference
	template <class _Ty>
	struct remove_reference {
		using type = _Ty;
	};

	template <class _Ty>
	struct remove_reference<_Ty&> {
		using type = _Ty;
	};

	template <class _Ty>
	struct remove_reference<_Ty&&> {
		using type = _Ty;
	};

	template <class _Ty>
	using remove_reference_t = typename remove_reference<_Ty>::type;

	// STRUCT TEMPLATE remove_const
	template <class _Ty>
	struct remove_const { // remove top-level const qualifier
		using type = _Ty;
	};

	template <class _Ty>
	struct remove_const<const _Ty> {
		using type = _Ty;
	};

	template <class _Ty>
	using remove_const_t = typename remove_const<_Ty>::type;
}
#else
#include <type_traits>
#endif

namespace skc
{
	template<class _Ty>
	using clean_type = typename std::remove_const_t<std::remove_reference_t<_Ty>>;

	template <int _size, char _key1, char _key2, typename T>
	class skCrypter
	{
	public:
		__forceinline constexpr skCrypter(T* data)
		{
			crypt(data);
		}

		__forceinline T* get()
		{
			return _storage;
		}

		__forceinline int size() // (w)char count
		{
			return _size;
		}

		__forceinline  char key()
		{
			return _key1;
		}

		__forceinline  T* encrypt()
		{
			if (!isEncrypted())
				crypt(_storage);

			return _storage;
		}

		__forceinline  T* decrypt()
		{
			if (isEncrypted())
				crypt(_storage);

			return _storage;
		}

		__forceinline bool isEncrypted()
		{
			return _storage[_size - 1] != 0;
		}

		__forceinline void clear() // set full storage to 0
		{
			for (int i = 0; i < _size; i++)
			{
				_storage[i] = 0;
			}
		}

		__forceinline operator T* ()
		{
			decrypt();

			return _storage;
		}

	private:
		__forceinline constexpr void crypt(T* data)
		{
			for (int i = 0; i < _size; i++)
			{
				_storage[i] = data[i] ^ (_key1 + i % (1 + _key2));
			}
		}

		T _storage[_size]{};
	};
}

#define EncryptString(chararr) std::string(Encrypt(chararr)).c_str()
#define Encrypt(str) skCrypt_key(str, __TIME__[7] + 1, __TIME__[4] + 9)
#define skCrypt_key(str, key1, key2) []() { \
			constexpr static auto crypted = skc::skCrypter \
				<sizeof(str) / sizeof(str[0]), key1, key2, skc::clean_type<decltype(str[0])>>((skc::clean_type<decltype(str[0])>*)str); \
					return crypted; }()


#define DecryptOffset(value) [&](){ auto val = value; int retval = strtol(val.decrypt(), NULL, 16); val.encrypt(); return retval; }()
#define DecryptOffsetClear(value) [&](){ auto val = value; int retval = strtol(val.decrypt(), NULL, 16); val.clear(); return retval; }()
#define EncryptGet(value) [&](){ auto val = Encrypt(value); std::string return_value = val.decrypt(); val.encrypt(); return return_value; }()
#define EncryptGetX(value) [&](){ auto val = Encrypt(value); const char* return_value = val.decrypt(); val.encrypt(); return return_value; }()
#define EncryptGetL(value) [&](){ auto val = Encrypt(value); return val.decrypt(); }()


template <int X> struct EnsureCompileTime {
	enum : int {
		Value = X
	};
};
#define Seedx ((__TIME__[7] - '0') * 1  + (__TIME__[6] - '0') * 10  + \
	(__TIME__[4] - '0') * 60 + (__TIME__[3] - '0') * 600 + \
	(__TIME__[1] - '0') * 3600 + (__TIME__[0] - '0') * 36000)

__forceinline constexpr int LinearCongruentGenerator(int Rounds) {
	return 2013904223 + 2664525 * ((Rounds > 0) ? LinearCongruentGenerator(Rounds - 1) : Seedx & 0xFFFFFFFF);
}
#define Random() EnsureCompileTime<LinearCongruentGenerator(15)>::Value 
#define RandomNumber(Min, Max) (Min + (Random() % (Max - Min + 1)))
template <int... Pack> struct IndexList {};
template <typename IndexList, int Right> struct Append;
template <int... Left, int Right> struct Append<IndexList<Left...>, Right> {
	typedef IndexList<Left..., Right> Result;
};
template <int N> struct ConstructIndexList {
	typedef typename Append<typename ConstructIndexList<N - 1>::Result, N - 1>::Result Result;
};
template <> struct ConstructIndexList<0> {
	typedef IndexList<> Result;
};
const char XORKEY_A = static_cast<char>(0x51);
const wchar_t XORKEY_W = static_cast<wchar_t>(0x331);
__declspec(noinline)  constexpr char EncryptCharacterA(const char Character, int Index) {
	return Character ^ (XORKEY_A + Index);
}
template <typename IndexList> class CingA;
template <int... Index> class CingA<IndexList<Index...> > {
private:
	char Value[sizeof...(Index) + 1];
public:
	__forceinline constexpr CingA(const char* const String)
		: Value{ EncryptCharacterA(String[Index], Index)... } {}

	__forceinline char* decrypt() {
		for (int t = 0; t < sizeof...(Index); t++) {
			Value[t] = Value[t] ^ (XORKEY_A + t);
		}
		Value[sizeof...(Index)] = '\0';
		return Value;
	}

	__forceinline char* get() {
		return Value;
	}
};
__declspec(noinline) constexpr wchar_t EncryptCharacterW(const wchar_t Character, int Index) {
	return Character ^ (XORKEY_W + Index);
}
template <typename IndexList> class CingW;
template <int... Index> class CingW<IndexList<Index...> > {
private:
	wchar_t Value[sizeof...(Index) + 1];
public:
	__forceinline constexpr CingW(const wchar_t* const String)
		: Value{ EncryptCharacterW(String[Index], Index)... } {}

	__forceinline wchar_t* decrypt() {
		for (int t = 0; t < sizeof...(Index); t++) {
			Value[t] = Value[t] ^ (XORKEY_W + t);
		}
		Value[sizeof...(Index)] = '\0\0';
		return Value;
	}

	__forceinline wchar_t* get() {
		return Value;
	}
};

#define xorstr_( String ) ( CingA<ConstructIndexList<sizeof( String ) - 1>::Result>( String ).decrypt() )  
#define wxorstr_( String ) ( CingW<ConstructIndexList<sizeof( String ) - 1>::Result>( String ).decrypt() )  