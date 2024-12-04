#pragma once
#include <utility>
#include <stdio.h>
#include <intrin.h>

#define spoof_func CallSpoofer::SpoofFunction spoof(_AddressOfReturnAddress());
#ifdef _KERNEL_MODE
#define SPOOF_CALL(ret_type,name) (CallSpoofer::SafeCall<ret_type,std::remove_reference_t<decltype(*name)>>(name))
#else
#define spoof_call(name) (CallSpoofer::SafeCall(name))
#endif

#define MAX_FUNC_BUFFERED 100
#define SHELLCODE_GENERATOR_SIZE 500

inline uintptr_t codecave = 0;

namespace CallSpoofer
{
    typedef unsigned __int64  uintptr_t, size_t;

    class SpoofFunction
    {
    public:
        uintptr_t temp = 0;
        const uintptr_t xor_key = 0xff00ff00ff00ff00;
        void* ret_addr_in_stack = 0;

        SpoofFunction(void* addr) :ret_addr_in_stack(addr)
        {
            temp = *(uintptr_t*)ret_addr_in_stack;
            temp ^= xor_key;
            *(uintptr_t*)ret_addr_in_stack = 0;
        }
        ~SpoofFunction()
        {
            temp ^= xor_key;
            *(uintptr_t*)ret_addr_in_stack = temp;
        }
    };

    __forceinline PVOID LocateShellCode(PVOID func, size_t size = SHELLCODE_GENERATOR_SIZE)
    {
        void* addr = (void*)IFH(VirtualAlloc)(NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (!addr)
            return nullptr;
        return lazy(memcpy)(addr, func, size);
    }

    template <typename Func, typename ...Args>
    typename std::invoke_result<Func, Args...>::type
        __declspec(safebuffers)ShellCodeGenerator(Func f, Args&... args)
    {
        using this_func_type = decltype(ShellCodeGenerator<Func, Args&...>);
        using return_type = typename std::invoke_result<Func, Args...>::type;

        const uintptr_t xor_key = 0xff00ff00ff00ff00;
        void* ret_addr_in_stack = _AddressOfReturnAddress();
        uintptr_t temp = *(uintptr_t*)ret_addr_in_stack;
        temp ^= xor_key;
        *(uintptr_t*)ret_addr_in_stack = 0;

        if constexpr (std::is_same<return_type, void>::value)
        {
            f(args...);
            temp ^= xor_key;
            *(uintptr_t*)ret_addr_in_stack = temp;
        }
        else
        {
            return_type&& ret = f(args...);
            temp ^= xor_key;
            *(uintptr_t*)ret_addr_in_stack = temp;
            return ret;
        }
    }

    template<class Func >
    class SafeCall
    {

        Func* funcPtr;

    public:
        SafeCall(Func* func) :funcPtr(func) {}


        template<typename... Args>
        __forceinline decltype(auto) operator()(Args&&... args)
        {
            spoof_func;

            using return_type = typename std::invoke_result<Func, Args...>::type;
            using p_shell_code_generator_type = decltype(&ShellCodeGenerator<Func*, Args...>);
            p_shell_code_generator_type self_addr = static_cast<p_shell_code_generator_type>(&ShellCodeGenerator<Func*, Args&&...>);

            p_shell_code_generator_type p_shellcode{};

            static size_t count{};
            static p_shell_code_generator_type orig_generator[MAX_FUNC_BUFFERED]{};
            static p_shell_code_generator_type alloc_generator[MAX_FUNC_BUFFERED]{};

            unsigned index{};
            while (orig_generator[index])
            {
                if (orig_generator[index] == self_addr)
                {
                    p_shellcode = alloc_generator[index];
                    break;
                }
                index++;
            }

            if (!p_shellcode)
            {
                p_shellcode = reinterpret_cast<p_shell_code_generator_type>(LocateShellCode(self_addr));
                orig_generator[count] = self_addr;
                alloc_generator[count] = p_shellcode;
                count++;
            }

            return p_shellcode(funcPtr, args...);
        }
    };
}