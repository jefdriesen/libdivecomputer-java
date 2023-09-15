#include "org_libdivecomputer_Serial.h"

#include <libdivecomputer/serial.h>


JNIEXPORT jlong JNICALL Java_org_libdivecomputer_Serial_Open
  (JNIEnv *env, jobject, jlong context, jstring devname)
{
	dc_status_t status = DC_STATUS_SUCCESS;
	dc_iostream_t *iostream = NULL;

	const char *str = (*env)->GetStringUTFChars(env, devname, NULL);

	status = dc_serial_open (&iostream,
		(dc_context_t *) context,
		str);

	(*env)->ReleaseStringUTFChars(env, devname, str);

	if (status != DC_STATUS_SUCCESS) {
		return 0;
	}

	return (jlong) iostream;
}
