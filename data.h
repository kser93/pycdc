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
    PycData(std::streambuf& sb);
    PycData(const std::filesystem::path& filepath);
    PycData(const void* buffer, int size);

    bool isOpen() const;
    bool atEof() const;

    int getByte();
    int get16();
    int get32();
    std::int64_t get64();
    int getBuffer(int bytes, void* buffer);

private:
    std::unique_ptr<std::istream> i_stm;
    template<typename T> T get();
};

#endif
