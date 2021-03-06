#pragma once

#include <stddef.h>

#ifdef _WIN32
#define AC_API __stdcall
#ifndef AC_DLL
#define AC_DLL __declspec(dllimport)
#else
#undef AC_DLL
#define AC_DLL __declspec(dllexport)
#endif
#else
#define AC_API
#ifndef AC_DLL
#define AC_DLL
#endif
#endif

#if defined(__GNUC__) || defined(__clang__)
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#define DEPRECATED
#endif

#define acSaveImageYUV acSaveImageRGB
#define acSaveImageYUV444Bytes acSaveImageRGBBytes

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum ac_processType
    {
        AC_CPU_Anime4K09,
        AC_CPU_ACNet,
        AC_OpenCL_Anime4K09,
        AC_OpenCL_ACNet,
        AC_Cuda_Anime4K09,
        AC_Cuda_ACNet,
        //deprecated, use AC_CPU_Anime4K09
        AC_CPU = AC_CPU_Anime4K09,
        //deprecated, use AC_CPU_ACNet
        AC_CPUCNN = AC_CPU_ACNet,
        //deprecated, use AC_OpenCL_Anime4K09
        AC_GPU = AC_OpenCL_Anime4K09,
        //deprecated, use AC_OpenCL_ACNet
        AC_GPUCNN = AC_OpenCL_ACNet
    } ac_processType;

    typedef enum ac_CNNType
    {
        AC_Default,
        AC_ACNetHDNL0,
        AC_ACNetHDNL1,
        AC_ACNetHDNL2,
        AC_ACNetHDNL3
    } ac_CNNType;

    typedef enum ac_GPGPU
    {
        AC_CUDA,
        AC_OpenCL
    } ac_GPGPU;

    typedef enum ac_manager
    {
        AC_Manager_OpenCL_Anime4K09 = 1 << 0,
        AC_Manager_OpenCL_ACNet = 1 << 1,
        AC_Manager_Cuda = 1 << 2
    } ac_manager;

    typedef enum ac_bool
    {
        AC_FALSE = 0,
        AC_TRUE = 1
    } ac_bool;

    typedef enum ac_error
    {
        AC_OK = 0,
        AC_ERROR_NULL_INSTANCE,
        AC_ERROR_NULL_PARAMETERS,
        AC_ERROR_NULL_Data,
        AC_ERROR_INIT_GPU,
        AC_ERROR_PORCESSOR_TYPE,
        AC_ERROR_LOAD_IMAGE,
        AC_ERROR_LOAD_VIDEO,
        AC_ERROR_INIT_VIDEO_WRITER,
        AC_ERROR_GPU_PROCESS,
        AC_ERROR_SAVE_TO_NULL_POINTER,
        AC_ERROR_NOT_YUV444,
        AC_ERROR_YUV444_AND_RGB32_AT_SAME_TIME,
        AC_ERROR_CUDA_NOT_SUPPORTED
    } ac_error;

    typedef enum ac_codec
    {
        OTHER = -1,
        MP4V = 0,
        DXVA = 1,
        AVC1 = 2,
        VP09 = 3,
        HEVC = 4,
        AV01 = 5
    } ac_codec;

    typedef struct ac_parameters
    {
        int passes;
        int pushColorCount;
        double strengthColor;
        double strengthGradient;
        double zoomFactor;
        ac_bool fastMode;
        ac_bool videoMode;
        ac_bool preprocessing;
        ac_bool postprocessing;
        unsigned char preFilters;
        unsigned char postFilters;
        unsigned int maxThreads;
        ac_bool HDN;
        int HDNLevel;
        ac_bool alpha;
    } ac_parameters;

    typedef struct ac_version
    {
        char coreVersion[6];
        char wrapperVersion[6];
    } ac_version;

    typedef struct ac_OpenCLAnime4K09Data
    {
        unsigned int pID;
        unsigned int dID;
    } ac_OpenCLAnime4K09Data;

    typedef struct ac_OpenCLACNetData
    {
        unsigned int pID;
        unsigned int dID;
        ac_CNNType CNNType;
    } ac_OpenCLACNetData;

    typedef struct ac_CUDAData
    {
        unsigned int dID;
    } ac_CUDAData;

    typedef struct ac_managerData
    {
        ac_OpenCLAnime4K09Data* OpenCLAnime4K09Data;
        ac_OpenCLACNetData* OpenCLACNetData;
        ac_CUDAData* CUDAData;
    } ac_managerData;

    typedef void* ac_instance;

    AC_DLL ac_version AC_API acGetVersion(void);
    DEPRECATED AC_DLL ac_instance AC_API acGetInstance(
        ac_bool initGPU, ac_bool initGPUCNN,
        unsigned int platformID, unsigned int deviceID,
        ac_parameters* parameters, ac_processType type,
        ac_error* error
    );
    AC_DLL ac_instance AC_API acGetInstance2(
        unsigned int managers, ac_managerData* managerData,
        ac_parameters* parameters, ac_processType type,
        ac_error* error
    );
    DEPRECATED AC_DLL void AC_API acFreeInstance(ac_instance instance, ac_bool releaseGPU, ac_bool releaseGPUCNN);
    AC_DLL void AC_API acFreeInstance2(ac_instance instance);
    AC_DLL ac_error AC_API acInitParameters(ac_parameters* parameters);
    AC_DLL ac_error AC_API acLoadImage(ac_instance instance, const char* srcFile);
    AC_DLL ac_error AC_API acLoadVideo(ac_instance instance, const char* srcFile);
    AC_DLL ac_error AC_API acProcess(ac_instance instance);
    AC_DLL ac_error AC_API acProcessWithPrintProgress(ac_instance instance);
    /*
    Processing with callback funciton:
    callBack(double progress);
    progress form 0 to 1
    */
    AC_DLL ac_error AC_API acProcessWithProgress(ac_instance instance, void (*callBack)(double));
    /*
    Processing with callback funciton:
    callBack(double progress, double elapsedTime);
    progress form 0 to 1
    */
    AC_DLL ac_error AC_API acProcessWithProgressTime(ac_instance instance, void (*callBack)(double, double));
    AC_DLL ac_error AC_API acStopVideoProcess(ac_instance instance);
    AC_DLL ac_error AC_API acPauseVideoProcess(ac_instance instance);
    AC_DLL ac_error AC_API acContinueVideoProcess(ac_instance instance);
    AC_DLL ac_error AC_API acShowImage(ac_instance instance, ac_bool R2B);
    AC_DLL ac_error AC_API acSaveImage(ac_instance instance, const char* dstFile);
    AC_DLL ac_error AC_API acSetSaveVideoInfo(ac_instance instance, const char* dstFile, ac_codec codec, double fps);
    AC_DLL ac_error AC_API acSaveVideo(ac_instance instance);
    AC_DLL ac_error AC_API acSetArguments(ac_instance instance, ac_parameters* parameters);
    AC_DLL ac_error AC_API acSetVideoMode(ac_instance instance, ac_bool flag);
    DEPRECATED AC_DLL ac_error AC_API acInitGPU(void);
    DEPRECATED AC_DLL void AC_API acReleaseGPU(void);
    DEPRECATED AC_DLL ac_error AC_API acInitGPUCNN(void);
    DEPRECATED AC_DLL void AC_API acReleaseGPUCNN(void);
    AC_DLL ac_error AC_API acInitGPU2(unsigned int managers, ac_managerData* managerData);
    AC_DLL void AC_API acReleaseGPU2(void);
    AC_DLL ac_error AC_API acLoadImageRGB(ac_instance instance, int rows, int cols, unsigned char* r, unsigned char* g, unsigned char* b, ac_bool inputAsYUV444);
    AC_DLL ac_error AC_API acLoadImageYUV(ac_instance instance, int rowsY, int colsY, unsigned char* y, int rowsU, int colsU, unsigned char* u, int rowsV, int colsV, unsigned char* v);
    AC_DLL ac_error AC_API acLoadImageRGBBytes(ac_instance instance, int rows, int cols, unsigned char* data, size_t bytesPerLine, ac_bool inputAsYUV444, ac_bool inputAsRGB32);
    AC_DLL ac_error AC_API acSaveImageRGB(ac_instance instance, unsigned char** r, unsigned char** g, unsigned char** b);
    AC_DLL ac_error AC_API acSaveImageRGBBytes(ac_instance instance, unsigned char** data);
    AC_DLL size_t AC_API acGetResultDataLength(ac_instance instance, ac_error* error);
    AC_DLL size_t AC_API acGetResultDataPerChannelLength(ac_instance instance, ac_error* error);
    //shape should be int[3]
    AC_DLL ac_error AC_API acGetResultShape(ac_instance instance, int shape[3]);
    //acGetInfo may need to run two times for getting length of info string first
    AC_DLL ac_error AC_API acGetInfo(ac_instance instance, char* info, size_t* length);
    //acGetFiltersInfo may need to run two times for getting length of info string first
    AC_DLL ac_error AC_API acGetFiltersInfo(ac_instance instance, char* info, size_t* length);
    //acCheckGPUSupport may need to run two times for getting length of info string first
    DEPRECATED AC_DLL ac_bool AC_API acCheckGPUSupport(unsigned int pID, unsigned int dID, char* info, size_t* length);
    AC_DLL ac_bool AC_API acCheckGPUSupport2(ac_GPGPU GPGPUModel, unsigned int pID, unsigned int dID, char* info, size_t* length);
    //acCheckGPUSupport may need to run two times for getting length of info string and length(platforms) of devices first
    AC_DLL void AC_API acListGPUs(char* info, size_t* length, size_t* platforms, size_t* devices);
    AC_DLL ac_bool AC_API acIsInitializedGPU(void);
    AC_DLL ac_bool AC_API acIsInitializedGPUCNN(void);
    AC_DLL void AC_API acGetLastCoreErrorString(char* err, size_t* length);
    DEPRECATED AC_DLL void AC_API acBenchmark(unsigned int pID, unsigned int dID, double* CPUScore, double* GPUScore);
    AC_DLL double AC_API acBenchmark2(ac_processType processType);
    AC_DLL ac_processType AC_API acGetProcessType(ac_instance instance, ac_error* error);
#ifdef __cplusplus
}
#endif
