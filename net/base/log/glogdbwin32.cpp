#include "glogdbwin32.h"

#ifdef Q_OS_WIN

#include <windows.h> // for OutputDebugString

// ----------------------------------------------------------------------------
// GLogDbWin32
// ----------------------------------------------------------------------------
void GLogDbWin32::write(QString& msg) {
	OutputDebugStringA(qPrintable(msg));
}

#endif // Q_OS_WIN
