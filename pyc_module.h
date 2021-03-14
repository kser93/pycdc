#ifndef _PYC_MODULE_H
#define _PYC_MODULE_H

#include "pyc_code.h"
#include <filesystem>
#include <list>
#include <string>

class PycModule {
public:
    PycModule() : m_maj(-1), m_min(-1), m_unicode(false) { }
    PycModule(std::filesystem::path filepath);

    int majorVer() const { return m_maj; }
    int minorVer() const { return m_min; }
    std::string getVersionString() const;

    int verCompare(int major, int minor) const;

    bool isUnicode() const { return m_unicode; }

    bool strIsUnicode() const
    {
        return (m_maj >= 3) || (m_code->flags() & PycCode::CO_FUTURE_UNICODE_LITERALS) != 0;
    }

    PycRef<PycCode> code() const { return m_code; }

    void intern(PycRef<PycString> str) { m_interns.push_back(str); }
    PycRef<PycString> getIntern(int ref) const;

    void refObject(PycRef<PycObject> str) { m_refs.push_back(str); }
    PycRef<PycObject> getRef(int ref) const;

private:
    void setVersion(unsigned int magic);
    void setVersionHelper(int major, int minor, bool has_unicode);

private:
    int m_maj;
    int m_min;
    bool m_unicode;

    PycRef<PycCode> m_code;
    std::list<PycRef<PycString> > m_interns;
    std::list<PycRef<PycObject> > m_refs;
};

#endif
