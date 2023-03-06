//
// Generated file, do not edit! Created by opp_msgtool 6.0 from request.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "request_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace fattree {

Register_Class(Request)

Request::Request(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

Request::Request(const Request& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

Request::~Request()
{
}

Request& Request::operator=(const Request& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void Request::copy(const Request& other)
{
    this->work_type = other.work_type;
    this->port_index = other.port_index;
    this->data_size = other.data_size;
    this->src_addr = other.src_addr;
    this->des_addr = other.des_addr;
    this->next_hop_addr = other.next_hop_addr;
    this->generate_time = other.generate_time;
    this->arriveModule_time = other.arriveModule_time;
    this->leaveModule_time = other.leaveModule_time;
}

void Request::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->work_type);
    doParsimPacking(b,this->port_index);
    doParsimPacking(b,this->data_size);
    doParsimPacking(b,this->src_addr);
    doParsimPacking(b,this->des_addr);
    doParsimPacking(b,this->next_hop_addr);
    doParsimPacking(b,this->generate_time);
    doParsimPacking(b,this->arriveModule_time);
    doParsimPacking(b,this->leaveModule_time);
}

void Request::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->work_type);
    doParsimUnpacking(b,this->port_index);
    doParsimUnpacking(b,this->data_size);
    doParsimUnpacking(b,this->src_addr);
    doParsimUnpacking(b,this->des_addr);
    doParsimUnpacking(b,this->next_hop_addr);
    doParsimUnpacking(b,this->generate_time);
    doParsimUnpacking(b,this->arriveModule_time);
    doParsimUnpacking(b,this->leaveModule_time);
}

char Request::getWork_type() const
{
    return this->work_type;
}

void Request::setWork_type(char work_type)
{
    this->work_type = work_type;
}

short Request::getPort_index() const
{
    return this->port_index;
}

void Request::setPort_index(short port_index)
{
    this->port_index = port_index;
}

int64_t Request::getData_size() const
{
    return this->data_size;
}

void Request::setData_size(int64_t data_size)
{
    this->data_size = data_size;
}

const char * Request::getSrc_addr() const
{
    return this->src_addr.c_str();
}

void Request::setSrc_addr(const char * src_addr)
{
    this->src_addr = src_addr;
}

const char * Request::getDes_addr() const
{
    return this->des_addr.c_str();
}

void Request::setDes_addr(const char * des_addr)
{
    this->des_addr = des_addr;
}

const char * Request::getNext_hop_addr() const
{
    return this->next_hop_addr.c_str();
}

void Request::setNext_hop_addr(const char * next_hop_addr)
{
    this->next_hop_addr = next_hop_addr;
}

::omnetpp::simtime_t Request::getGenerate_time() const
{
    return this->generate_time;
}

void Request::setGenerate_time(::omnetpp::simtime_t generate_time)
{
    this->generate_time = generate_time;
}

::omnetpp::simtime_t Request::getArriveModule_time() const
{
    return this->arriveModule_time;
}

void Request::setArriveModule_time(::omnetpp::simtime_t arriveModule_time)
{
    this->arriveModule_time = arriveModule_time;
}

::omnetpp::simtime_t Request::getLeaveModule_time() const
{
    return this->leaveModule_time;
}

void Request::setLeaveModule_time(::omnetpp::simtime_t leaveModule_time)
{
    this->leaveModule_time = leaveModule_time;
}

class RequestDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_work_type,
        FIELD_port_index,
        FIELD_data_size,
        FIELD_src_addr,
        FIELD_des_addr,
        FIELD_next_hop_addr,
        FIELD_generate_time,
        FIELD_arriveModule_time,
        FIELD_leaveModule_time,
    };
  public:
    RequestDescriptor();
    virtual ~RequestDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(RequestDescriptor)

RequestDescriptor::RequestDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(fattree::Request)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

RequestDescriptor::~RequestDescriptor()
{
    delete[] propertyNames;
}

bool RequestDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Request *>(obj)!=nullptr;
}

const char **RequestDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *RequestDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int RequestDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 9+base->getFieldCount() : 9;
}

unsigned int RequestDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_work_type
        FD_ISEDITABLE,    // FIELD_port_index
        FD_ISEDITABLE,    // FIELD_data_size
        FD_ISEDITABLE,    // FIELD_src_addr
        FD_ISEDITABLE,    // FIELD_des_addr
        FD_ISEDITABLE,    // FIELD_next_hop_addr
        FD_ISEDITABLE,    // FIELD_generate_time
        FD_ISEDITABLE,    // FIELD_arriveModule_time
        FD_ISEDITABLE,    // FIELD_leaveModule_time
    };
    return (field >= 0 && field < 9) ? fieldTypeFlags[field] : 0;
}

const char *RequestDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "work_type",
        "port_index",
        "data_size",
        "src_addr",
        "des_addr",
        "next_hop_addr",
        "generate_time",
        "arriveModule_time",
        "leaveModule_time",
    };
    return (field >= 0 && field < 9) ? fieldNames[field] : nullptr;
}

int RequestDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "work_type") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "port_index") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "data_size") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "src_addr") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "des_addr") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "next_hop_addr") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "generate_time") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "arriveModule_time") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "leaveModule_time") == 0) return baseIndex + 8;
    return base ? base->findField(fieldName) : -1;
}

const char *RequestDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "char",    // FIELD_work_type
        "short",    // FIELD_port_index
        "int64_t",    // FIELD_data_size
        "string",    // FIELD_src_addr
        "string",    // FIELD_des_addr
        "string",    // FIELD_next_hop_addr
        "omnetpp::simtime_t",    // FIELD_generate_time
        "omnetpp::simtime_t",    // FIELD_arriveModule_time
        "omnetpp::simtime_t",    // FIELD_leaveModule_time
    };
    return (field >= 0 && field < 9) ? fieldTypeStrings[field] : nullptr;
}

const char **RequestDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *RequestDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int RequestDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    Request *pp = omnetpp::fromAnyPtr<Request>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void RequestDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    Request *pp = omnetpp::fromAnyPtr<Request>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'Request'", field);
    }
}

const char *RequestDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    Request *pp = omnetpp::fromAnyPtr<Request>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RequestDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    Request *pp = omnetpp::fromAnyPtr<Request>(object); (void)pp;
    switch (field) {
        case FIELD_work_type: return long2string(pp->getWork_type());
        case FIELD_port_index: return long2string(pp->getPort_index());
        case FIELD_data_size: return int642string(pp->getData_size());
        case FIELD_src_addr: return oppstring2string(pp->getSrc_addr());
        case FIELD_des_addr: return oppstring2string(pp->getDes_addr());
        case FIELD_next_hop_addr: return oppstring2string(pp->getNext_hop_addr());
        case FIELD_generate_time: return simtime2string(pp->getGenerate_time());
        case FIELD_arriveModule_time: return simtime2string(pp->getArriveModule_time());
        case FIELD_leaveModule_time: return simtime2string(pp->getLeaveModule_time());
        default: return "";
    }
}

void RequestDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    Request *pp = omnetpp::fromAnyPtr<Request>(object); (void)pp;
    switch (field) {
        case FIELD_work_type: pp->setWork_type(string2long(value)); break;
        case FIELD_port_index: pp->setPort_index(string2long(value)); break;
        case FIELD_data_size: pp->setData_size(string2int64(value)); break;
        case FIELD_src_addr: pp->setSrc_addr((value)); break;
        case FIELD_des_addr: pp->setDes_addr((value)); break;
        case FIELD_next_hop_addr: pp->setNext_hop_addr((value)); break;
        case FIELD_generate_time: pp->setGenerate_time(string2simtime(value)); break;
        case FIELD_arriveModule_time: pp->setArriveModule_time(string2simtime(value)); break;
        case FIELD_leaveModule_time: pp->setLeaveModule_time(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Request'", field);
    }
}

omnetpp::cValue RequestDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    Request *pp = omnetpp::fromAnyPtr<Request>(object); (void)pp;
    switch (field) {
        case FIELD_work_type: return pp->getWork_type();
        case FIELD_port_index: return pp->getPort_index();
        case FIELD_data_size: return pp->getData_size();
        case FIELD_src_addr: return pp->getSrc_addr();
        case FIELD_des_addr: return pp->getDes_addr();
        case FIELD_next_hop_addr: return pp->getNext_hop_addr();
        case FIELD_generate_time: return pp->getGenerate_time().dbl();
        case FIELD_arriveModule_time: return pp->getArriveModule_time().dbl();
        case FIELD_leaveModule_time: return pp->getLeaveModule_time().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'Request' as cValue -- field index out of range?", field);
    }
}

void RequestDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    Request *pp = omnetpp::fromAnyPtr<Request>(object); (void)pp;
    switch (field) {
        case FIELD_work_type: pp->setWork_type(omnetpp::checked_int_cast<char>(value.intValue())); break;
        case FIELD_port_index: pp->setPort_index(omnetpp::checked_int_cast<short>(value.intValue())); break;
        case FIELD_data_size: pp->setData_size(omnetpp::checked_int_cast<int64_t>(value.intValue())); break;
        case FIELD_src_addr: pp->setSrc_addr(value.stringValue()); break;
        case FIELD_des_addr: pp->setDes_addr(value.stringValue()); break;
        case FIELD_next_hop_addr: pp->setNext_hop_addr(value.stringValue()); break;
        case FIELD_generate_time: pp->setGenerate_time(value.doubleValue()); break;
        case FIELD_arriveModule_time: pp->setArriveModule_time(value.doubleValue()); break;
        case FIELD_leaveModule_time: pp->setLeaveModule_time(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Request'", field);
    }
}

const char *RequestDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr RequestDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    Request *pp = omnetpp::fromAnyPtr<Request>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void RequestDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    Request *pp = omnetpp::fromAnyPtr<Request>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Request'", field);
    }
}

}  // namespace fattree

namespace omnetpp {

}  // namespace omnetpp

