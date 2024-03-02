#include <stdlib.h>
#include <string.h>

#include <libdivecomputer/custom.h>

#include "org_libdivecomputer_Serial.h"

typedef struct jni_custom_t {
	JNIEnv *env;
	jobject obj;
	jclass cls;
	jmethodID set_timeout;
	jmethodID set_break;
	jmethodID set_dtr;
	jmethodID set_rts;
	jmethodID get_lines;
	jmethodID get_available;
	jmethodID configure;
	jmethodID poll;
	jmethodID read;
	jmethodID write;
	jmethodID ioctl;
	jmethodID flush;
	jmethodID purge;
	jmethodID sleep;
	jmethodID close;
} jni_custom_t;

static dc_status_t
custom_set_timeout (void *userdata, int timeout)
{
	jni_custom_t *jni = userdata;

	(*jni->env)->CallVoidMethod(jni->env, jni->obj,
		jni->set_timeout,
		timeout);

	return DC_STATUS_SUCCESS;
}

static dc_status_t
custom_set_break (void *userdata, unsigned int value)
{
	jni_custom_t *jni = userdata;

	(*jni->env)->CallVoidMethod(jni->env, jni->obj,
		jni->set_break,
		value);

	return DC_STATUS_SUCCESS;
}

static dc_status_t
custom_set_dtr (void *userdata, unsigned int value)
{
	jni_custom_t *jni = userdata;

	(*jni->env)->CallVoidMethod(jni->env, jni->obj,
		jni->set_dtr,
		value);

	return DC_STATUS_SUCCESS;
}

static dc_status_t
custom_set_rts (void *userdata, unsigned int value)
{
	jni_custom_t *jni = userdata;

	(*jni->env)->CallVoidMethod(jni->env, jni->obj,
		jni->set_rts,
		value);

	return DC_STATUS_SUCCESS;
}

static dc_status_t
custom_get_lines (void *userdata, unsigned int *value)
{
	jni_custom_t *jni = userdata;

	*value = (*jni->env)->CallIntMethod(jni->env, jni->obj,
		jni->get_lines);

	return DC_STATUS_SUCCESS;
}

static dc_status_t
custom_get_available (void *userdata, size_t *value)
{
	jni_custom_t *jni = userdata;

	*value = (*jni->env)->CallIntMethod(jni->env, jni->obj,
		jni->get_available);

	return DC_STATUS_SUCCESS;
}

static dc_status_t
custom_configure (void *userdata, unsigned int baudrate, unsigned int databits, dc_parity_t parity, dc_stopbits_t stopbits, dc_flowcontrol_t flowcontrol)
{
	jni_custom_t *jni = userdata;

	(*jni->env)->CallVoidMethod(jni->env, jni->obj,
		jni->configure,
		baudrate, databits, parity, stopbits, flowcontrol);

	return DC_STATUS_SUCCESS;
}

static dc_status_t
custom_poll (void *userdata, int timeout)
{
	jni_custom_t *jni = userdata;

	(*jni->env)->CallVoidMethod(jni->env, jni->obj,
		jni->poll,
		timeout);

	return DC_STATUS_SUCCESS;
}

static dc_status_t
custom_read (void *userdata, void *data, size_t size, size_t *actual)
{
	dc_status_t status = DC_STATUS_SUCCESS;
	jni_custom_t *jni = userdata;

	jobject result = (*jni->env)->CallObjectMethod(jni->env, jni->obj,
		jni->read);
	if (result == NULL) {
		*actual = 0;
		return DC_STATUS_TIMEOUT;
	}

	// Get the length.
	jint len = (*jni->env)->GetArrayLength(jni->env, result);

	// Copy the data.
	if (len > size) {
		// Packet is too large. Copy the first size bytes only.
		(*jni->env)->GetByteArrayRegion(jni->env, result, 0, size, data);
		*actual = size;
		status = DC_STATUS_IO;
	} else {
		(*jni->env)->GetByteArrayRegion(jni->env, result, 0, len, data);
		*actual = len;
		status = DC_STATUS_SUCCESS;
	}

	return status;
}

static dc_status_t
custom_write (void *userdata, const void *data, size_t size, size_t *actual)
{
	jni_custom_t *jni = userdata;

	jbyteArray array = (*jni->env)->NewByteArray(jni->env, size);
	(*jni->env)->SetByteArrayRegion (jni->env, array, 0, size, data);

	(*jni->env)->CallVoidMethod(jni->env, jni->obj,
		jni->write,
		array);

	*actual = size;

	return DC_STATUS_SUCCESS;
}

