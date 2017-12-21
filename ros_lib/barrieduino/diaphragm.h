#ifndef _ROS_barrieduino_diaphragm_h
#define _ROS_barrieduino_diaphragm_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace barrieduino
{

  class diaphragm : public ros::Msg
  {
    public:
      typedef uint8_t _diaphragm_type;
      _diaphragm_type diaphragm;
      typedef bool _position_type;
      _position_type position;

    diaphragm():
      diaphragm(0),
      position(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->diaphragm >> (8 * 0)) & 0xFF;
      offset += sizeof(this->diaphragm);
      union {
        bool real;
        uint8_t base;
      } u_position;
      u_position.real = this->position;
      *(outbuffer + offset + 0) = (u_position.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->position);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->diaphragm =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->diaphragm);
      union {
        bool real;
        uint8_t base;
      } u_position;
      u_position.base = 0;
      u_position.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->position = u_position.real;
      offset += sizeof(this->position);
     return offset;
    }

    const char * getType(){ return "barrieduino/diaphragm"; };
    const char * getMD5(){ return "08b2df51c5da052e029df1d8aefa23a2"; };

  };

}
#endif