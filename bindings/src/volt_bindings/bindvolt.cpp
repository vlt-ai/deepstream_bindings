/*
 * SPDX-FileCopyrightText: Copyright (c) 2022 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// VoltDataStruct

#include "bind_string_property_definitions.h"
#include "include/bindvolt.hpp"

namespace py = pybind11;

namespace pydeepstream {

    void * copy_volt_struct(void* data, void* user_data) {
        NvDsUserMeta * srcMeta = (NvDsUserMeta*) data;
        VoltDataStruct * srcData = (VoltDataStruct *) srcMeta->user_meta_data;
        VoltDataStruct *destData = (VoltDataStruct *) g_malloc0(
                        sizeof(VoltDataStruct));
        destData->structId = srcData->cameraUUID;
        destData->sampleInt = srcData->producerTimestamp;
        return destData;
    }

    void release_volt_struct(void * data, void * user_data) {
        NvDsUserMeta * srcMeta = (NvDsUserMeta*) data;
        if (srcMeta != nullptr) {
            VoltDataStruct * srcData = (VoltDataStruct *) srcMeta->user_meta_data;
            if (srcData != nullptr) {
                srcData->cameraUUID = "";
                srcData->producerTimestamp = 0;
                
                g_free(srcData);
            }
        }
    }

    void bindvolt(py::module &m) {
                /* VoltDataStruct bindings to be used with NvDsUserMeta */
                py::class_<VoltDataStruct>(m, "VoltDataStruct",
                                pydsdoc::volt::VoltDataStructDoc::descr)
                .def(py::init<>())
                .def_readwrite("cameraUUID", &VoltDataStruct::cameraUUID)
                .def_readwrite("producerTimestamp", &VoltDataStruct::producerTimestamp)

                .def("cast",
                     [](void *data) {
                         return (VoltDataStruct *) data;
                     },
                     py::return_value_policy::reference,
                     pydsdoc::volt::VoltDataStructDoc::cast);

        m.def("alloc_volt_struct",
              [](NvDsUserMeta *meta) {
                  auto *mem = (VoltDataStruct *) g_malloc0(
                          sizeof(VoltDataStruct));
                  meta->base_meta.copy_func = (NvDsMetaCopyFunc) pydeepstream::copy_volt_struct;
                  meta->base_meta.release_func = (NvDsMetaReleaseFunc) pydeepstream::release_volt_struct;
                  return mem;
              },
              py::return_value_policy::reference,
              pydsdoc::methodsDoc::alloc_volt_struct);

    }

}