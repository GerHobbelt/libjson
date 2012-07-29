#include "JSON_ASSERT.h"
#include "../../Source/JSONDebug.h"

#if defined JSON_DEBUG
	#ifndef JSON_STDERROR

		// http://www.parashift.com/c++-faq-lite/static-init-order.html (sections 10.14 - 10.17)
		static json_string &last_f() {
			static json_string last_s;
			return last_s;
		}
		#define last             last_f()
		// ~ static json_string last;

		#ifdef JSON_LIBRARY
			static void callback(const json_char * p){ last = p; }
		#else
			static void callback(const json_string & p){ last = p; }
		#endif
	#endif
#endif

// http://www.parashift.com/c++-faq-lite/static-init-order.html (sections 10.14 - 10.17)
static const json_string &fail_consta_f() {
	static const json_string fca = JSON_TEXT("fail"); //should pass the same pointer all the way through, no copies
	return fca;
}
#define fail_consta         fail_consta_f()
// ~ const json_string fail_consta = JSON_TEXT("fail"); //should pass the same pointer all the way through, no copies

// http://www.parashift.com/c++-faq-lite/static-init-order.html (sections 10.14 - 10.17)
static const json_string &null_consta_f() {
	static json_string nca = JSON_TEXT("");
	return nca;
}
#define null_consta         null_consta_f()
// ~ const json_string null_consta = JSON_TEXT("");

#if defined JSON_DEBUG || defined JSON_SAFE
	static json_error_callback_t origCallbacka = NULL;
#endif

void testJSONDebug_JSON_ASSERT::setUp(const std::string & methodName){
	BaseTest::setUp(methodName);
	#if defined JSON_DEBUG
		#ifndef JSON_STDERROR
			origCallbacka = JSONDebug::register_callback(callback);  //check that the callback was called
			last = null_consta;
		#endif
	#endif
}

void testJSONDebug_JSON_ASSERT::tearDown(void){
	BaseTest::tearDown();
	#if defined JSON_DEBUG
		#ifndef JSON_STDERROR
			JSONDebug::register_callback(origCallbacka);  //check that the callback was called
		#endif
	#endif
}


/**
 *	Make sure asserts that pass do not call the callback or run extra code
 */
void testJSONDebug_JSON_ASSERT::testPass(void){
	#if defined JSON_DEBUG
		#ifndef JSON_STDERROR
			JSON_ASSERT(1 == 1, fail_consta);
			assertEquals(last, null_consta);  //make sure the callback was not called
		#endif
	#endif
}


/**
 *	Make sure asserts that fail do call the callback and run extra code
 */
void testJSONDebug_JSON_ASSERT::testFail(void){
	#if defined JSON_DEBUG
		#ifndef JSON_STDERROR
			JSON_ASSERT(1 == 0, fail_consta);
			assertEquals(last, fail_consta);  //make sure the callback was actually called
		#endif
	#endif
}
