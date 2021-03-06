/*******************************************************************************
 * Copyright (c) 2015-2018 Skymind, Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/


#ifndef REDUCE_SAME_H
#define REDUCE_SAME_H
#include <dll.h>
//#include <string>
#include <stdio.h>
#include <helpers/shape.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <templatemath.h>
#include <nd4jmalloc.h>
#include <pairwise_util.h>
#include <ops/ops.h>
#include <op_boilerplate.h>

#pragma once
#ifdef __CUDACC__
#include <cuda.h>
#include <cuda_runtime.h>
#endif

#include "legacy_ops.h"

//an op for the kernel
namespace functions {
    namespace reduce {

/**
 * A reduce function
 * reduces a vector down to
 * a subset of itself
 * via aggregating member
 * elements.
 */
        template<typename X>
        class ReduceSameFunction {
        public:
#ifdef __CUDACC__

            template<typename OpType>
            static __device__ void aggregatePartials(void *sPartials, Nd4jLong tid, Nd4jLong numItems, void *extraParams);

            template<typename OpType>
            static __device__ void execScalarCuda( void *vx, Nd4jLong *xShapeInfo, void *extraParams, void *vz, Nd4jLong *zShapeInfo, void *reductionBuffer, Nd4jLong *tadOnlyShapeInfo);

            static __device__ void execScalarCudaLegacy(int opNum, void *vx, Nd4jLong *xShapeInfo, void *extraParams, void *vz, Nd4jLong *zShapeInfo, void *reductionBuffer, Nd4jLong *tadOnlyShapeInfo);

            template<typename OpType>
            static __device__ void transformCudaXD( void *vx, Nd4jLong *xShapeInfo, void *extraParams, void *vz, Nd4jLong *zShapeInfo, int *dimension, int dimensionLength, void *reductionBuffer, Nd4jLong *tadOnlyShapeInfo, Nd4jLong *tadOffsets);

            template<typename OpType>
            static __host__ void intermediateScalar(dim3 launchDims, cudaStream_t *stream, void *vx, Nd4jLong *xShapeInfo, Nd4jLong *hXShapeInfo, void *extraParams, void *vz, Nd4jLong *zShapeInfo, Nd4jLong *hZShapeInfo, int *dimension, int dimensionLength, void *reductionBuffer, Nd4jLong *tadOnlyShapeInfo);

            template<typename OpType>
            static __host__ void intermediateXD(dim3 launchDims, cudaStream_t *stream, void *vx, Nd4jLong *xShapeInfo, Nd4jLong *hXShapeInfo, void *extraParams, void *vz, Nd4jLong *zShapeInfo, Nd4jLong *hZShapeInfo, int *dimension, int dimensionLength, void *reductionPointer, Nd4jLong *tadShapeInfo, Nd4jLong *tadOffsets);

            static __host__ void execReduceScalar(dim3 launchDims, cudaStream_t *stream, int opNum, void *vx, Nd4jLong *xShapeInfo, Nd4jLong* hXShapeInfo, void *extraParams, void *vz, Nd4jLong *zShapeInfo, Nd4jLong* hZShapeInfo, int *dimension, int dimensionLength, void *reductionBuffer, Nd4jLong *tadOnlyShapeInfo);

            static __host__ void execReduceXD(dim3 launchDims, cudaStream_t *stream, int opNum, int rank, void *vx, Nd4jLong *xShapeInfo, Nd4jLong* hXShapeInfo, void *extraParams, void *vz, Nd4jLong *zShapeInfo, Nd4jLong* hZShapeInfo, int *dimension, int dimensionLength, void *reductionPointer, Nd4jLong *tadShapeInfo, Nd4jLong *tadOffsets);
#else

            /**
             * Reduce down to 1 number
             * @param x the input
             * @param xShapeInfo the shape information
             * for the input
             * @param extraParams the extra params
             * @return
             */
            template<typename OpType>
            static _CUDA_H X execScalar(void *x,
                    Nd4jLong *xShapeInfo,
                    void *extraParams);

            template<typename OpType>
            static _CUDA_H void execScalar(void *x,
                                        Nd4jLong *xShapeInfo,
                                        void *extraParams,
                                        void *z,
                                        Nd4jLong *zShapeInfo);


            static X execScalar(int opNum,
                    void *x,
                    Nd4jLong *xShapeInfo,
                    void *extraParams);

            static void execScalar(int opNum,
                                void *x,
                                Nd4jLong *xShapeInfo,
                                void *extraParams,
                                void *z,
                                Nd4jLong *zShapeInfo);

            static void exec(int opNum,
                             void *x,
                             Nd4jLong *xShapeInfo,
                             void *extraParams,
                             void *result,
                             Nd4jLong *resultShapeInfoBuffer,
                             int *dimension,
                             int dimensionLength,
                             Nd4jLong *tadShapeInfo,
                             Nd4jLong *tadOffset, int64_t start, int64_t stop);

            /**
             * Execute on the cpu
             * @param x the input data
             * @param xShapeInfo the shape information for x
             * @param extraParams the extra parameters
             * @param result the result buffer
             * @param resultShapeInfoBuffer the shape information
             * @param dimension the dimension to perform
             * the reduce along long
             * @param dimensionLength the length of the dimension buffer
             */


            template<typename OpType>
            static void _CUDA_H exec(void *x,
                             Nd4jLong *xShapeInfo,
                             void *extraParams,
                             void *result,
                             Nd4jLong *resultShapeInfoBuffer,
                             int *dimension,
                             int dimensionLength,
                             Nd4jLong *tadShapeInfo,
                             Nd4jLong *tadOffset, int64_t start, int64_t stop);

            /**
            * CPU implementation
            * @param x the input data
            * @param xShapeInfo the shape information for
            * the input data
            * @param extraParams the extra parameters for the problem
            * @param result the result buffer
            * @param resultShapeInfo the shape information
            */
            template<typename OpType>
            static void _CUDA_H exec(void *x,
                             Nd4jLong *xShapeInfo,
                             void *extraParams,
                             void *result,
                             Nd4jLong *resultShapeInfo);



            /**
            * Reduce down to 1 number
            * @param x the input
            * @param xShapeInfo the shape information
            * for the input
            * @param extraParams the extra params
            * @return
            */
            template<typename OpType>
            static X _CUDA_H execScalar(void *x,
                    Nd4jLong xElementWiseStride,
                    Nd4jLong length,
                    void *extraParams);

#endif
        };

#ifdef __CUDACC__
        /**
    *
    * @param extraParams
    * @param sPartials
    * @param sMemSize
    */
        template<typename T>
        __device__ void initializeShared(T *extraParams, T **sPartials, int sMemSize);

#endif

    }

}

#endif

