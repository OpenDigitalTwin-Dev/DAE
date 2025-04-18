
#include "chrono/serialization/ChOutputASCII.h"

namespace chrono {

ChOutputASCII::ChOutputASCII(std::ostream& stream_out) : m_ostream(stream_out) {
    tablevel = 0;
    use_versions = false;
    suppress_names = false;
}
ChOutputASCII::~ChOutputASCII() {}

void ChOutputASCII::SetSuppressNames(bool msu) {
    suppress_names = msu;
}

std::ostream& ChOutputASCII::GetStream() {
    return m_ostream;
}

void ChOutputASCII::indent() {
    for (int i = 0; i < tablevel; ++i)
        m_ostream << "\t";
}
void ChOutputASCII::out(ChNameValue<unsigned int> bVal) {
    indent();
    if (!suppress_names)
        m_ostream << bVal.name() << "\t";
    m_ostream << bVal.value();
    m_ostream << "\n";
}
void ChOutputASCII::out(ChNameValue<char> bVal) {
    indent();
    m_ostream << bVal.name();
    m_ostream << "\t";
    m_ostream << bVal.value();
    m_ostream << "\n";
}
void ChOutputASCII::out(ChNameValue<float> bVal) {
    indent();
    if (!suppress_names)
        m_ostream << bVal.name() << "\t";
    m_ostream << bVal.value();
    m_ostream << "\n";
}
void ChOutputASCII::out(ChNameValue<double> bVal) {
    indent();
    if (!suppress_names)
        m_ostream << bVal.name() << "\t";
    m_ostream << bVal.value();
    m_ostream << "\n";
}
void ChOutputASCII::out(ChNameValue<int> bVal) {
    indent();
    if (!suppress_names)
        m_ostream << bVal.name() << "\t";
    m_ostream << bVal.value();
    m_ostream << "\n";
}
void ChOutputASCII::out(ChNameValue<bool> bVal) {
    indent();
    if (!suppress_names)
        m_ostream << bVal.name() << "\t";
    m_ostream << bVal.value();
    m_ostream << "\n";
}
void ChOutputASCII::out(ChNameValue<std::string> bVal) {
    indent();
    if (!suppress_names)
        m_ostream << bVal.name() << "\t";
    m_ostream << "\"";
    m_ostream << bVal.value();
    m_ostream << "\"\n";
}
void ChOutputASCII::out(ChNameValue<unsigned long> bVal) {
    indent();
    if (!suppress_names)
        m_ostream << bVal.name() << "\t";
    m_ostream << bVal.value();
    m_ostream << "\n";
}
void ChOutputASCII::out(ChNameValue<unsigned long long> bVal) {
    indent();
    if (!suppress_names)
        m_ostream << bVal.name() << "\t";
    m_ostream << bVal.value();
    m_ostream << "\n";
}
void ChOutputASCII::out(ChNameValue<ChEnumMapperBase> bVal) {
    indent();
    if (!suppress_names)
        m_ostream << bVal.name() << "\t";
    m_ostream << "\"";
    std::string mstr = bVal.value().GetValueAsString();
    m_ostream << mstr;
    m_ostream << "\"\n";
}
void ChOutputASCII::out_array_pre(ChValue& bVal, size_t msize) {
    indent();
    if (!suppress_names) {
        m_ostream << bVal.name() << "  ";
    }
    m_ostream << "container of " << msize << " items, [" << bVal.GetTypeidName() << "]\n";
    ++tablevel;
    indent();
    m_ostream << "[ \n";
    ++tablevel;
}
void ChOutputASCII::out_array_between(ChValue& bVal, size_t msize) {}
void ChOutputASCII::out_array_end(ChValue& bVal, size_t msize) {
    --tablevel;
    indent();
    m_ostream << "] \n";
    --tablevel;
}

// for custom c++ objects:

void ChOutputASCII::out(ChValue& bVal, bool tracked, size_t obj_ID) {
    indent();
    if (!suppress_names)
        m_ostream << bVal.name() << "  ";
    m_ostream << "[" << bVal.GetTypeidName() << "]";
    if (tracked)
        m_ostream << " (tracked)   ID= " << obj_ID;
    if (this->use_versions)
        m_ostream << " version=" << bVal.GetClassRegisteredVersion();
    m_ostream << " \n";
    ++tablevel;
    bVal.CallArchiveOut(*this);
    --tablevel;
}
void ChOutputASCII::out_ref(ChValue& bVal, bool already_inserted, size_t obj_ID, size_t ext_ID) {
    const char* classname = bVal.GetClassRegisteredName().c_str();
    indent();
    if (!suppress_names)
        m_ostream << bVal.name();
    m_ostream << "->";
    if (strlen(classname) > 0) {
        m_ostream << " [" << classname << "] (registered type)";
    } else {
        m_ostream << " [" << bVal.GetTypeidName() << "]";
    }
    if (obj_ID)
        m_ostream << "  ID=" << obj_ID;
    if (ext_ID)
        m_ostream << "  external_ID=" << ext_ID;
    if (this->use_versions)
        m_ostream << " version=" << bVal.GetClassRegisteredVersion();
    m_ostream << "\n";
    ++tablevel;
    if (!already_inserted) {
        if (!bVal.IsNull()) {
            // New Object, we have to full serialize it
            bVal.CallArchiveOut(*this);
        } else {
            m_ostream << "NULL\n";
        }
    }
    --tablevel;
}

}  // end namespace chrono
