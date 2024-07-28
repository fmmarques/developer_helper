#pragma once
#if !defined(DEFINITIONS_HPP)
#    define DEFINITIONS_HPP

#define PURE(retv,func) virtual retv func = 0
#define PURE_ANNOTATED(retv, func, annotations) virtual retv func annotations = 0

#endif
