#include "glogstdout.h"

// ----------------------------------------------------------------------------
// GLogStdout
// ----------------------------------------------------------------------------
void GLogStdout::write(QString& msg) {
	printf("%s", qPrintable(msg));
	fflush(stdout);
}
