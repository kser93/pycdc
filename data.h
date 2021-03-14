#ifndef _PYC_FILE_H
#define _PYC_FILE_H
#include <cstdint>
#include <cstdio>
#include <filesystem>

class PycData {
public:
    PycData() { }
    virtual ~PycData() { }

    virtual bool isOpen() const = 0;
    virtual bool atEof() const = 0;

    virtual int getByte() = 0;
    virtual int getBuffer(int bytes, void* buffer) = 0;
    virtual int get16() = 0;
    virtual int get32() = 0;
    virtual std::int64_t get64() = 0;
};

//class PycStream: PycData {
//public:
//    PycData() { }
//    virtual ~PycData() { }
//
//    virtual bool isOpen() const = 0;
//    virtual bool atEof() const = 0;
//
//    virtual int getByte() = 0;
//    virtual int getBuffer(int bytes, void* buffer) = 0;
//    int get16();
//    int get32();
//    std::int64_t get64();
//};


class PycFile : public PycData {
public:
    PycFile(const char* filename);
    PycFile(std::filesystem::path filepath);
    ~PycFile();

    bool isOpen() const override;
    bool atEof() const override;

    int getByte() override;
    int get16() override;
    int get32() override;
    std::int64_t get64() override;
    int getBuffer(int bytes, void* buffer) override;

private:
    FILE* m_stream;
};

class PycBuffer : public PycData {
public:
    PycBuffer(const void* buffer, int size);
    ~PycBuffer() { }

    bool isOpen() const override;
    bool atEof() const override;

    int getByte() override;
    int get16() override;
    int get32() override;
    std::int64_t get64() override;
    int getBuffer(int bytes, void* buffer) override;

private:
    const unsigned char* m_buffer;
    int m_size, m_pos;
};

extern FILE* pyc_output;

#endif
