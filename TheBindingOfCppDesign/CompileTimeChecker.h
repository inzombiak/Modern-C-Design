#ifndef COMPILE_TIME_CHECKER_H
#define COMPILE_TIME_CHECKER_H

#define STATIC_CHECK(expr, msg) \
		{\
			class ERROR_##msg {} ;\
			(void) sizeof((::CompileTimeChecker<(expr) != 0> (ERROR_##msg())));\
		}

template <bool> struct CompileTimeChecker
{
	CompileTimeChecker(...);
};
template<> struct CompileTimeChecker<false> { };

#endif