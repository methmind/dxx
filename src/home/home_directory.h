//
// Created by sexey on 26.10.2025.
//

#ifndef HOME_DIRECTORY_H
#define HOME_DIRECTORY_H

#include <filesystem>
#include <vector>

namespace home
{
    class C_HomeDirectory
    {
    private:
        std::filesystem::path path_;

        static std::filesystem::path GetHomeDirectory();

    public:

        [[nodiscard]] const std::filesystem::path& getPath() const { return this->path_; }

        [[nodiscard]] bool isFileExists(const std::string_view& name) const {
            return std::filesystem::exists(this->path_ / name);
        }

        [[nodiscard]] bool createFile(const std::string_view& name, const std::vector<uint8_t>& data) const;

        [[nodiscard]] std::vector<uint8_t> readFile(const std::string_view& name) const;

        [[nodiscard]] bool initialize() const {
            if (std::filesystem::exists(this->path_)) {
                return true;
            }

            return std::filesystem::create_directory(this->path_);
        }

        C_HomeDirectory() : path_(GetHomeDirectory() / "dota++") {}

        ~C_HomeDirectory() = default;
    };
} // home

#endif //HOME_DIRECTORY_H