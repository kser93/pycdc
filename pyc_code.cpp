#include "pyc_code.h"
#include "pyc_module.h"
#include "data.h"

void PycCode::load(PycData* stream, PycModule* mod)
{
    if (mod->verCompare(1, 3) >= 0 && mod->verCompare(2, 3) < 0)
        m_argCount = stream->get<std::int16_t>();
    else if (mod->verCompare(2, 3) >= 0)
        m_argCount = stream->get<std::int32_t>();

    if (mod->verCompare(3, 8) >= 0)
        m_posOnlyArgCount = stream->get<std::int32_t>();
    else
        m_posOnlyArgCount = 0;

    if (mod->majorVer() >= 3)
        m_kwOnlyArgCount = stream->get<std::int32_t>();
    else
        m_kwOnlyArgCount = 0;

    if (mod->verCompare(1, 3) >= 0 && mod->verCompare(2, 3) < 0)
        m_numLocals = stream->get<std::int16_t>();
    else if (mod->verCompare(2, 3) >= 0)
        m_numLocals = stream->get<std::int32_t>();
    else
        m_numLocals = 0;

    if (mod->verCompare(1, 5) >= 0 && mod->verCompare(2, 3) < 0)
        m_stackSize = stream->get<std::int16_t>();
    else if (mod->verCompare(2, 3) >= 0)
        m_stackSize = stream->get<std::int32_t>();
    else
        m_stackSize = 0;

    if (mod->verCompare(1, 3) >= 0 && mod->verCompare(2, 3) < 0)
        m_flags = stream->get<std::int16_t>();
    else if (mod->verCompare(2, 3) >= 0)
        m_flags = stream->get<std::int32_t>();
    else
        m_flags = 0;

    m_code = LoadObject(stream, mod).require_cast<PycString>();
    m_consts = LoadObject(stream, mod).require_cast<PycSequence>();
    m_names = LoadObject(stream, mod).require_cast<PycSequence>();

    if (mod->verCompare(1, 3) >= 0)
        m_varNames = LoadObject(stream, mod).require_cast<PycSequence>();
    else
        m_varNames = new PycTuple;

    if (mod->verCompare(2, 1) >= 0)
        m_freeVars = LoadObject(stream, mod).require_cast<PycSequence>();
    else
        m_freeVars = new PycTuple;

    if (mod->verCompare(2, 1) >= 0)
        m_cellVars = LoadObject(stream, mod).require_cast<PycSequence>();
    else
        m_cellVars = new PycTuple;

    m_fileName = LoadObject(stream, mod).require_cast<PycString>();
    m_name = LoadObject(stream, mod).require_cast<PycString>();

    if (mod->verCompare(1, 5) >= 0 && mod->verCompare(2, 3) < 0)
        m_firstLine = stream->get<std::int16_t>();
    else if (mod->verCompare(2, 3) >= 0)
        m_firstLine = stream->get<std::int32_t>();

    if (mod->verCompare(1, 5) >= 0)
        m_lnTable = LoadObject(stream, mod).require_cast<PycString>();
    else
        m_lnTable = new PycString;
}
