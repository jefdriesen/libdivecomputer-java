#include "org_libdivecomputer_Device.h"

#include <libdivecomputer/device.h>

typedef struct jni_device_t {
	JNIEnv *env;
	jobject obj;
	jclass cls;
	jmethodID method;
} jni_device_t;

typedef struct jni_events_t {
	JNIEnv *env;
	jobject obj;
	jclass cls;
	jmethodID waiting;
	jmethodID progress;
	jmethodID devinfo;
	jmethodID clock;
	jmethodID vendor;
} jni_events_t;

static int
dive_cb (const unsigned char *data, unsigned int size, const unsigned char *fingerprint, unsigned int fsize, void *userdata)
{
	jni_device_t *jni = userdata;

	jbyteArray dive = (*jni->env)->NewByteArray(jni->env, size);
	(*jni->env)->SetByteArrayRegion (jni->env, dive, 0, size, data);

	jbyteArray fp = (*jni->env)->NewByteArray(jni->env, fsize);
	(*jni->env)->SetByteArrayRegion (jni->env, fp, 0, fsize, fingerprint);

	return (*jni->env)->CallIntMethod(jni->env, jni->obj, jni->method,
		dive,
		fp);
}

static void
event_cb (dc_device_t *device, dc_event_type_t event, const void *data, void *userdata)
{
	jni_events_t *jni = userdata;
	jbyteArray array = NULL;

	const dc_event_progress_t *progress = (const dc_event_progress_t *) data;
	const dc_event_devinfo_t *devinfo = (const dc_event_devinfo_t *) data;
	const dc_event_clock_t *clock = (const dc_event_clock_t *) data;
	const dc_event_vendor_t *vendor = (const dc_event_vendor_t *) data;

	switch (event) {
	case DC_EVENT_WAITING:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->waiting);
		break;
	case DC_EVENT_PROGRESS:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->progress,
			100.0 * (double) progress->current / (double) progress->maximum);
		break;
	case DC_EVENT_DEVINFO:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->devinfo,
			devinfo->model, devinfo->firmware, devinfo->serial);
		break;
	case DC_EVENT_CLOCK:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->clock,
			clock->devtime, clock->systime);
		break;
	case DC_EVENT_VENDOR:
		array = (*jni->env)->NewByteArray(jni->env, vendor->size);
		(*jni->env)->SetByteArrayRegion (jni->env, array, 0, vendor->size, vendor->data);
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->vendor,
			array);
		break;
	default:
		break;
	}
}

static int
cancel_cb (void *userdata)
{
	jni_device_t *jni = userdata;

	return (*jni->env)->CallBooleanMethod(jni->env, jni->obj, jni->method);
}

JNIEXPORT jlong JNICALL Java_org_libdivecomputer_Device_Open
  (JNIEnv *env, jobject, jlong context, jlong descriptor, jlong iostream)
{
	dc_status_t status = DC_STATUS_SUCCESS;
	dc_device_t *device = NULL;

	status = dc_device_open (&device,
		(dc_context_t *) context,
		(dc_descriptor_t *) descriptor,
		(dc_iostream_t *) iostream);
	if (status != DC_STATUS_SUCCESS) {
		return 0;
	}

	return (jlong) device;
}

JNIEXPORT void JNICALL Java_org_libdivecomputer_Device_Foreach
  (JNIEnv *env, jobject obj, jlong handle, jobject callback)
{
	jni_device_t jni = {0};

	if (callback) {
		jni.env = env;
		jni.obj = (*env)->NewGlobalRef(env, callback);
		jni.cls = (*env)->GetObjectClass(env, callback);
		jni.method = (*env)->GetMethodID(env, jni.cls, "Dive", "([B[B)I");

		dc_device_foreach((dc_device_t *) handle, dive_cb, &jni);

		(*env)->DeleteGlobalRef(env, jni.obj);
	} else {
		dc_device_foreach((dc_device_t *) handle, NULL, NULL);
	}
}

JNIEXPORT void JNICALL Java_org_libdivecomputer_Device_SetFingerprint
  (JNIEnv *env, jobject obj, jlong handle, jbyteArray fingerprint)
{
	dc_status_t status = DC_STATUS_SUCCESS;
	unsigned char *buf = NULL;
	unsigned int len = 0;

	// Get the pointer and length.
	if (fingerprint) {
		jboolean isCopy = 0;
		len = (*env)->GetArrayLength(env, fingerprint);
		buf = (*env)->GetByteArrayElements(env, fingerprint, &isCopy);
	}

	status = dc_device_set_fingerprint((dc_device_t *) handle, buf, len);

	// Release the pointer.
	if (fingerprint) {
		(*env)->ReleaseByteArrayElements(env, fingerprint, buf, JNI_ABORT);
	}
}

/*
 * Class:     org_libdivecomputer_Device
 * Method:    SetEvents
 * Signature: (JLorg/libdivecomputer/Device/Events;)V
 */
JNIEXPORT void JNICALL Java_org_libdivecomputer_Device_SetEvents
  (JNIEnv *env, jobject obj, jlong handle, jobject events)
{
	static jni_events_t jni = {0}; // FIXME: Not thread-safe.

	if (events) {
		jni.env = env;
		jni.obj = (*env)->NewGlobalRef(env, events); // FIXME: Memory leak.
		jni.cls = (*env)->GetObjectClass(env, events);
		jni.waiting = (*env)->GetMethodID(env, jni.cls, "Waiting", "()V");
		jni.progress = (*env)->GetMethodID(env, jni.cls, "Progress", "(D)V");
		jni.devinfo = (*env)->GetMethodID(env, jni.cls, "Devinfo", "(III)V");
		jni.clock = (*env)->GetMethodID(env, jni.cls, "Clock", "(JJ)V");
		jni.vendor = (*env)->GetMethodID(env, jni.cls, "Vendor", "([B)V");

		dc_device_set_events ((dc_device_t *) handle, DC_EVENT_WAITING | DC_EVENT_PROGRESS | DC_EVENT_DEVINFO | DC_EVENT_CLOCK | DC_EVENT_VENDOR, event_cb, &jni);
	} else {
		dc_device_set_events ((dc_device_t *) handle, 0, NULL, NULL);
	}
}

/*
 * Class:     org_libdivecomputer_Device
 * Method:    SetCancel
 * Signature: (JLorg/libdivecomputer/Device/Cancel;)V
 */
JNIEXPORT void JNICALL Java_org_libdivecomputer_Device_SetCancel
  (JNIEnv *env, jobject obj, jlong handle, jobject cancel)
{
	static jni_device_t jni = {0}; // FIXME: Not thread-safe.

	if (cancel) {
		jni.env = env;
		jni.obj = (*env)->NewGlobalRef(env, cancel); // FIXME: Memory leak.
		jni.cls = (*env)->GetObjectClass(env, cancel);
		jni.method = (*env)->GetMethodID(env, jni.cls, "Cancel", "()Z");

		dc_device_set_cancel ((dc_device_t *) handle, cancel_cb, &jni);
	} else {
		dc_device_set_cancel ((dc_device_t *) handle, NULL, NULL);
	}
}
