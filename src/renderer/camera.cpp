#include "camera.hpp"
#include <bx/math.h>

camera::camera(): m_position({0.0f, 0.0f, 0.0f}), m_target({0.0f, 0.0f, 0.0f}) {
    bx::mtxIdentity(m_view);
    bx::mtxIdentity(m_proj);
    set_position({0.0f, 0.0f, -5.0f});
    look_at({0.0f, 0.0f, 0.0f});
}

void camera::set_position(const bx::Vec3 &position) {
    m_position = position;
    update_view_matrix();
}

void camera::look_at(const bx::Vec3 &target) {
    m_target = target;
    update_view_matrix();
}

void camera::update_view_matrix() {
    bx::mtxLookAt(m_view, m_position, m_target);
}

void camera::set_perspective(float fov, float aspect, float nearF, float farF) {
    bx::mtxProj(m_proj, fov, aspect, nearF, farF, bgfx::getCaps()->homogeneousDepth);
}

void camera::move_forward(float delta) {
    bx::Vec3 forward = bx::normalize(bx::sub(m_target, m_position));
    m_position = bx::add(m_position, bx::mul(forward, delta));
    m_target = bx::add(m_target, bx::mul(forward, delta));
    update_view_matrix();
}

void camera::move_backward(float delta) {
    bx::Vec3 backward = bx::normalize(bx::sub(m_position, m_target));
    m_position = bx::add(m_position, bx::mul(backward, delta));
    m_target = bx::add(m_target, bx::mul(backward, delta));
    update_view_matrix();
}

void camera::move_left(float delta) {
    bx::Vec3 forward = bx::normalize(bx::sub(m_target, m_position));
    bx::Vec3 left = bx::cross({0.0f, 1.0f, 0.0f}, forward);
    m_position = bx::add(m_position, bx::mul(left, delta));
    m_target = bx::add(m_target, bx::mul(left, delta));
    update_view_matrix();
}

void camera::move_right(float delta) {
    bx::Vec3 forward = bx::normalize(bx::sub(m_target, m_position));
    bx::Vec3 right = bx::cross(forward, {0.0f, 1.0f, 0.0f});
    m_position = bx::add(m_position, bx::mul(right, delta));
    m_target = bx::add(m_target, bx::mul(right, delta));
    update_view_matrix();
}

void camera::move_up(float delta) {
    m_position.y += delta;
    m_target.y += delta;
    update_view_matrix();
}

void camera::move_down(float delta) {
    m_position.y -= delta;
    m_target.y -= delta;
    update_view_matrix();
}

const float *camera::get_view_matrix() const {
    return m_view;
}

const float *camera::get_proj_matrix() const {
    return m_proj;
}
