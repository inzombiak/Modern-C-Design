#ifndef TYPE_INFO_H
#define TYPE_INFO_H

#include <assert.h>
#include <typeinfo>
#include <ostream>

#include "../Chapter5_Functors/Functors.h"
/*
std::type_info wrapper from Chapter 2.8
*/

class TypeInfo
{
public:

	TypeInfo();
	TypeInfo(const std::type_info&);

	const std::type_info& Get() const;
	bool before(const TypeInfo& rhs) const;
	const char* name() const;

private:
	const std::type_info* m_info;
};

inline TypeInfo::TypeInfo()
{
	class Nil {};
	m_info = &typeid(Nil);
	assert(m_info);
}

inline TypeInfo::TypeInfo(const std::type_info& ti)
	: m_info(&ti)
{
	assert(m_info);
}

inline bool TypeInfo::before(const TypeInfo& rhs) const
{
	assert(m_info);
	return m_info->before(*rhs.m_info) != 0;
}

inline const std::type_info& TypeInfo::Get() const
{
	assert(m_info);
	return *m_info;
}

inline const char* TypeInfo::name() const
{
	assert(m_info);
	return m_info->name();
}

inline bool operator==(const TypeInfo& lhs, const TypeInfo& rhs)
// type_info::operator== return type is int in some VC libraries
{
	return (lhs.Get() == rhs.Get()) != 0;
}

inline bool operator<(const TypeInfo& lhs, const TypeInfo& rhs)
{
	return lhs.before(rhs);
}

inline bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs)
{
	return !(lhs == rhs);
}

inline bool operator>(const TypeInfo& lhs, const TypeInfo& rhs)
{
	return rhs < lhs;
}

inline bool operator<=(const TypeInfo& lhs, const TypeInfo& rhs)
{
	return !(lhs > rhs);
}

inline bool operator>=(const TypeInfo& lhs, const TypeInfo& rhs)
{
	return !(lhs < rhs);
}

//Used for the error logging
std::ostream& operator<<(std::ostream& os, const TypeInfo& info)
{
	os << info.name();
	return os;
}

#endif