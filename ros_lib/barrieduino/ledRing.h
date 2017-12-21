#ifndef _ROS_barrieduino_ledRing_h
#define _ROS_barrieduino_ledRing_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "barrieduino/HSL.h"

namespace barrieduino
{

  class ledRing : public ros::Msg
  {
    public:
      typedef uint8_t _ring_type;
      _ring_type ring;
      typedef uint8_t _mode_type;
      _mode_type mode;
      typedef uint16_t _param_type;
      _param_type param;
      typedef barrieduino::HSL _color_type;
      _color_type color;

    ledRing():
      ring(0),
      mode(0),
      param(0),
      color()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->ring >> (8 * 0)) & 0xFF;
      offset += sizeof(this->ring);
      *(outbuffer + offset + 0) = (this->mode >> (8 * 0)) & 0xFF;
      offset += sizeof(this->mode);
      *(outbuffer + offset + 0) = (this->param >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->param >> (8 * 1)) & 0xFF;
      offset += sizeof(this->param);
      offset += this->color.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->ring =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->ring);
      this->mode =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->mode);
      this->param =  ((uint16_t) (*(inbuffer + offset)));
      this->param |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->param);
      offset += this->color.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "barrieduino/ledRing"; };
    const char * getMD5(){ return "795cfd6839d35bdd2a2224705e20ac08"; };

  };

}
#endif