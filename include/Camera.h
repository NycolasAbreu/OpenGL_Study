#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

enum CameraMovement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

// Default camera values
constexpr double YAW = -90.0;
constexpr double PITCH = 0.0;
constexpr double SPEED = 2.5;
constexpr double SENSITIVITY = 0.1;
constexpr double ZOOM = 45.0;

//--------------------------------------------------------------------------------------------

class Camera
{
  public:
    Camera(
      glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
      double yaw = YAW,
      double pitch = PITCH
    );

    ~Camera() = default;

    glm::mat4 GetViewMatrix() const;

    void ProcessKeyboard(
      CameraMovement direction,
      double deltaTime
    );

    void ProcessMouseMovement(
      double xOffset,
      double yOffset,
      GLboolean constrainPitch = true
    );

    void ProcessMouseScroll(
      double yOffset
    );

    double GetZoom() const;

    glm::vec3 GetPosition() const;

  private:
    void UpdateCameraVectors();

    // camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // euler Angles
    double yaw;
    double pitch;

    // camera options
    double movementSpeed;
    double mouseSensitivity;
    double zoom;
};

//--------------------------------------------------------------------------------------------

#endif