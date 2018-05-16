#ifndef WINHANDLE_H
#define WINHANDLE_H

#include <Rad/WinError.h>
#include <memory>

namespace rad
{
    inline void CheckCloseHandle(HANDLE Handle)
    {
        if (Handle != NULL && Handle != INVALID_HANDLE_VALUE)
        {
            if (!::CloseHandle(Handle))
                rad::ThrowWinError();
        }
    }

    class WinHandle : private std::unique_ptr<std::remove_pointer<HANDLE>::type, void (*)(HANDLE)>
    {
    public:
        WinHandle(HANDLE Handle = INVALID_HANDLE_VALUE)
            : std::unique_ptr<std::remove_pointer<HANDLE>::type, void(*)(HANDLE)>(Handle, CheckCloseHandle)
        {
        }

        void Close()
        {
            release();
        }

        HANDLE Get() const
        {
            return get();
        }

        void Attach(HANDLE Handle = INVALID_HANDLE_VALUE)
        {
            operator=(Handle);
        }

        HANDLE Release()
        {
            return release();
        }

        explicit operator bool() const _NOEXCEPT
        {   // test for non-null pointer
            return (get() != NULL && get() != INVALID_HANDLE_VALUE);
        }
    };
}

#endif
