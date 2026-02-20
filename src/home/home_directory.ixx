//
// Created by sexey on 19.02.2026.
//
module;
#include <filesystem>

#include "debug/debug_output.h"

export module home_directory;

namespace home
{
    export class C_HomeDirectory
    {
    public:
        ~C_HomeDirectory() = default;

        C_HomeDirectory() : path_(GetHomeDirectory() / "dota++") {}

        [[nodiscard]] const std::filesystem::path& getPath() const { return this->path_; }

        [[nodiscard]] bool initialize() const
        {
            if (std::filesystem::exists(this->path_)) {
                return true;
            }

            return std::filesystem::create_directory(this->path_);
        }

    private:
        static std::filesystem::path GetHomeDirectory()
        {
            const char* userProfilePath = std::getenv("USERPROFILE");
            if (!userProfilePath) {
                dbg("std::genenv got:err = Unable to get %USERPROFILE%");
                userProfilePath = "C:/ProgramData"; //@note big meme...
            }

            return userProfilePath;
        }

        std::filesystem::path path_;
    };
}
