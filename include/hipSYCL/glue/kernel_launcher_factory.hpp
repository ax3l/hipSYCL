/*
 * This file is part of hipSYCL, a SYCL implementation based on CUDA/HIP
 *
 * Copyright (c) 2019-2020 Aksel Alpay
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef HIPSYCL_KERNEL_LAUNCHER_FACTORY_HPP
#define HIPSYCL_KERNEL_LAUNCHER_FACTORY_HPP

#include <vector>

#include "hipSYCL/sycl/backend/backend.hpp"
#include "hipSYCL/runtime/kernel_launcher.hpp"

#if defined(HIPSYCL_PLATFORM_ROCM) || defined(HIPSYCL_PLATFORM_CUDA)
#include "hip/hip_kernel_launcher.hpp"
#endif

namespace hipsycl {
namespace glue {

template <class KernelNameTag, rt::kernel_type Type, int Dim, class Kernel>
std::vector<std::unique_ptr<rt::backend_kernel_launcher>>
make_kernel_launchers(sycl::id<Dim> offset, sycl::range<Dim> local_range,
                      sycl::range<Dim> global_range,
                      std::size_t dynamic_local_memory, Kernel k) {

  std::vector<std::unique_ptr<rt::backend_kernel_launcher>> launchers;
#if defined(HIPSYCL_PLATFORM_CUDA) || defined(HIPSYCL_PLATFORM_ROCM)
  auto launcher = std::make_unique<hip_kernel_launcher>();
  launcher->bind<KernelNameTag, Type>(offset, local_range, global_range,
                                      dynamic_local_memory, k);
  launchers.emplace_back(launcher);
#endif
  return launchers;
}
}
}

#endif