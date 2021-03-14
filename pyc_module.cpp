#include "pyc_module.h"
#include "data.h"
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>

enum struct PycMagic: std::uint32_t {
    MAGIC_1_0 = 0x00999902,
    MAGIC_1_1 = 0x00999903, /* Also covers 1.2 */
    MAGIC_1_3 = 0x0A0D2E89,
    MAGIC_1_4 = 0x0A0D1704,
    MAGIC_1_5 = 0x0A0D4E99,
    MAGIC_1_6 = 0x0A0DC4FC,
    MAGIC_1_6_U = 0x0A0DC4FD,

    MAGIC_2_0 = 0x0A0DC687,
    MAGIC_2_0_U = 0x0A0DC688,
    MAGIC_2_1 = 0x0A0DEB2A,
    MAGIC_2_1_U = 0x0A0DEB2B,
    MAGIC_2_2 = 0x0A0DED2D,
    MAGIC_2_2_U = 0x0A0DED2E,
    MAGIC_2_3 = 0x0A0DF23B,
    MAGIC_2_3_U = 0x0A0DF23C,
    MAGIC_2_4 = 0x0A0DF26D,
    MAGIC_2_4_U = 0x0A0DF26E,
    MAGIC_2_5 = 0x0A0DF2B3,
    MAGIC_2_5_U = 0x0A0DF2B4,
    MAGIC_2_6 = 0x0A0DF2D1,
    MAGIC_2_6_U = 0x0A0DF2D2,
    MAGIC_2_7 = 0x0A0DF303,
    MAGIC_2_7_U = 0x0A0DF304,

    MAGIC_3_0 = 0x0A0D0C3A,
    MAGIC_3_0_U = 0x0A0D0C3B,
    MAGIC_3_1 = 0x0A0D0C4E,
    MAGIC_3_1_U = 0x0A0D0C4F,
    MAGIC_3_2 = 0x0A0D0C6C,
    MAGIC_3_3 = 0x0A0D0C9E,
    MAGIC_3_4 = 0x0A0D0CEE,
    MAGIC_3_5 = 0x0A0D0D16,
    MAGIC_3_5_3 = 0x0A0D0D17,
    MAGIC_3_6 = 0x0A0D0D33,
    MAGIC_3_7 = 0x0A0D0D42,
    MAGIC_3_8 = 0x0A0D0D55,
    MAGIC_3_9 = 0x0A0D0D61,
};

PycModule::PycModule(std::filesystem::path filepath) : m_maj(-1), m_min(-1), m_unicode(false)
{
    PycFile in(filepath.string().c_str());
    if (!in.isOpen()) {
        std::ostringstream errmsg;
        errmsg << "Error opening file " << filepath;
        throw std::invalid_argument(errmsg.str());
    }
    auto magic{ in.get32() };
    setVersion(magic);

    auto flags{ 0 };
    if (verCompare(3, 7) >= 0) {
        flags = in.get32();
    }

    if (flags & 0x1) {
        // Optional checksum added in Python 3.7
        in.get32();
        in.get32();
    }
    else {
        in.get32(); // Timestamp -- who cares?

        if (verCompare(3, 3) >= 0)
            in.get32(); // Size parameter added in Python 3.3
    }

    m_code = LoadObject(&in, this).require_cast<PycCode>();
}

