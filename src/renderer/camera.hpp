#pragma once
#include <bx/math.h>

class camera {
public:
    camera();

    void set_position(const bx::Vec3 &position);

    void look_at(const bx::Vec3 &target);

    void set_perspective(float fov, float aspect, float nearPos, float farPos);

    [[nodiscard]] const float *get_view_matrix() const;

    [[nodiscard]] const float *get_proj_matrix() const;

    void update_view_matrix();

    void move_forward(float delta);

    void move_backward(float delta);

    void move_left(float delta);

    void move_right(float delta);

    void move_up(float delta);

    void move_down(float delta);

private:
    float m_view[16];
    float m_proj[16];
    bx::Vec3 m_position;
    bx::Vec3 m_target;
};
