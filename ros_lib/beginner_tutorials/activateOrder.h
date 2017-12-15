#ifndef _ROS_beginner_tutorials_activateOrder_h
#define _ROS_beginner_tutorials_activateOrder_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace beginner_tutorials
{

  class activateOrder : public ros::Msg
  {
    public:
      typedef uint8_t _order_type_type;
      _order_type_type order_type;
      typedef uint8_t _selection_type;
      _selection_type selection;

    activateOrder():
      order_type(0),
      selection(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->order_type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->order_type);
      *(outbuffer + offset + 0) = (this->selection >> (8 * 0)) & 0xFF;
      offset += sizeof(this->selection);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->order_type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->order_type);
      this->selection =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->selection);
     return offset;
    }

    const char * getType(){ return "beginner_tutorials/activateOrder"; };
    const char * getMD5(){ return "b50f5f942b181fcc2f8dc6f4e8ff879c"; };

  };

}
#endif