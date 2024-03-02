#include "org_libdivecomputer_IOStream.h"

#include <libdivecomputer/iostream.h>

JNIEXPORT jlong JNICALL Java_org_libdivecomputer_IOStream_Close
  (JNIEnv *env, jobject obj, jlong handle)
{
	dc_iostream_close ((dc_iostream_t *) handle);
}
