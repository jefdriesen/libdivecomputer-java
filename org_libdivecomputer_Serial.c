#include "org_libdivecomputer_Serial.h"
#include "exception.h"

#include <libdivecomputer/serial.h>


JNIEXPORT jlong JNICALL Java_org_libdivecomputer_Serial_Open
  (JNIEnv *env, jobject, jlong context, jstring devname)
{
	dc_iostream_t *iostream = NULL;

	const char *str = (*env)->GetStringUTFChars(env, devname, NULL);

	DC_EXCEPTION_THROW(dc_serial_open (&iostream,
		(dc_context_t *) context,
		str));

	(*env)->ReleaseStringUTFChars(env, devname, str);

	return (jlong) iostream;
}