static dc_status_t
custom_ioctl (void *userdata, unsigned int request, void *data, size_t size)
{
	jni_custom_t *jni = userdata;

	jbyteArray array = (*jni->env)->NewByteArray(jni->env, size);
	(*jni->env)->SetByteArrayRegion (jni->env, array, 0, size, data);

	(*jni->env)->CallVoidMethod(jni->env, jni->obj,
		jni->ioctl,
		request,
		array);

	return DC_STATUS_SUCCESS;
}

static dc_status_t
custom_flush (void *userdata)
{
	jni_custom_t *jni = userdata;

	(*jni->env)->CallVoidMethod(jni->env, jni->obj,
		jni->flush);

	return DC_STATUS_SUCCESS;
}

static dc_status_t
custom_purge (void *userdata, dc_direction_t direction)
{
	jni_custom_t *jni = userdata;

	(*jni->env)->CallVoidMethod(jni->env, jni->obj,
		jni->purge,
		direction);

	return DC_STATUS_SUCCESS;
}

static dc_status_t
custom_sleep (void *userdata, unsigned int milliseconds)
{
	jni_custom_t *jni = userdata;

	(*jni->env)->CallVoidMethod(jni->env, jni->obj,
		jni->sleep,
		milliseconds);

	return DC_STATUS_SUCCESS;
}

static dc_status_t
custom_close (void *userdata)
{
	jni_custom_t *jni = userdata;

	(*jni->env)->CallVoidMethod(jni->env, jni->obj,
		jni->close);

	(*jni->env)->DeleteGlobalRef(jni->env, jni->obj);
	free (jni);

	return DC_STATUS_SUCCESS;
}

JNIEXPORT jlong JNICALL Java_org_libdivecomputer_Custom_Open
  (JNIEnv *env, jobject, jlong context, jint transport, jobject callback)
{
	dc_status_t status = DC_STATUS_SUCCESS;
	dc_iostream_t *iostream = NULL;

	static const dc_custom_cbs_t callbacks = {
		custom_set_timeout, /* set_timeout */
		custom_set_break, /* set_break */
		custom_set_dtr, /* set_dtr */
		custom_set_rts, /* set_rts */
		custom_get_lines, /* get_lines */
		custom_get_available, /* get_available */
		custom_configure, /* configure */
		custom_poll, /* poll */
		custom_read, /* read */
		custom_write, /* write */
		custom_ioctl, /* ioctl */
		custom_flush, /* flush */
		custom_purge, /* purge */
		custom_sleep, /* sleep */
		custom_close, /* close */
	};

	if (callback == NULL) {
		return 0;
	}

	jni_custom_t *jni = malloc (sizeof(jni_custom_t));
	jni->env = env;
	jni->obj = (*env)->NewGlobalRef(env, callback);
	jni->cls = (*env)->GetObjectClass(env, callback);
	jni->set_timeout   = (*env)->GetMethodID(env, jni->cls, "SetTimeout", "(I)V");
	jni->set_break     = (*env)->GetMethodID(env, jni->cls, "SetBreak", "(Z)V");
	jni->set_dtr       = (*env)->GetMethodID(env, jni->cls, "SetDtr", "(Z)V");
	jni->set_rts       = (*env)->GetMethodID(env, jni->cls, "SetRts", "(Z)V");
	jni->get_lines     = (*env)->GetMethodID(env, jni->cls, "GetLines", "()I");
	jni->get_available = (*env)->GetMethodID(env, jni->cls, "GetAvailable", "()I");
	jni->configure     = (*env)->GetMethodID(env, jni->cls, "Configure", "(IIIII)V");
	jni->poll          = (*env)->GetMethodID(env, jni->cls, "Poll", "(I)V");
	jni->read          = (*env)->GetMethodID(env, jni->cls, "Read", "()[B");
	jni->write         = (*env)->GetMethodID(env, jni->cls, "Write", "([B)V");
	jni->ioctl         = (*env)->GetMethodID(env, jni->cls, "Ioctl", "(I[B)V");
	jni->flush         = (*env)->GetMethodID(env, jni->cls, "Flush", "()V");
	jni->purge         = (*env)->GetMethodID(env, jni->cls, "Purge", "(I)V");
	jni->sleep         = (*env)->GetMethodID(env, jni->cls, "Sleep", "(I)V");
	jni->close         = (*env)->GetMethodID(env, jni->cls, "Close", "()V");

	status = dc_custom_open (&iostream,
		(dc_context_t *) context,
		transport,
		&callbacks,
		jni);
	if (status != DC_STATUS_SUCCESS) {
		return 0;
	}

	return (jlong) iostream;
}
