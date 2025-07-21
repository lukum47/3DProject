#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "resources/camera.h"
#include <iostream>
#include "render/ShaderProgram.h"
#include <sstream>
#include <fstream>
#include "render/texture2D.h"
#include "resources/Model.h"
#include "resources/MyFormat.h"
#include "manager/objectFabric.h"
#include "objects/GameObject.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include <iostream>
#include <memory>

class gameLoop {

private: 
	std::shared_ptr <gameLoop> instance;
	gameLoop() {
	
	
	};

public:
	void initialiazeLoop() {
		if (instance.get() == nullptr)
		{
			instance = std::make_shared<gameLoop>();
		}
	}
	std::weak_ptr<gameLoop> getInstance() {
		return instance;
	}

};