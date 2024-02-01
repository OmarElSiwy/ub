#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "CommunicationProtocol/tim.h"

typedef enum Orientation {
  FrontLeft,
  FrontRight,
  BackLeft,
  BackRight
} Orientation;

/*
 * Initializes a singular motor with a unique pwm timer to control its speed
 */
void InitializeMotor(Orientation orientation);

void StopMotor(Orientation orientation);
/*
 * @param orientation
 * @param value must be between 0-100
 */
void ChangeSpeed(Orientation orientation, uint8_t value);

#ifdef __cplusplus
}
#endif
