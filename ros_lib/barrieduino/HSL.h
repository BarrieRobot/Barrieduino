#ifndef _ROS_barrieduino_HSL_h
#define _ROS_barrieduino_HSL_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace barrieduino
{

  class HSL : public ros::Msg
  {
    public:
      typedef uint8_t _hue_type;
      _hue_type hue;
      typedef uint8_t _sat_type;
      _sat_type sat;
      typedef uint8_t _val_type;
      _val_type val;

    HSL():
      hue(0),
      sat(0),
      val(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->hue >> (8 * 0)) & 0xFF;
      offset += sizeof(this->hue);
      *(outbuffer + offset + 0) = (this->sat >> (8 * 0)) & 0xFF;
      offset += sizeof(this->sat);
      *(outbuffer + offset + 0) = (this->val >> (8 * 0)) & 0xFF;
      offset += sizeof(this->val);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->hue =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->hue);
      this->sat =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->sat);
      this->val =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->val);
     return offset;
    }

    const char * getType(){ return "barrieduino/HSL"; };
    const char * getMD5(){ return "e4eea4d46452f5eaf34611db054739d7"; };

  };

}
#endif