void PycModule::setVersion(unsigned int magic)
{
    /* Starting with 1.6, Python adds +1 for unicode mode (-U)
       3.0 and above are always unicode
       3.2 stops using the unicode increment
    */
    switch (PycMagic(magic)) {
    case PycMagic::MAGIC_1_0:
        setVersionHelper(1, 0, false);
        break;
    case PycMagic::MAGIC_1_1:
        setVersionHelper(1, 1, false);
        break;
    case PycMagic::MAGIC_1_3:
        setVersionHelper(1, 3, false);
        break;
    case PycMagic::MAGIC_1_4:
        setVersionHelper(1, 4, false);
        break;
    case PycMagic::MAGIC_1_5:
        setVersionHelper(1, 5, false);
        break;
    case PycMagic::MAGIC_1_6:
        setVersionHelper(1, 6, false);
        break;
    case PycMagic::MAGIC_1_6_U:
        setVersionHelper(1, 6, true);
        break;
    case PycMagic::MAGIC_2_0:
        setVersionHelper(2, 0, false);
        break;
    case PycMagic::MAGIC_2_0_U:
        setVersionHelper(2, 0, true);
        break;
    case PycMagic::MAGIC_2_1:
        setVersionHelper(2, 1, false);
        break;
    case PycMagic::MAGIC_2_1_U:
        setVersionHelper(2, 1, true);
        break;
    case PycMagic::MAGIC_2_2:
        setVersionHelper(2, 2, false);
        break;
    case PycMagic::MAGIC_2_2_U:
        setVersionHelper(2, 2, true);
        break;
    case PycMagic::MAGIC_2_3:
        setVersionHelper(2, 3, false);
        break;
    case PycMagic::MAGIC_2_3_U:
        setVersionHelper(2, 3, true);
        break;
    case PycMagic::MAGIC_2_4:
        setVersionHelper(2, 4, false);
        break;
    case PycMagic::MAGIC_2_4_U:
        setVersionHelper(2, 4, true);
        break;
    case PycMagic::MAGIC_2_5:
        setVersionHelper(2, 5, false);
        break;
    case PycMagic::MAGIC_2_5_U:
        setVersionHelper(2, 5, true);
        break;
    case PycMagic::MAGIC_2_6:
        setVersionHelper(2, 6, false);
        break;
    case PycMagic::MAGIC_2_6_U:
        setVersionHelper(2, 6, true);
        break;
    case PycMagic::MAGIC_2_7:
        setVersionHelper(2, 7, false);
        break;
    case PycMagic::MAGIC_2_7_U:
        setVersionHelper(2, 7, true);
        break;
    case PycMagic::MAGIC_3_0_U:
        setVersionHelper(3, 0, true);
        break;
    case PycMagic::MAGIC_3_1_U:
        setVersionHelper(3, 1, true);
        break;
    case PycMagic::MAGIC_3_2:
        setVersionHelper(3, 2, true);
        break;
    case PycMagic::MAGIC_3_3:
        setVersionHelper(3, 3, true);
        break;
    case PycMagic::MAGIC_3_4:
        setVersionHelper(3, 4, true);
        break;
    case PycMagic::MAGIC_3_5:
        setVersionHelper(3, 5, true);
        break;
    case PycMagic::MAGIC_3_5_3:
        setVersionHelper(3, 5, true);
        break;
    case PycMagic::MAGIC_3_6:
        setVersionHelper(3, 6, true);
        break;
    case PycMagic::MAGIC_3_7:
        setVersionHelper(3, 7, true);
        break;
    case PycMagic::MAGIC_3_8:
        setVersionHelper(3, 8, true);
        break;
    case PycMagic::MAGIC_3_9:
        setVersionHelper(3, 9, true);
        break;
    default:
        std::ostringstream errmsg;
        errmsg << "Bad MAGIC " << std::hex << magic;
        throw std::invalid_argument(errmsg.str());
    }
}

void PycModule::setVersionHelper(int major, int minor, bool has_unicode)
{
    m_maj = major;
    m_min = minor;
    m_unicode = has_unicode;
}

std::string PycModule::getVersionString() const
{
    std::ostringstream ss;
    ss << majorVer() << '.' << minorVer();
    if (majorVer() < 3 && isUnicode()) {
        ss << " -U";
    }
    return ss.str();
}

int PycModule::verCompare(int major, int minor) const
{
    if (m_maj == major)
        return m_min - minor;
    return m_maj - major;
}

PycRef<PycString> PycModule::getIntern(int ref) const
{
    if (ref < 0)
        throw std::out_of_range("Intern index out of range");

    auto it = m_interns.cbegin();
    while (ref-- && it != m_interns.cend())
        ++it;
    if (it == m_interns.cend())
        throw std::out_of_range("Intern index out of range");
    return *it;
}

PycRef<PycObject> PycModule::getRef(int ref) const
{
    if (ref < 0)
        throw std::out_of_range("Ref index out of range");

    auto it = m_refs.cbegin();
    while (ref-- && it != m_refs.cend())
        ++it;
    if (it == m_refs.cend())
        throw std::out_of_range("Ref index out of range");
    return *it;
}
