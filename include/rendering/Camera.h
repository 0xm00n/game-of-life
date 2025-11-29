#pragma once
#include <glm/glm.hpp>

class Camera {
private:
    glm::vec2 pos;
    float zoom;
    float aspect_ratio;

public:
    Camera(float aspect_ratio);

    glm::mat4 get_matrix() const;

    void pan(float dx, float dy);
    void adjust_zoom(float factor);

    float get_zoom() const { return zoom; }
    
    void set_aspect_ratio(float ratio) { aspect_ratio = ratio; }
};