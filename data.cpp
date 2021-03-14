#include "data.h"
#include <cstring>

FILE* pyc_output = stdout;

/* PycFile */
PycFile::PycFile(const char* filename)
{
    m_stream = fopen(filename, "rb");
}

PycFile::PycFile(std::filesystem::path filepath)
{
    m_stream = fopen(filepath.string().c_str(), "rb");
}

PycFile::~PycFile()
{
    if (m_stream) fclose(m_stream);
}

bool PycFile::isOpen() const
{
    return (m_stream != 0);
}

bool PycFile::atEof() const
{
    int ch = fgetc(m_stream);
    ungetc(ch, m_stream);
    return (ch == EOF);
}

int PycFile::getByte()
{
    int ch = fgetc(m_stream);
    if (ch == EOF)
        ungetc(ch, m_stream);
    return ch;
}

int PycFile::get16()
{
    /* Ensure endianness */
    int result = getByte() & 0xFF;
    result |= (getByte() & 0xFF) << 8;
    return result;
}

int PycFile::get32()
{
    /* Ensure endianness */
    int result = getByte() & 0xFF;
    result |= (getByte() & 0xFF) << 8;
    result |= (getByte() & 0xFF) << 16;
    result |= (getByte() & 0xFF) << 24;
    return result;
}

std::int64_t PycFile::get64()
{
    /* Ensure endianness */
    std::int64_t result = (std::int64_t)(getByte() & 0xFF);
    result |= (std::int64_t)(getByte() & 0xFF) << 8;
    result |= (std::int64_t)(getByte() & 0xFF) << 16;
    result |= (std::int64_t)(getByte() & 0xFF) << 24;
    result |= (std::int64_t)(getByte() & 0xFF) << 32;
    result |= (std::int64_t)(getByte() & 0xFF) << 40;
    result |= (std::int64_t)(getByte() & 0xFF) << 48;
    result |= (std::int64_t)(getByte() & 0xFF) << 56;
    return result;
}

int PycFile::getBuffer(int bytes, void* buffer)
{
    return (int)fread(buffer, 1, bytes, m_stream);
}


/* PycBuffer */

PycBuffer::PycBuffer(const void* buffer, int size)
    : m_buffer((const unsigned char*)buffer), m_size(size), m_pos(0) { }

bool PycBuffer::isOpen() const
{
    return (m_buffer != 0);
}

bool PycBuffer::atEof() const
{
    return (m_pos == m_size);
}

int PycBuffer::getByte()
{
    if (atEof())
        return EOF;
    int ch = (int)(*(m_buffer + m_pos));
    ++m_pos;
    return ch & 0xFF;   // Make sure it's just a byte!
}

int PycBuffer::getBuffer(int bytes, void* buffer)
{
    if (m_pos + bytes > m_size)
        bytes = m_size - m_pos;
    if (bytes != 0)
        memcpy(buffer, (m_buffer + m_pos), bytes);
    return bytes;
}

int PycBuffer::get16()
{
    /* Ensure endianness */
    int result = getByte() & 0xFF;
    result |= (getByte() & 0xFF) << 8;
    return result;
}

int PycBuffer::get32()
{
    /* Ensure endianness */
    int result = getByte() & 0xFF;
    result |= (getByte() & 0xFF) << 8;
    result |= (getByte() & 0xFF) << 16;
    result |= (getByte() & 0xFF) << 24;
    return result;
}

std::int64_t PycBuffer::get64()
{
    /* Ensure endianness */
    std::int64_t result = (std::int64_t)(getByte() & 0xFF);
    result |= (std::int64_t)(getByte() & 0xFF) << 8;
    result |= (std::int64_t)(getByte() & 0xFF) << 16;
    result |= (std::int64_t)(getByte() & 0xFF) << 24;
    result |= (std::int64_t)(getByte() & 0xFF) << 32;
    result |= (std::int64_t)(getByte() & 0xFF) << 40;
    result |= (std::int64_t)(getByte() & 0xFF) << 48;
    result |= (std::int64_t)(getByte() & 0xFF) << 56;
    return result;
}
