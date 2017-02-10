#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H



template <typename T>
class TypeTraits
{
private:


public:
	static const int isElement = is_element<T>::value;
	static const int isMelee = is_melee<T>::value;
	static const int isProjectile = is_projectile<T>::value;
};

#endif