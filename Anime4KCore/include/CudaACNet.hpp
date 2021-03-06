#pragma once

#ifdef ENABLE_CUDA

#include"CudaInterface.hpp"
#include"AC.hpp"
#include"CNN.hpp"

namespace Anime4KCPP
{
    namespace Cuda
    {
        class DLL ACNet;
    }
}

class Anime4KCPP::Cuda::ACNet :public AC
{
public:
    ACNet(const Parameters& parameters = Parameters());
    virtual ~ACNet() = default;

    virtual std::string getInfo() override;
    virtual std::string getFiltersInfo() override;
private:
    void runKernelB(const cv::Mat& orgImg, cv::Mat& dstImg);
    void runKernelW(const cv::Mat& orgImg, cv::Mat& dstImg);
    void runKernelF(const cv::Mat& orgImg, cv::Mat& dstImg);

    virtual void processYUVImageB() override;
    virtual void processRGBImageB() override;
    virtual void processRGBVideoB() override;

    virtual void processYUVImageW() override;
    virtual void processRGBImageW() override;

    virtual void processYUVImageF() override;
    virtual void processRGBImageF() override;

    virtual Processor::Type getProcessorType() noexcept override;
};

#endif
