/**
  ******************************************************************************
  * @file    network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Tue Nov 29 11:38:51 2022
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "network.h"
#include "network_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_network
 
#undef AI_NETWORK_MODEL_SIGNATURE
#define AI_NETWORK_MODEL_SIGNATURE     "1eb2760747c27be1f84de83f759d268e"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Tue Nov 29 11:38:51 2022"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_N_BATCHES
#define AI_NETWORK_N_BATCHES         (1)

static ai_ptr g_network_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_network_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  _classifier_classifier_0_Gemm_output_0_weights_array, AI_ARRAY_FORMAT_LUT4_FLOAT,
  NULL, NULL, 645120, AI_STATIC)
/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  _classifier_classifier_0_Gemm_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 84, AI_STATIC)
/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  output_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 840, AI_STATIC)
/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  output_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 10, AI_STATIC)
/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  _feature_extractor_feature_extractor_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 384, AI_STATIC)
/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  _feature_extractor_feature_extractor_3_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)
/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  input_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 3072, AI_STATIC)
/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  input_Transpose_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3072, AI_STATIC)
/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  _feature_extractor_feature_extractor_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1536, AI_STATIC)
/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  _feature_extractor_feature_extractor_3_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1024, AI_STATIC)
/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  _feature_extractor_feature_extractor_6_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 7680, AI_STATIC)
/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  _Flatten_output_0_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 7680, AI_STATIC)
/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  _classifier_classifier_0_Gemm_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 84, AI_STATIC)
/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  _classifier_classifier_1_Relu_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 84, AI_STATIC)
/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  output_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 10, AI_STATIC)
/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  _feature_extractor_feature_extractor_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 450, AI_STATIC)
/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  _feature_extractor_feature_extractor_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6, AI_STATIC)
/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  _feature_extractor_feature_extractor_3_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2400, AI_STATIC)
/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  _feature_extractor_feature_extractor_3_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)
/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  _feature_extractor_feature_extractor_6_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48000, AI_STATIC)
/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  _feature_extractor_feature_extractor_6_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 120, AI_STATIC)
/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  _classifier_classifier_0_Gemm_output_0_weights, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 7680, 84, 1, 1), AI_STRIDE_INIT(4, 1, 3840, 322560, 322560),
  1, &_classifier_classifier_0_Gemm_output_0_weights_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  _classifier_classifier_0_Gemm_output_0_bias, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 84, 1, 1), AI_STRIDE_INIT(4, 4, 4, 336, 336),
  1, &_classifier_classifier_0_Gemm_output_0_bias_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  output_weights, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 84, 10, 1, 1), AI_STRIDE_INIT(4, 4, 336, 3360, 3360),
  1, &output_weights_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  output_bias, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 10, 1, 1), AI_STRIDE_INIT(4, 4, 4, 40, 40),
  1, &output_bias_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  _feature_extractor_feature_extractor_0_Conv_output_0_scratch0, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 6, 32, 2), AI_STRIDE_INIT(4, 4, 4, 24, 768),
  1, &_feature_extractor_feature_extractor_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  _feature_extractor_feature_extractor_3_Conv_output_0_scratch0, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 16, 2), AI_STRIDE_INIT(4, 4, 4, 64, 1024),
  1, &_feature_extractor_feature_extractor_3_Conv_output_0_scratch0_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  input_output, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 32, 3), AI_STRIDE_INIT(4, 4, 4, 128, 4096),
  1, &input_output_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  input_Transpose_output, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 32, 32), AI_STRIDE_INIT(4, 4, 4, 12, 384),
  1, &input_Transpose_output_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  _feature_extractor_feature_extractor_0_Conv_output_0_output, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 6, 16, 16), AI_STRIDE_INIT(4, 4, 4, 24, 384),
  1, &_feature_extractor_feature_extractor_0_Conv_output_0_output_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  _feature_extractor_feature_extractor_3_Conv_output_0_output, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 8, 8), AI_STRIDE_INIT(4, 4, 4, 64, 512),
  1, &_feature_extractor_feature_extractor_3_Conv_output_0_output_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  _feature_extractor_feature_extractor_6_Conv_output_0_output, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 8, 8), AI_STRIDE_INIT(4, 4, 4, 480, 3840),
  1, &_feature_extractor_feature_extractor_6_Conv_output_0_output_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  _Flatten_output_0_to_chlast_output, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 8, 120), AI_STRIDE_INIT(4, 4, 4, 32, 256),
  1, &_Flatten_output_0_to_chlast_output_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  _Flatten_output_0_to_chlast_output0, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 7680, 1, 1), AI_STRIDE_INIT(4, 4, 4, 30720, 30720),
  1, &_Flatten_output_0_to_chlast_output_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  _classifier_classifier_0_Gemm_output_0_output, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 84, 1, 1), AI_STRIDE_INIT(4, 4, 4, 336, 336),
  1, &_classifier_classifier_0_Gemm_output_0_output_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  _classifier_classifier_1_Relu_output_0_output, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 1, 84, 1, 1), AI_STRIDE_INIT(4, 4, 4, 336, 336),
  1, &_classifier_classifier_1_Relu_output_0_output_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  output_output, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 10, 1, 1), AI_STRIDE_INIT(4, 4, 4, 40, 40),
  1, &output_output_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  _feature_extractor_feature_extractor_0_Conv_output_0_weights, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 3, 5, 5, 6), AI_STRIDE_INIT(4, 4, 12, 60, 300),
  1, &_feature_extractor_feature_extractor_0_Conv_output_0_weights_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  _feature_extractor_feature_extractor_0_Conv_output_0_bias, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 6, 1, 1), AI_STRIDE_INIT(4, 4, 4, 24, 24),
  1, &_feature_extractor_feature_extractor_0_Conv_output_0_bias_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  _feature_extractor_feature_extractor_3_Conv_output_0_weights, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 6, 5, 5, 16), AI_STRIDE_INIT(4, 4, 24, 120, 600),
  1, &_feature_extractor_feature_extractor_3_Conv_output_0_weights_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  _feature_extractor_feature_extractor_3_Conv_output_0_bias, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_feature_extractor_feature_extractor_3_Conv_output_0_bias_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  _feature_extractor_feature_extractor_6_Conv_output_0_weights, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 16, 5, 5, 120), AI_STRIDE_INIT(4, 4, 64, 320, 1600),
  1, &_feature_extractor_feature_extractor_6_Conv_output_0_weights_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  _feature_extractor_feature_extractor_6_Conv_output_0_bias, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 1, 1), AI_STRIDE_INIT(4, 4, 4, 480, 480),
  1, &_feature_extractor_feature_extractor_6_Conv_output_0_bias_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  output_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_classifier_classifier_1_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &output_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &output_weights, &output_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  output_layer, 12,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &output_chain,
  NULL, &output_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _classifier_classifier_1_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_classifier_classifier_0_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_classifier_classifier_1_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _classifier_classifier_1_Relu_output_0_layer, 11,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &_classifier_classifier_1_Relu_output_0_chain,
  NULL, &output_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _classifier_classifier_0_Gemm_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_Flatten_output_0_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_classifier_classifier_0_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_classifier_classifier_0_Gemm_output_0_weights, &_classifier_classifier_0_Gemm_output_0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _classifier_classifier_0_Gemm_output_0_layer, 10,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &_classifier_classifier_0_Gemm_output_0_chain,
  NULL, &_classifier_classifier_1_Relu_output_0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _Flatten_output_0_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feature_extractor_feature_extractor_6_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_Flatten_output_0_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _Flatten_output_0_to_chlast_layer, 9,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &_Flatten_output_0_to_chlast_chain,
  NULL, &_classifier_classifier_0_Gemm_output_0_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feature_extractor_feature_extractor_6_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feature_extractor_feature_extractor_3_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feature_extractor_feature_extractor_6_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feature_extractor_feature_extractor_6_Conv_output_0_weights, &_feature_extractor_feature_extractor_6_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feature_extractor_feature_extractor_6_Conv_output_0_layer, 8,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d,
  &_feature_extractor_feature_extractor_6_Conv_output_0_chain,
  NULL, &_Flatten_output_0_to_chlast_layer, AI_STATIC, 
  .groups = 1, 
  .nl_params = NULL, 
  .nl_func = nl_func_relu_array_f32, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 2, 2, 2, 2), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feature_extractor_feature_extractor_3_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feature_extractor_feature_extractor_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feature_extractor_feature_extractor_3_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feature_extractor_feature_extractor_3_Conv_output_0_weights, &_feature_extractor_feature_extractor_3_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feature_extractor_feature_extractor_3_Conv_output_0_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  _feature_extractor_feature_extractor_3_Conv_output_0_layer, 6,
  OPTIMIZED_CONV2D_TYPE, 0x0, NULL,
  conv2d_nl_pool, forward_conv2d_nl_pool,
  &_feature_extractor_feature_extractor_3_Conv_output_0_chain,
  NULL, &_feature_extractor_feature_extractor_6_Conv_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .nl_params = NULL, 
  .nl_func = nl_func_relu_array_f32, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 2, 2, 2, 2), 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .pool_func = pool_func_mp_array_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feature_extractor_feature_extractor_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_Transpose_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feature_extractor_feature_extractor_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feature_extractor_feature_extractor_0_Conv_output_0_weights, &_feature_extractor_feature_extractor_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feature_extractor_feature_extractor_0_Conv_output_0_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  _feature_extractor_feature_extractor_0_Conv_output_0_layer, 3,
  OPTIMIZED_CONV2D_TYPE, 0x0, NULL,
  conv2d_nl_pool, forward_conv2d_nl_pool,
  &_feature_extractor_feature_extractor_0_Conv_output_0_chain,
  NULL, &_feature_extractor_feature_extractor_3_Conv_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .nl_params = NULL, 
  .nl_func = nl_func_relu_array_f32, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 2, 2, 2, 2), 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .pool_func = pool_func_mp_array_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  input_Transpose_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_Transpose_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  input_Transpose_layer, 2,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &input_Transpose_chain,
  NULL, &_feature_extractor_feature_extractor_0_Conv_output_0_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 530328, 1, 1),
    530328, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 61440, 1, 1),
    61440, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &output_output),
  &input_Transpose_layer, 0, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 530328, 1, 1),
      530328, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 61440, 1, 1),
      61440, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &output_output),
  &input_Transpose_layer, 0, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/


/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_network_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    input_output_array.data = AI_PTR(g_network_activations_map[0] + 16896);
    input_output_array.data_start = AI_PTR(g_network_activations_map[0] + 16896);
    
    input_Transpose_output_array.data = AI_PTR(g_network_activations_map[0] + 29184);
    input_Transpose_output_array.data_start = AI_PTR(g_network_activations_map[0] + 29184);
    
    _feature_extractor_feature_extractor_0_Conv_output_0_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 16896);
    _feature_extractor_feature_extractor_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 16896);
    
    _feature_extractor_feature_extractor_0_Conv_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 18432);
    _feature_extractor_feature_extractor_0_Conv_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 18432);
    
    _feature_extractor_feature_extractor_3_Conv_output_0_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 24576);
    _feature_extractor_feature_extractor_3_Conv_output_0_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 24576);
    
    _feature_extractor_feature_extractor_3_Conv_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 26624);
    _feature_extractor_feature_extractor_3_Conv_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 26624);
    
    _feature_extractor_feature_extractor_6_Conv_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 30720);
    _feature_extractor_feature_extractor_6_Conv_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 30720);
    
    _Flatten_output_0_to_chlast_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    _Flatten_output_0_to_chlast_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    
    _classifier_classifier_0_Gemm_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 30720);
    _classifier_classifier_0_Gemm_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 30720);
    
    _classifier_classifier_1_Relu_output_0_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    _classifier_classifier_1_Relu_output_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    
    output_output_array.data = AI_PTR(g_network_activations_map[0] + 336);
    output_output_array.data_start = AI_PTR(g_network_activations_map[0] + 336);
    
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_network_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    _classifier_classifier_0_Gemm_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _classifier_classifier_0_Gemm_output_0_weights_array.data = AI_PTR(g_network_weights_map[0] + 64);
    _classifier_classifier_0_Gemm_output_0_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 0);
    
    _classifier_classifier_0_Gemm_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _classifier_classifier_0_Gemm_output_0_bias_array.data = AI_PTR(g_network_weights_map[0] + 322624);
    _classifier_classifier_0_Gemm_output_0_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 322624);
    
    output_weights_array.format |= AI_FMT_FLAG_CONST;
    output_weights_array.data = AI_PTR(g_network_weights_map[0] + 322960);
    output_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 322960);
    
    output_bias_array.format |= AI_FMT_FLAG_CONST;
    output_bias_array.data = AI_PTR(g_network_weights_map[0] + 326320);
    output_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 326320);
    
    _feature_extractor_feature_extractor_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feature_extractor_feature_extractor_0_Conv_output_0_weights_array.data = AI_PTR(g_network_weights_map[0] + 326360);
    _feature_extractor_feature_extractor_0_Conv_output_0_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 326360);
    
    _feature_extractor_feature_extractor_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feature_extractor_feature_extractor_0_Conv_output_0_bias_array.data = AI_PTR(g_network_weights_map[0] + 328160);
    _feature_extractor_feature_extractor_0_Conv_output_0_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 328160);
    
    _feature_extractor_feature_extractor_3_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feature_extractor_feature_extractor_3_Conv_output_0_weights_array.data = AI_PTR(g_network_weights_map[0] + 328184);
    _feature_extractor_feature_extractor_3_Conv_output_0_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 328184);
    
    _feature_extractor_feature_extractor_3_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feature_extractor_feature_extractor_3_Conv_output_0_bias_array.data = AI_PTR(g_network_weights_map[0] + 337784);
    _feature_extractor_feature_extractor_3_Conv_output_0_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 337784);
    
    _feature_extractor_feature_extractor_6_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feature_extractor_feature_extractor_6_Conv_output_0_weights_array.data = AI_PTR(g_network_weights_map[0] + 337848);
    _feature_extractor_feature_extractor_6_Conv_output_0_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 337848);
    
    _feature_extractor_feature_extractor_6_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feature_extractor_feature_extractor_6_Conv_output_0_bias_array.data = AI_PTR(g_network_weights_map[0] + 529848);
    _feature_extractor_feature_extractor_6_Conv_output_0_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 529848);
    
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/


AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_network_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 4821640,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_bool ai_network_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 4821640,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}

AI_API_ENTRY
ai_error ai_network_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}

AI_API_ENTRY
ai_error ai_network_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    &AI_NET_OBJ_INSTANCE,
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}

AI_API_ENTRY
ai_error ai_network_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
    ai_error err;
    ai_network_params params;

    err = ai_network_create(network, AI_NETWORK_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE)
        return err;
    if (ai_network_data_params_get(&params) != true) {
        err = ai_network_get_error(*network);
        return err;
    }
#if defined(AI_NETWORK_DATA_ACTIVATIONS_COUNT)
    if (activations) {
        /* set the addresses of the activations buffers */
        for (int idx=0;idx<params.map_activations.size;idx++)
            AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
    }
#endif
#if defined(AI_NETWORK_DATA_WEIGHTS_COUNT)
    if (weights) {
        /* set the addresses of the weight buffers */
        for (int idx=0;idx<params.map_weights.size;idx++)
            AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
    }
#endif
    if (ai_network_init(*network, &params) != true) {
        err = ai_network_get_error(*network);
    }
    return err;
}

AI_API_ENTRY
ai_buffer* ai_network_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    ((ai_network *)network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}

AI_API_ENTRY
ai_buffer* ai_network_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    ((ai_network *)network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}

AI_API_ENTRY
ai_handle ai_network_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}

AI_API_ENTRY
ai_bool ai_network_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = ai_platform_network_init(network, params);
  if (!net_ctx) return false;

  ai_bool ok = true;
  ok &= network_configure_weights(net_ctx, params);
  ok &= network_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_network_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}

AI_API_ENTRY
ai_i32 ai_network_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_NETWORK_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

