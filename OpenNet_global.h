#ifndef OPENNET_GLOBAL_H
#define OPENNET_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OPENNET_LIBRARY)
#  define OPENNET_EXPORT Q_DECL_EXPORT
#else
#  define OPENNET_EXPORT Q_DECL_IMPORT
#endif

#endif // OPENNET_GLOBAL_H
