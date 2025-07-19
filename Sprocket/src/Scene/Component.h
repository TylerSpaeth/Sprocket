#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Macros.h"

namespace Sprocket {

  class SPROCKET_API Component {

    public:
      // This is needed to allow dynamic cast checking against Component pointers
      virtual ~Component() = default;

  };

}

#endif