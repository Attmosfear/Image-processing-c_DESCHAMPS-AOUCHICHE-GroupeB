//
// Created by desch on 22/05/2025.
//

#ifndef KERNEL_CHARGER_H
#define KERNEL_CHARGER_H

#include <stdio.h>
#include <stdlib.h>

float** create_kernel(int size);
void free_kernel(float **kernel, int size);
float** get_box_blur_kernel();
float** get_gaussian_blur_kernel();
float** get_outline_kernel();
float** get_emboss_kernel();
float** get_sharpen_kernel();

#endif //KERNEL_CHARGER_H
