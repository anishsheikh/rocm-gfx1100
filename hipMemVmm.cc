/*
   Copyright (c) 2022 Advanced Micro Devices, Inc. All rights reserved.
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANNTY OF ANY KIND, EXPRESS OR
   IMPLIED, INNCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANNY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER INN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR INN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
 */

/* Test Case Description:
   1) This testcase verifies the  basic scenario - supported on
     all devices
*/

#include <hip/hip_runtime.h>
#include <iostream>
#include <vector>

/*
    This testcase verifies HIP Mem VMM API basic scenario - supported on all devices
 */

int main() {
  int vmm = 0;
  hipError_t ret;
  ret = hipDeviceGetAttribute(&vmm, hipDeviceAttributeVirtualMemoryManagementSupported, 0);
  if(ret != hipSuccess) {
    std::cout << "Failed to query Attribute " << std::endl;
  }
  std::cout << "hipDeviceAttributeVirtualMemoryManagementSupported: " << vmm << std::endl;

  if (vmm == 0) {
    std::cout << "GPU 0 doesn't support hipDeviceAttributeVirtualMemoryManagement "
           "attribute. Hence skipping the testing with Pass result.\n" << std::endl;
  }

  size_t granularity = 0;

  hipMemAllocationProp memAllocationProp = {};
  memAllocationProp.type = hipMemAllocationTypePinned;
  memAllocationProp.location.id = 0;
  memAllocationProp.location.type = hipMemLocationTypeDevice;

  ret = hipMemGetAllocationGranularity(&granularity, &memAllocationProp, hipMemAllocationGranularityRecommended);
  if(ret != hipSuccess) {
    std::cout << "Failed get granularity " << std::endl;
  }
  else {
    std::cout << "Successfully got granularity "  << std::endl;  
  }
  size_t size = 6442450944;
  size_t size2 = 2 *1024;
  void* reservedAddress = nullptr;
  ret = hipMemAddressReserve(&reservedAddress, size, granularity, nullptr, 0);
  if(ret != hipSuccess) {
    std::cout << "Failed to Reserve Memory " << std::endl;
  }
  else {
    std::cout << "Successfully reserved " << size << "bytes" << std::endl;  
  }

  hipMemGenericAllocationHandle_t gaHandle = nullptr;
  ret = hipMemCreate(&gaHandle, size, &memAllocationProp, 0);
   if(ret != hipSuccess) {
    auto errnm =  hipGetErrorName(ret);
    std::cout << "Failed to Create Memory Handle " << "error : " << errnm << std::endl;
  }
  else {
    std::cout << "Successfully created Memory Handle " << size << "bytes" << ret << std::endl;  
  }


  ret = hipMemMap(&reservedAddress, size, 0 , gaHandle, 0);
   if(ret != hipSuccess) {
    auto errnm =  hipGetErrorName(ret);
    std::cout << "Failed to Map Memory " << "error : " << errnm << std::endl;
  }
  else {
    std::cout << "Successfully Mapped" << size << " bytes" << std::endl;  
  }

  hipMemAccessDesc desc = {};
  desc.location.id = 0;
  desc.location.type = hipMemLocationTypeDevice;
  desc.flags = hipMemAccessFlagsProtReadWrite;
  std::vector<char> values(size);
  int value = 1;

  ret = hipMemSetAccess(&reservedAddress, size, &desc, 1);
   if(ret != hipSuccess) {
    std::cout << "Failed to Set Memory Access " << std::endl;
  }
  else {
    std::cout << "Successfully Set Access" << size << " bytes" << std::endl;  
  }

  ret = hipMemUnmap(&reservedAddress, size);
   if(ret != hipSuccess) {
   auto errnm =  hipGetErrorName(ret);
    std::cout << "Failed to Unmap Memory " << errnm <<std::endl;
  }
  else {
    std::cout << "Successfully unmapped" << size << "bytes" << std::endl;  
  }


  ret = hipMemRelease(gaHandle);
   if(ret != hipSuccess) {
    std::cout << "Failed to Release Handle " << std::endl;
  }
  else {
    std::cout << "Successfully released Handle" << size << "bytes" << std::endl;  
  }

  ret = hipMemAddressFree(&reservedAddress, size);
   if(ret != hipSuccess) {
    std::cout << "Failed to free Memory " << std::endl;
  }
  else {
    std::cout << "Successfully freed memory" << size << "bytes" << std::endl;  
  }
  ret = hipDeviceReset(); 
  if (ret != hipSuccess) {
    std::cout << "Unable to reset device to flush all data"<< std::endl;
        }
  else {
    std::cout << "Flushing Device reset Successful after failed to free Vmem"<< std::endl;
        }

  return 0;
}

