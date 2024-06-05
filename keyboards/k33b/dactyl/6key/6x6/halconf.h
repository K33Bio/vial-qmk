#pragma once
#include_next <halconf.h>

// Audio setup
#undef HAL_USE_PWM
#define HAL_USE_PWM TRUE