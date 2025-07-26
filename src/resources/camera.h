#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

//���������� ��������� ��������� ����� ��� �������� ������. ���������� ��� ����������� ������� ������� �����
// ���������� enum ��� ������������
enum cameraMovement {
	FORWARD,
	BACKWARD, 
	LEFT,
	RIGHT
};

//�������� ������ �� ����������
const GLfloat YAW = -90.f;
const GLfloat PITCH = 0.f;
const GLfloat SPEED = 3.f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.f;


//����� ��� ������������ ������� �����, ������ � ���������
class Camera {
public:
	glm::vec3 Position;//���������� ������� ������
	glm::vec3 Front;//����������� ������
	glm::vec3 Up;//������ �� ��� y
	glm::vec3 Right;//������ �� ��� x
	glm::vec3 WorldUp;//

	float lastX;
	float lastY;
	//���� ������
	GLfloat Yaw;
	GLfloat Pitch;
	//����� ������
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;
	bool firstMouse = true;
	//����������� ��������
	Camera(const unsigned int& scrWidth, const unsigned int& scrHeight, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
	//����������� ��������� ��������
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);
	//������� ������� ���� 
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	// ������� ��� ������������ ������� ������
	void processKeyboard(cameraMovement direction, GLfloat deltaTime);
	void processMouseMovement(GLfloat xoffset, GLfloat yoffset, float deltaTime, GLboolean constrainPitch = true);
	void processMouseScroll(GLfloat yoffset);
	void setProjectionMatrix(glm::mat4 projectionMatrix) {
		
		this->projectionMatrix = projectionMatrix;
	}
	
private:
	glm::mat4 projectionMatrix;
	void updateCameraVectors();
	
};
#endif // !CAMERA_H
