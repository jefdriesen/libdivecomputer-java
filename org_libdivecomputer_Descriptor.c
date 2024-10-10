#include "org_libdivecomputer_Descriptor.h"

#include <libdivecomputer/descriptor.h>

JNIEXPORT void JNICALL Java_org_libdivecomputer_Descriptor_Free
  (JNIEnv *env, jobject obj, jlong handle)
{
	dc_descriptor_free ((dc_descriptor_t *) handle);
}

JNIEXPORT jstring JNICALL Java_org_libdivecomputer_Descriptor_Vendor
  (JNIEnv *env, jobject obj, jlong handle)
{
/*
	jclass cls = (*env)->GetObjectClass(env, obj);
	jfieldID fid = (*env)->GetFieldID(env, cls, "handle", "J");
	jlong handle = (*env)->GetLongField(env, obj, fid);
*/

	const char *result = dc_descriptor_get_vendor ((dc_descriptor_t *) handle);
	return (*env)->NewStringUTF (env, result);
}

JNIEXPORT jstring JNICALL Java_org_libdivecomputer_Descriptor_Product
  (JNIEnv *env, jobject obj, jlong handle)
{
	const char *result = dc_descriptor_get_product ((dc_descriptor_t *) handle);
	return (*env)->NewStringUTF (env, result);
}

JNIEXPORT jint JNICALL Java_org_libdivecomputer_Descriptor_Type
  (JNIEnv *env, jobject obj, jlong handle)
{
	return dc_descriptor_get_type ((dc_descriptor_t *) handle);
}

JNIEXPORT jint JNICALL Java_org_libdivecomputer_Descriptor_Model
  (JNIEnv *env, jobject obj, jlong handle)
{
	return dc_descriptor_get_model ((dc_descriptor_t *) handle);
}

JNIEXPORT jint JNICALL Java_org_libdivecomputer_Descriptor_Transports
  (JNIEnv *env, jobject obj, jlong handle)
{
	return dc_descriptor_get_transports ((dc_descriptor_t *) handle);
}

JNIEXPORT jobject JNICALL Java_org_libdivecomputer_Descriptor_Iterator
  (JNIEnv *env, jclass class)
{
	jclass cls = (*env)->FindClass(env, "java/util/ArrayList");
	jmethodID init = (*env)->GetMethodID(env, cls, "<init>", "()V");
	jmethodID add = (*env)->GetMethodID(env, cls, "add", "(Ljava/lang/Object;)Z");

	jclass desc_cls = class;
	jmethodID desc_init = (*env)->GetMethodID(env, desc_cls, "<init>", "(J)V");

	jobject result = (*env)->NewObject(env, cls, init);

	dc_iterator_t *iterator = NULL;
	dc_descriptor_t *descriptor = NULL;
	dc_descriptor_iterator (&iterator);
	while (dc_iterator_next (iterator, &descriptor) == DC_STATUS_SUCCESS) {
		/*printf ("%s %s\n",
			dc_descriptor_get_vendor (descriptor),
			dc_descriptor_get_product (descriptor));*/

		jobject item = (*env)->NewObject(env, desc_cls, desc_init, descriptor);

		(*env)->CallBooleanMethod(env, result, add, item);

		//dc_descriptor_free (descriptor);
	}
	dc_iterator_free (iterator);

	return result;
}
