#include "org_libdivecomputer_IOStream.h"
#include "exception.h"

#include <libdivecomputer/iostream.h>

JNIEXPORT void JNICALL Java_org_libdivecomputer_IOStream_Close
  (JNIEnv *env, jobject obj, jlong handle)
{
	DC_EXCEPTION_THROW(dc_iostream_close ((dc_iostream_t *) handle));
}
