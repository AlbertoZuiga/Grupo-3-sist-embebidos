/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

/*
 * SPDX-FileCopyrightText: 2019-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "detection_responder.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "model_settings.h"

#include "esp_main.h"

void RespondToDetection(TfLiteTensor* output) {
    if (output->type != kTfLiteFloat32) {
        MicroPrintf("Error: El tensor de salida no es de tipo float32.");
        return;
    }
    if (output->dims->size != 2 || output->dims->data[1] != kCategoryCount) {
        MicroPrintf("Error: El tensor de salida tiene una forma incorrecta.");
        return;
    }

    float* scores = output->data.f;

    // Calcula el porcentaje de cada clase
    int percentages[kCategoryCount];
    float total_score = 0.0f;
    for (int i = 0; i < kCategoryCount; ++i) {
        total_score += scores[i];
    }
    for (int i = 0; i < kCategoryCount; ++i) {
        percentages[i] = static_cast<int>((scores[i] / total_score) * 100 + 0.5);
    }

    // Muestra los porcentajes
    MicroPrintf("Scores for each class:\n");
    for (int i = 0; i < kCategoryCount; ++i) {
        MicroPrintf("Class %d: %d%%", i, percentages[i]);
    }
}
