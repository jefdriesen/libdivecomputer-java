#include <libdivecomputer/parser.h>

#include "org_libdivecomputer_Parser.h"

typedef struct jni_parser_t {
	JNIEnv *env;
	jobject obj;
	jclass cls;
	jmethodID method;
} jni_parser_t;

static void
sample_cb (dc_sample_type_t type, const dc_sample_value_t *value, void *userdata)
{
	jni_parser_t *jni = userdata;

	(*jni->env)->CallVoidMethod(jni->env, jni->obj, jni->method,
		type);
}

JNIEXPORT jlong JNICALL Java_org_libdivecomputer_Parser_New
  (JNIEnv *env, jobject obj, jlong device, jbyteArray data)
{
	dc_status_t status = DC_STATUS_SUCCESS;
	dc_parser_t *parser = NULL;

	// Get the pointer and length.
	jboolean isCopy = 0;
	jint len = (*env)->GetArrayLength(env, data);
	jbyte *buf = (*env)->GetByteArrayElements(env, data, &isCopy);

	status = dc_parser_new (&parser,
		(dc_device_t *) device,
		buf,
		len);

	// Release the pointer.
	(*env)->ReleaseByteArrayElements(env, data, buf, 0);

	if (status != DC_STATUS_SUCCESS) {
		return 0;
	}

	return (jlong) parser;
}

JNIEXPORT jlong JNICALL Java_org_libdivecomputer_Parser_New2
  (JNIEnv *env, jobject obj, jlong context, jlong descriptor, jbyteArray data)
{
	dc_status_t status = DC_STATUS_SUCCESS;
	dc_parser_t *parser = NULL;

	// Get the pointer and length.
	jboolean isCopy = 0;
	jint len = (*env)->GetArrayLength(env, data);
	jbyte *buf = (*env)->GetByteArrayElements(env, data, &isCopy);

	status = dc_parser_new2 (&parser,
		(dc_context_t *) context,
		(dc_descriptor_t *) descriptor,
		buf,
		len);

	// Release the pointer.
	(*env)->ReleaseByteArrayElements(env, data, buf, 0);

	if (status != DC_STATUS_SUCCESS) {
		return 0;
	}

	return (jlong) parser;
}

JNIEXPORT void JNICALL Java_org_libdivecomputer_Parser_Foreach
  (JNIEnv *env, jobject obj, jlong handle, jobject callback)
{
	jni_parser_t jni = {0};
	jni.env = env;
	jni.obj = (*env)->NewGlobalRef(env, callback);
	jni.cls = (*env)->GetObjectClass(env, callback);
	jni.method = (*env)->GetMethodID(env, jni.cls, "Sample", "(I)V");

	dc_parser_samples_foreach((dc_parser_t *) handle, sample_cb, &jni);

	(*env)->DeleteGlobalRef(env, jni.obj);
}

JNIEXPORT void JNICALL Java_org_libdivecomputer_Parser_GetDatetime
  (JNIEnv *env, jobject obj, jlong handle, jobject value)
{
	dc_datetime_t datetime = {0};
	dc_parser_get_datetime ((dc_parser_t *) handle, &datetime);

	jclass cls = (*env)->GetObjectClass(env, value);
	jfieldID fid_year = (*env)->GetFieldID(env, cls, "year", "I");
	jfieldID fid_month = (*env)->GetFieldID(env, cls, "month", "I");
	jfieldID fid_day = (*env)->GetFieldID(env, cls, "day", "I");
	jfieldID fid_hour = (*env)->GetFieldID(env, cls, "hour", "I");
	jfieldID fid_minute = (*env)->GetFieldID(env, cls, "minute", "I");
	jfieldID fid_second = (*env)->GetFieldID(env, cls, "second", "I");
	jfieldID fid_timezone = (*env)->GetFieldID(env, cls, "timezone", "I");

	(*env)->SetIntField(env, value, fid_year, datetime.year);
	(*env)->SetIntField(env, value, fid_month, datetime.month);
	(*env)->SetIntField(env, value, fid_day, datetime.day);
	(*env)->SetIntField(env, value, fid_hour, datetime.hour);
	(*env)->SetIntField(env, value, fid_minute, datetime.minute);
	(*env)->SetIntField(env, value, fid_second, datetime.second);
	(*env)->SetIntField(env, value, fid_timezone, datetime.timezone);
}
