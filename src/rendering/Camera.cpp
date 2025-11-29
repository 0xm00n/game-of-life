#include "rendering/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float aspect_ratio)
    : pos(0.0f, 0.0f), zoom(10.0f), aspect_ratio(aspect_ratio) {}

glm::mat4 Camera::get_matrix() const {
    float half_width = zoom * aspect_ratio;
    float half_height = zoom;

    return glm::ortho(
        pos.x - half_width,     // left
        pos.x + half_width,     // right
        pos.y - half_height,    // bottom
        pos.y + half_height,    // top
        -1.0f,                  // near
        1.0f                    // far
    );
}

void Camera::pan(float dx, float dy) {
    pos.x += dx;
    pos.y += dy;
}

void Camera::adjust_zoom(float factor) {
    zoom *= factor;
    // clamp
    if (zoom < 1.0f) zoom = 1.0f;
    if (zoom > 1000.0f) zoom = 1000.0f;
}