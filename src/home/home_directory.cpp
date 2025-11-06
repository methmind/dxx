//
// Created by sexey on 26.10.2025.
//

#include "home_directory.h"

#include <fstream>
#include "debug/debug_output.h"

namespace home
{
    std::filesystem::path C_HomeDirectory::GetHomeDirectory()
    {
        const char* userProfilePath = std::getenv("USERPROFILE");
        if (!userProfilePath) {
            dbg("std::genenv got:err = Unable to get %USERPROFILE%");
            userProfilePath = "C:/Program Data"; // todo fix it...
        }

        return userProfilePath;
    }

    bool C_HomeDirectory::createFile(const std::string_view& name, const std::vector<uint8_t>& data) const
    {
        const auto filePath = this->path_ / name;
        std::ofstream file(filePath, std::ios::out | std::ios::trunc | std::ios::binary);
        if (file.is_open()) {
            dbg("Unable to open file for writing %s", filePath.c_str());
            return false;
        }

        file << data.data();
        return true;
    }

    std::vector<uint8_t> C_HomeDirectory::readFile(const std::string_view& name) const
    {
        const auto filePath = this->path_ / name;
        std::basic_ifstream<uint8_t> file(filePath, std::ios::binary);
        if (file.is_open()) {
            dbg("Unable to open file %s", filePath.c_str());
            return {};
        }

        file.seekg(0, std::ios::end);
        const auto size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<uint8_t> buffer(size);
        if (!file.read(buffer.data(), size)) {
            dbg("Unable to read file %s", filePath.c_str());
            return {};
        }

        return std::move(buffer);
    }
} // home