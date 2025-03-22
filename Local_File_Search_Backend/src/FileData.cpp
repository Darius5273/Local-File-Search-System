#include "../include/FileData.h"
#include <windows.h>
#include <rpc.h>

#pragma comment(lib, "Rpcrt4.lib")

FileData::FileData(const std::string& name, const std::string& path,
                           const std::string& extension, const std::string& mime_type,
                           const std::string& modified_time, const std::string& created_at,
                           long long size, bool is_text)
        : name(name), path(path), extension(extension), mime_type(mime_type),
          modified_time(modified_time), created_at(created_at), size(size), is_text(is_text) {

    UUID uuid;
    UuidCreate(&uuid);
    RPC_CSTR uuidStr;

    UuidToStringA(&uuid, &uuidStr);
    id = reinterpret_cast<char*>(uuidStr);
    RpcStringFreeA(&uuidStr);
}
