/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_libdivecomputer_Parser */

#ifndef _Included_org_libdivecomputer_Parser
#define _Included_org_libdivecomputer_Parser
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_libdivecomputer_Parser
 * Method:    New
 * Signature: (J[B)J
 */
JNIEXPORT jlong JNICALL Java_org_libdivecomputer_Parser_New
  (JNIEnv *, jobject, jlong, jbyteArray);

/*
 * Class:     org_libdivecomputer_Parser
 * Method:    New2
 * Signature: (JJ[B)J
 */
JNIEXPORT jlong JNICALL Java_org_libdivecomputer_Parser_New2
  (JNIEnv *, jobject, jlong, jlong, jbyteArray);

/*
 * Class:     org_libdivecomputer_Parser
 * Method:    Free
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_libdivecomputer_Parser_Free
  (JNIEnv *, jobject, jlong);

/*
 * Class:     org_libdivecomputer_Parser
 * Method:    Foreach
 * Signature: (JLorg/libdivecomputer/Parser/Callback;)V
 */
JNIEXPORT void JNICALL Java_org_libdivecomputer_Parser_Foreach
  (JNIEnv *, jobject, jlong, jobject);

/*
 * Class:     org_libdivecomputer_Parser
 * Method:    GetDatetime
 * Signature: (JLorg/libdivecomputer/Parser/Datetime;)V
 */
JNIEXPORT void JNICALL Java_org_libdivecomputer_Parser_GetDatetime
  (JNIEnv *, jobject, jlong, jobject);

/*
 * Class:     org_libdivecomputer_Parser
 * Method:    GetSalinity
 * Signature: (JLorg/libdivecomputer/Parser/Salinity;)V
 */
JNIEXPORT void JNICALL Java_org_libdivecomputer_Parser_GetSalinity
  (JNIEnv *, jobject, jlong, jobject);

/*
 * Class:     org_libdivecomputer_Parser
 * Method:    GetDecomodel
 * Signature: (JLorg/libdivecomputer/Parser/Decomodel;)V
 */
JNIEXPORT void JNICALL Java_org_libdivecomputer_Parser_GetDecomodel
  (JNIEnv *, jobject, jlong, jobject);

/*
 * Class:     org_libdivecomputer_Parser
 * Method:    GetGasmix
 * Signature: (JLorg/libdivecomputer/Parser/Gasmix;I)V
 */
JNIEXPORT void JNICALL Java_org_libdivecomputer_Parser_GetGasmix
  (JNIEnv *, jobject, jlong, jobject, jint);

/*
 * Class:     org_libdivecomputer_Parser
 * Method:    GetTank
 * Signature: (JLorg/libdivecomputer/Parser/Tank;I)V
 */
JNIEXPORT void JNICALL Java_org_libdivecomputer_Parser_GetTank
  (JNIEnv *, jobject, jlong, jobject, jint);

/*
 * Class:     org_libdivecomputer_Parser
 * Method:    GetFieldInt
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_org_libdivecomputer_Parser_GetFieldInt
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     org_libdivecomputer_Parser
 * Method:    GetFieldDouble
 * Signature: (JI)D
 */
JNIEXPORT jdouble JNICALL Java_org_libdivecomputer_Parser_GetFieldDouble
  (JNIEnv *, jobject, jlong, jint);

#ifdef __cplusplus
}
#endif
#endif
