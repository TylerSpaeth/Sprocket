#ifndef SCRIPTABLECOMPONENT_H
#define SCRIPTABLECOMPONENT_H

#include "Core/Macros.h"

namespace Sprocket {

  class SPROCKET_API ScriptableComponent {
    friend class Scene;
    private:
      
    protected:
      int entityID = -1;
      virtual void OnCreate() {}
      virtual void OnDestroy() {}
      virtual void OnUpdate(float deltaTime) {}
    public:
      virtual ~ScriptableComponent() {}
      // TODO add all functions that scripts should be able to use here
  };

}

#endif