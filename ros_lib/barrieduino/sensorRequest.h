#ifndef _ROS_SERVICE_sensorRequest_h
#define _ROS_SERVICE_sensorRequest_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace barrieduino
{

static const char SENSORREQUEST[] = "barrieduino/sensorRequest";

  class sensorRequestRequest : public ros::Msg
  {
    public:
      typedef const char* _sensor_type;
      _sensor_type sensor;

    sensorRequestRequest():
      sensor("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_sensor = strlen(this->sensor);
      varToArr(outbuffer + offset, length_sensor);
      offset += 4;
      memcpy(outbuffer + offset, this->sensor, length_sensor);
      offset += length_sensor;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_sensor;
      arrToVar(length_sensor, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_sensor; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_sensor-1]=0;
      this->sensor = (char *)(inbuffer + offset-1);
      offset += length_sensor;
     return offset;
    }

    const char * getType(){ return SENSORREQUEST; };
    const char * getMD5(){ return "c6599601f6241c1bc9e4676bbeb31d89"; };

  };

  class sensorRequestResponse : public ros::Msg
  {
    public:
      typedef uint8_t _status_type;
      _status_type status;
      typedef float _value_type;
      _value_type value;

    sensorRequestResponse():
      status(0),
      value(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->status >> (8 * 0)) & 0xFF;
      offset += sizeof(this->status);
      union {
        float real;
        uint32_t base;
      } u_value;
      u_value.real = this->value;
      *(outbuffer + offset + 0) = (u_value.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_value.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_value.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_value.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->value);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->status =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->status);
      union {
        float real;
        uint32_t base;
      } u_value;
      u_value.base = 0;
      u_value.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_value.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_value.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_value.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->value = u_value.real;
      offset += sizeof(this->value);
     return offset;
    }

    const char * getType(){ return SENSORREQUEST; };
    const char * getMD5(){ return "2de371c8cae8c6b1ee3a0d70771317a5"; };

  };

  class sensorRequest {
    public:
    typedef sensorRequestRequest Request;
    typedef sensorRequestResponse Response;
  };

}
#endif
