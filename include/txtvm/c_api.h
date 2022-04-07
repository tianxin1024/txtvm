#ifndef TXTVM_C_API_H_
#define TXTVM_C_API_H_

#ifdef __cplusplus
#define TXTVM_EXTERN_C extern "C"
#else
#define TXTVM_EXTERN_C
#endif

/*! \brief TXTVM_DLL prefix for windows */
#ifdef _WIN32
#ifdef TXTVM_EXPORTS
#define TXTVM_DLL TXTVM_EXTERN_C __declspec(dllexport)
#else
#define TXTVM_DLL TXTVM_EXTERN_C __declspec(dllimport)
#endif
#else
#define TXTVM_DLL TXTVM_EXTERN_C
#endif

/*! \brief handle to node creator */
typedef void* NodeCreatorHandle;
/*! \brief handle to node */
typedef void* NodeHandle;

TXTVM_DLL int TXTVMNodeCreatorGet(const char* node_type,
                              NodeCreatorHandle *handle);

TXTVM_DLL int TXTVMNodeCreate(NodeCreatorHandle handle,
                          int num_child_ref,
                          const char* child_ref_keys,
                          NodeHandle* child_node_refs,
                          int num_attrs,
                          const char* attr_keys,
                          const char* attr_vals,
                          NodeHandle* handle);

TXTVM_DLL int TXTVMNodeGetAttr(const char* key,
                           const char** value);

TXTVM_DLL int TXTVMNodeGetChildNodeRef(const char* key,
                                   NodeHandle* out);


#endif // TXTVM_C_API_H_
