#include "org_libdivecomputer_Device.h"

#include <libdivecomputer/device.h>

typedef struct jni_device_t {
	JNIEnv *env;
	jobject obj;
	jclass cls;
	jmethodID method;
} jni_device_t;

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
	jni.env = env;
	jni.obj = (*env)->NewGlobalRef(env, callback);
	jni.cls = (*env)->GetObjectClass(env, callback);
	jni.method = (*env)->GetMethodID(env, jni.cls, "Dive", "([B[B)I");

	dc_device_foreach((dc_device_t *) handle, dive_cb, &jni);

	(*env)->DeleteGlobalRef(env, jni.obj);
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
