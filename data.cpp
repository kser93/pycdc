#include "data.h"
#include <cstring>
#include <exception>
#include <fstream>
#include <sstream>

FILE* pyc_output = stdout;

PycData::PycData(const std::filesystem::path &filepath)
{
    i_stm = std::make_unique<std::ifstream>(filepath, std::ios::in | std::ios::binary);
}

PycData::PycData(const void* buffer, int size)
{
    i_stm = std::make_unique<std::istringstream>(
        std::istringstream{
            std::string{ static_cast<const char*>(buffer), static_cast<size_t>(size) }
        }
    );
}

bool PycData::good() const
{
    return i_stm->good();
}

bool PycData::eof() const
{
    i_stm->peek();
    return i_stm->eof();
}

template<typename T> T PycData::get()
{
    T x{ 0 };
    i_stm->read(reinterpret_cast<char*>(&x), sizeof(x));
    if (i_stm->eof()) {
        return EOF;
    }
    else if (i_stm->fail()) {
        std::ostringstream errmsg;
        errmsg << "Cannot read int" << sizeof(T) * 8 << "_t";
        throw std::runtime_error(errmsg.str());
    }
    return x;
}

template std::uint8_t PycData::get();
template std::int16_t PycData::get();
template std::int32_t PycData::get();
template std::int64_t PycData::get();

int PycData::getBuffer(int bytes, void* buffer)
{
    i_stm->read(static_cast<char*>(buffer), bytes);
    return static_cast<int>(i_stm->gcount());
}
