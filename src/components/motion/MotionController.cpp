#include "components/motion/MotionController.h"

using namespace Pinetime::Controllers;

void MotionController::Update(int16_t x, int16_t y, int16_t z, uint32_t nbSteps, uint32_t nbAcc) {
  if (this->nbSteps != nbSteps && service != nullptr) {
    service->OnNewStepCountValue(nbSteps);
  }

  if(service != nullptr && (this->x != x || this->y != y || this->z != z)) {
    service->OnNewMotionValues(x, y, z);
  }

  uint32_t nbAcc = x*x + y*y + z*z;
  this->nbAcc = nbAcc;
  
  this->x = x;
  this->y = y;
  this->z = z;
  int32_t deltaSteps = nbSteps - this->nbSteps;
  this->nbSteps = nbSteps;
  if (deltaSteps > 0) {
    currentTripSteps += deltaSteps;
  }
}

bool MotionController::ShouldWakeUp(bool isSleeping) {
  if ((x + 335) <= 670 && z < 0) {
    if (not isSleeping) {
      if (y <= 0) {
        return false;
      } else {
        lastYForWakeUp = 0;
        return false;
      }
    }

    if (y >= 0) {
      lastYForWakeUp = 0;
      return false;
    }
    if (y + 230 < lastYForWakeUp) {
      lastYForWakeUp = y;
      return true;
    }
  }
  return false;
}
void MotionController::IsSensorOk(bool isOk) {
  isSensorOk = isOk;
}
void MotionController::Init(Pinetime::Drivers::Bma421::DeviceTypes types) {
  switch (types) {
    case Drivers::Bma421::DeviceTypes::BMA421:
      this->deviceType = DeviceTypes::BMA421;
      break;
    case Drivers::Bma421::DeviceTypes::BMA425:
      this->deviceType = DeviceTypes::BMA425;
      break;
    default:
      this->deviceType = DeviceTypes::Unknown;
      break;
  }
}
void MotionController::SetService(Pinetime::Controllers::MotionService* service) {
  this->service = service;
}
