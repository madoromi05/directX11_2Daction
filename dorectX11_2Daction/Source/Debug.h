#pragma once
#include <string>
#include <windows.h>

namespace debug
{
    // Unity の Debug.Log に相当するログ出力ユーティリティ。
    // 直接呼ばず、下部のマクロ経由で使用すること。
    // _DEBUG 未定義ではマクロが空実装になり完全に除去される。
    class Debug
    {
    public:
        static void Output(
            const std::string& level,
            const std::string& file,
            int                line,
            const std::string& function,
            const std::string& msg )
        {
            const std::string log =
                file + "(" + std::to_string( line ) + "): "
                + "[" + level + "] "
                + function + " - "
                + msg + "\n";

            OutputDebugStringA( log.c_str() );
        }
    };
}

#ifdef _DEBUG
#define DEBUG_LOG(msg) \
        debug::Debug::Output("LOG",     __FILE__, __LINE__, __FUNCTION__, msg)

#define DEBUG_LOG_WARNING(msg) \
        debug::Debug::Output("WARNING", __FILE__, __LINE__, __FUNCTION__, msg)

// エラー出力後にデバッガを停止する。
// デバッガ未接続の場合は停止しない
#define DEBUG_LOG_ERROR(msg)\
        debug::Debug::Output("ERROR",   __FILE__, __LINE__, __FUNCTION__, msg);
#else
#define DEBUG_LOG(msg)
#define DEBUG_LOG_WARNING(msg)
#define DEBUG_LOG_ERROR(msg)
#endif