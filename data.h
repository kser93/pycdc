#ifndef _PYC_FILE_H
#define _PYC_FILE_H
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <memory>

extern FILE* pyc_output;

class PycData{
public:
    PycData() = delete;
    PycData(const std::filesystem::path& filepath);
    PycData(const void* buffer, int size);

    bool good() const;
    bool eof() const;

    template<typename T> T get();
    int getBuffer(int bytes, void* buffer);

private:
    std::unique_ptr<std::istream> i_stm;
};

#endif
