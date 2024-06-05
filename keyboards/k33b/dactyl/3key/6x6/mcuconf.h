#pragma once
#include_next <mcuconf.h>

// Audio setup
#undef RP_PWM_USE_PWM0
#define RP_PWM_USE_PWM0 TRUE