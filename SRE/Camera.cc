//
// Created by morten on 01/08/16.
//

#include "Camera.h"

#if defined(_WIN32)
#   define GLEW_STATIC
#   include <GL/glew.h>
#else
#   include <OpenGL/gl3.h>
#endif

#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>

namespace SRE{
    Camera::Camera()
    :projectionTransform {1.0f}, viewTransform{1.0f}
    {
    }

    void Camera::setPerspectiveProjection(float fieldOfViewY, float viewportWidth,float viewportHeight, float nearPlane, float farPlane) {
        projectionTransform = glm::perspectiveFov<float>	(	glm::radians( fieldOfViewY),
                                                          viewportWidth,
                                                          viewportHeight,
                                                          nearPlane,
                                                          farPlane);
    }

    void Camera::setOrthographicProjection(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
        projectionTransform = glm::ortho<float>	(left, right, bottom, top, nearPlane, farPlane);
    }

    void Camera::lookAt(glm::vec3 eye, glm::vec3 at, glm::vec3 up) {
        viewTransform = glm::lookAt<float>(eye, at, up);
    }

    glm::mat4 Camera::getViewTransform() {
        return viewTransform;
    }

    glm::mat4 Camera::getProjectionTransform() {
        return projectionTransform;
    }

    void Camera::setViewTransform(const glm::mat4 &viewTransform) {
        Camera::viewTransform = viewTransform;
    }

    void Camera::setProjectionTransform(const glm::mat4 &projectionTransform) {
        Camera::projectionTransform = projectionTransform;
    }

    void Camera::setViewport(int x, int y, int width, int height) {
        viewportX = x;
        viewportY = y;
        viewportWidth = width;
        viewportHeight = height;
        if (Renderer::instance->getCamera() == this){
            glViewport(x, y, width, height);
            glScissor(x, y, width, height);
        }
    }
}