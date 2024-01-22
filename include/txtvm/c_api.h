#ifndef TVM_C_API_H_
#define TVM_C_API_H_

#ifdef __cplusplus
#define TVM_EXTERN_C extern "C"
#else
#define TVM_EXTERN_C
#endif

#ifdef _WIN32
#ifdef TVM_EXPORTS
#define TVM_DLL TVM_EXTERN_C __declspec(dllexport)
#else
#define TVM_DLL TVM_EXTERN_C __declspec(dllimport)
#endif
#else
#define TVM_DLL TVM_EXTERN_C
#endif

typedef void* FunctionHandle;
typedef void* NodeHandle;

typedef union {
    long v_long;
    double v_double;
    const char* v_str;
    NodeHandle v_handle;
} ArgVariant;

typedef enum {
    kNull       = 0,
    kLong       = 1,
    kDouble     = 2,
    kStr        = 3,
    kNodeHandle = 4
} ArgVariantID;

TVM_DLL const char *TVMGetLastError(void);

TVM_DLL int TVMListFunctionNames(int *out_size, const char*** out_array);

TVM_DLL int TVMGetFunctionHandle(const char* name, FunctionHandle *handle);

TVM_DLL int TVMGetFunctionInfo(FunctionHandle handle,
                          const char **real_name,
                          const char **description,
                          int *num_doc_args,
                          const char ***arg_names,
                          const char ***arg_type_infos,
                          const char ***arg_descriptions,
                          const char **return_type);


TVM_DLL int TVMPushStack(ArgVariant arg, int type_id);

TVM_DLL int TVMFunctionCall(FunctionHandle handle, 
                            ArgVariant* ret_val,
                            int* ret_typeid);

TVM_DLL int TVMNodeFree(NodeHandle handle);

TVM_DLL int TVMNodeGetAttr(NodeHandle handle,
                           const char* key,
                           ArgVariant* out_value,
                           int* out_typeid);

#endif // TVM_C_API_H_
