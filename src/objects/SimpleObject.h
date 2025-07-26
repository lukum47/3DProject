#pragma once
#include <objects/GameObject.h>


struct ObjectBuffers;

using BuffersPtr = std::shared_ptr<ObjectBuffers>;
using BuffersRef = std::weak_ptr<ObjectBuffers>;

class SimpleObject : public GameObject {

protected:
    
    BuffersPtr instanceBuffers;
    int instancePositionID;
public:

 enum class simpleObjectType : std::uint8_t 
        {
            OBJECT_CUBE
        };
 auto getInstancePositionID() -> int {
        return instancePositionID;
    }
 void setInstancePositionID(int ID) {
    instancePositionID = ID;
 }
};