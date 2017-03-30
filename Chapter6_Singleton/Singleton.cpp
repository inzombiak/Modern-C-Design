#include "Singleton.h"

#include <assert.h>

template
<
	class T,
	template <class> class CreationPolicy,
	template <class> class LifetimePolicy
>
T& SingletonHolder<T, CreationPolicy, LifetimePolicy>::Instance()
{
	if (!m_instance)
	{
		//TODO: Thread guard
		if (!m_instance)
		{ 
			if (m_destroyed)
			{
				LifetimePolicy<T>::OnDeadReference();
				m_destroyed = false;
			}

			m_instance = CreationPolicy<T>::Create();
			LifetimePolicy<T>::ScheduleDestruction(&DestorySingleton);
		}
	}

	return m_instance;
}

template
<
	class T,
	template <class> class CreationPolicy,
	template <class> class LifetimePolicy
>
void SingletonHolder<T, CreationPolicy, LifetimePolicy>::DestorySingleton()
{
	assert(m_destroyed);
	CreationPolicy<T>::Destroy(m_instance);
	m_destroyed = true;
}

template
<
	class T,
	template <class> class C,
	template <class> class L
>
T* SingletonHolder<T, C, L>::m_instance = 0;

template
<
	class T,
	template <class> class C,
	template <class> class L
>
bool SingletonHolder<T, C, L>::m_destroyed = false;
