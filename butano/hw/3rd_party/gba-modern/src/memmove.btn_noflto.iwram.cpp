//--------------------------------------------------------------------------------
// memmove.c
//--------------------------------------------------------------------------------
// Provides memmove
//--------------------------------------------------------------------------------
#include <string.h>

extern "C"
{
    void* memmove(void* dst, const void* src, size_t cnt)
    {
        auto dst8 = reinterpret_cast<char*>(dst);
        auto src8 = reinterpret_cast<const char*>(src);

        // Call memcpy if the regions only overlap in the "good sense"
        if(dst8 <= src8 || dst8 >= src8 + cnt)
        {
            return memcpy(dst, src, cnt);
        }

        // Do "chunk calls" to memcpy
        auto ofs = size_t(dst8 - src8);
        dst8 += cnt;
        src8 += cnt;

        while(cnt >= ofs)
        {
            dst8 -= ofs;
            src8 -= ofs;
            cnt -= ofs;
            memcpy(dst8, src8, ofs);
        }

        // Copy the last chunk
        memcpy(dst8 - cnt, src8 - cnt, cnt);
        return dst;
    }
}
