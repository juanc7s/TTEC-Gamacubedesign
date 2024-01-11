#include "Arduino.h"

enum PROTOCOLS{
  QUERY_STATUS,
  QUERY_IMAGING_DATA,
  SET_OPERATION
};  

enum PROTOCOL_QUERY_STATUS{
  GS_QUERY_STATUS_GREETING,
  SATELLITE_SEND_STATUS,
  GS_QUERY_STATUS_DONE,
  SATELLITE_QUERY_STATUS_DONE
};

enum PROTOCOL_QUERY_IMAGING_DATA{
  GS_QUERY_IMAGING_GREETING,
  SATELLITE_PACKETS_AVAILABLE,
  GS_START_TELEMETRY_TRANSMISSION,
  SATELLITE_SEND_PACKETS,
  GS_RESEND_STATUS,
  SATELLITE_QUERY_IMAGING_DONE,
  GS_QUERY_IMAGING_DONE
};

enum PROTOCOL_SET_OPERATION{
    GS_SEND_OPERATION,
    SATELLITE_REPEAT_OPERATION,
    GS_SET_OPERATION_DONE,
    SATELLITE_SET_OPERATION_DONE
};

struct ProtocolOperation{
    unsigned protocol : 2;
    unsigned operation : 6;
};

struct GSCommand{
  unsigned switch_active_thermal_control : 1;
  unsigned switch_attitude_control : 1;
  unsigned switch_imaging : 1;
  unsigned switch_imaging_mode : 1;
  unsigned switch_stand_by_mode : 1;
};

// struct GSCommand{
//   uint8_t length;
//   GSCommand_b1 b1;
// };

struct GSResend{
  bool isDone;
  uint8_t packets[32];
};

union GSData{
  GSCommand command;
  GSResend resend;
};

struct GSPacket{
  uint8_t length;
  ProtocolOperation operation;
  GSData data;
};

#define N 10 // Size of the added array
struct HealthData{
  uint8_t length;
  uint8_t type;
  float battery_voltage;
  float battery_current;
  float battery_charge;
  float battery_temperature;
  float internal_temperature;
  float external_temperature;
  unsigned long int sd_memory_usage;
  uint8_t rasp_data[N];
};

struct LightningData{
  float x;
  float y;
  float radius;
};

struct ImagingData{
  uint8_t length;
  uint8_t type;
  LightningData lightnings[5];
};

// struct TelemetryPacket{
//   uint8_t index;
//   uint8_t data[56];
// };

union SatData{
  uint8_t number_of_packets;
  uint8_t index;
  HealthData healthData;
  ImagingData imagingData;
};

struct SatPacket{
  uint8_t length;
  ProtocolOperation operation;
  SatData data;
};

// extern Message message;
// extern TelemetryData telemetryData;
extern SatPacket satPacket;
extern GSPacket gsPacket;