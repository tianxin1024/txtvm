#ifndef TVM_C_API_C_API_COMMON_H_
#define TVM_C_API_C_API_COMMON_H_

#include <dmlc/base.h>
#include <dmlc/logging.h>
#include <dmlc/thread_local.h>
#include "tvm/c_api.h"
#include <vector>
#include <string>
#include <exception>
#include "c_api_registry.h"

#define API_BEGIN() try {

#define API_END() } catch(dmlc::Error &_except_) { return TVMAPIHandleException(_except_); } return 0;

#define API_END_HANDLE_ERROR(Finalize) } catch(dmlc::Error &_except_) { Finalize; return TVMAPIHandleException(_except_); } return 0;

void TVMAPISetLastError(const char* msg);

inline int TVMAPIHandleException(const dmlc::Error &e) {
    TVMAPISetLastError(e.what());
    return -1;
}

#endif // !TVM_C_API_C_API_COMMON_H_
