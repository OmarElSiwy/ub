#include "Components/Motor.h"
#include "CommunicationProtocol/tim.h"

static TIM_HandleTypeDef* GetMotor(Orientation orientation) {
  switch(orientation) {
    case FrontLeft:
      return &htim2;
    case FrontRight:
      return &htim3;
    case BackLeft:
      return &htim4;
    case BackRight:
      return &htim5;
  }
}

void InitializeMotor(Orientation orientation) {
  switch(orientation) {
    case FrontLeft:
      MX_TIM2_Init();
      break;
    case FrontRight:
      MX_TIM3_Init();
      break;
    case BackLeft:
      MX_TIM4_Init();
      break;
    case BackRight:
      MX_TIM5_Init();
      break;
  } 

  if (HAL_TIM_PWM_Start(GetMotor(orientation), TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }
}

void StopMotor(Orientation orientation) {
  if (HAL_TIM_PWM_Stop(GetMotor(orientation), TIM_CHANNEL_1) != HAL_OK) {
    Error_Handler();
  }
}

void ChangeSpeed(Orientation orientation, uint8_t speedPercent) {
  if (speedPercent > 100) speedPercent = 100;
  else if (speedPercent < 0) speedPercent = 0;

  TIM_HandleTypeDef* MotorController = GetMotor(orientation);
  
  uint32_t newPulse = (uint32_t)(((MotorController->Init.Period + 1) * speedPercent) / 100) - 1;
  if (newPulse > MotorController->Init.Period) newPulse = MotorController->Init.Period;
  __HAL_TIM_SET_COMPARE(MotorController, TIM_CHANNEL_1, newPulse);
}
