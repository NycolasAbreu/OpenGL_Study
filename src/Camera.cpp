#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <vector>

//--------------------------------------------------------------------------------------------

Camera::Camera(
  glm::vec3 position,
  glm::vec3 up,
  double yaw,
  double pitch
) : position(position),
    worldUp(up),
    yaw(yaw),
    pitch(pitch)
{
  front = glm::vec3(0.0f, 0.0f, -1.0f);
  movementSpeed = SPEED;
  mouseSensitivity = SENSITIVITY;
  zoom = ZOOM;
  UpdateCameraVectors();
}

//--------------------------------------------------------------------------------------------

glm::mat4 Camera::GetViewMatrix() const
{
  return glm::lookAt(position, position + front, up);
}

//--------------------------------------------------------------------------------------------

double Camera::GetZoom() const
{
  return zoom;
}

//--------------------------------------------------------------------------------------------

glm::vec3 Camera::GetPosition() const
{
  return position;
}

//--------------------------------------------------------------------------------------------

void Camera::ProcessKeyboard(
  CameraMovement direction,
  double deltaTime
)
{
  const auto velocity = (float)(movementSpeed * deltaTime);
  switch (direction)
  {
    case FORWARD:
      position += front * velocity;
      break;
    case BACKWARD:
      position -= front * velocity;
      break;
    case LEFT:
      position -= right * velocity;
      break;
    case RIGHT:
      position += right * velocity;
      break;
    default:
      break;
  }
}

//--------------------------------------------------------------------------------------------

void Camera::ProcessMouseMovement(
  double xOffset,
  double yOffset,
  GLboolean constrainPitch
)
{
  xOffset *= mouseSensitivity;
  yOffset *= mouseSensitivity;

  yaw += xOffset;
  pitch += yOffset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch)
  {
    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;
  }

  // update Front, Right and Up Vectors using the updated Euler angles
  UpdateCameraVectors();
}

//--------------------------------------------------------------------------------------------

void Camera::ProcessMouseScroll(
  double yOffset
)
{
  zoom -= yOffset;
  if (zoom < 1.0f) {
    zoom = 1.0f;
  }
  if (zoom > 45.0f) {
    zoom = 45.0f;
  }
}

//--------------------------------------------------------------------------------------------

void Camera::UpdateCameraVectors()
{
  // calculate the new Front vector
  glm::vec3 newFront;
  newFront.x = cos(glm::radians((float)yaw)) * cos(glm::radians((float)pitch));
  newFront.y = sin(glm::radians((float)pitch));
  newFront.z = sin(glm::radians((float)yaw)) * cos(glm::radians((float)pitch));
  
  front = glm::normalize(newFront);

  // also re-calculate the Right and Up vector
  right = glm::normalize(glm::cross(newFront, worldUp));
  up = glm::normalize(glm::cross(right, newFront));
}

//--------------------------------------------------------------------------------------------
