#include "glogstderr.h"

// ----------------------------------------------------------------------------
// GLogStderr
// ----------------------------------------------------------------------------
void GLogStderr::write(QString& msg) {
	fprintf(stderr, "%s", qPrintable(msg));
}
