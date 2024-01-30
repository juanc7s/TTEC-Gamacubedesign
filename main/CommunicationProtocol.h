#ifndef COMMUNICATION_PROTOCOL_H
#define COMMUNICATION_PROTOCOL_H

enum PROTOCOL{
  PROTOCOL_STATUS,
  PROTOCOL_IMAGING_DATA,
  PROTOCOL_SET_OPERATION
};  

enum PROTOCOL_STATUS{
  GS_STATUS_REQUEST,
  SATELLITE_STATUS_PACKETS_AVAILABLE,
  GS_STATUS_START_TRANSMISSION,
  SATELLITE_STATUS_PACKET,
  SATELLITE_STATUS_PACKETS_DONE,
  GS_STATUS_RESEND_PACKET,
  GS_STATUS_DONE,
  SATELLITE_STATUS_DONE
};

enum PROTOCOL_IMAGING_DATA{
  GS_IMAGING_REQUEST,
  SATELLITE_IMAGING_PACKETS_AVAILABLE,
  GS_IMAGING_START_TRANSMISSION,
  SATELLITE_IMAGING_PACKET,
  SATELLITE_IMAGING_PACKETS_DONE,
  GS_IMAGING_RESEND_STATUS,
  SATELLITE_IMAGING_DONE,
  GS_IMAGING_DONE
};

enum PROTOCOL_SET_OPERATION{
    GS_SET_OPERATION,
    SATELLITE_SET_OPERATION_ECHO,
    GS_SET_OPERATION_DONE,
    SATELLITE_SET_OPERATION_DONE
};

enum CONTROL_COMMAND{
  READ_HEAD,
  READ_ADDH,
  READ_ADDL,
  READ_CHAN,
  READ_AIR_DATA_RATE,
  READ_UART_BAUD_RATE,
  READ_PARITY,
  READ_TRANSMISSION_POWER,
  READ_TRANSMISSION_MODE,
  READ_OPERATION_MODE,
  READ_ALL,

  SET_HEAD,
  SET_ADDH,
  SET_ADDL,
  SET_CHAN,
  SET_AIR_DATA_RATE,
  SET_UART_BAUD_RATE,
  SET_PARITY,
  SET_TRANSMISSION_POWER,
  SET_TRANSMISSION_MODE,
  SET_OPERATION_MODE,
  SET_ALL,

  TOGGLE_QUERY_STATUS,
  SEND_QUERY_STATUS,
  TOGGLE_QUERY_IMAGING,
  SEND_QUERY_IMAGING,
  TOGGLE_COMMAND,
  SEND_COMMAND,

  SET_TX_ADDH,
  SET_TX_ADDL,
  SET_TX_CHAN,

  FLUSH,
  SET_MESSAGE_DATA,

  SET_ACTIVE_THERMAL_CONTROL,
  SET_ATTITUDE_CONTROL,
  SET_IMAGING,
  SET_IMAGING_MODE,
  SET_STAND_BY_MODE
};

struct ProtocolOperation{
    unsigned protocol : 2;
    unsigned operation : 6;
};

struct GSOperation{
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
  GSOperation operation;
  GSResend resend;
};

struct GSPacket{
  uint8_t length;
  ProtocolOperation operation;
  GSData data;
};

#define N 10 // Size of the added array
struct HealthData{
//   uint8_t length;
//   uint8_t type;
  unsigned long int index;
  unsigned long int time;
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
  unsigned long int index;
  float x;
  float y;
  float radius;
  float duration;
};

struct ImagingData{
//   uint8_t length;
//   uint8_t type;
  LightningData lightnings[5];
};

// struct TelemetryPacket{
//   uint8_t index;
//   uint8_t data[56];
// };

union SatData{
  uint8_t byte;
  uint8_t number_of_packets;
  HealthData healthData;
  ImagingData imagingData;
  GSOperation operation_echo;
};

struct SatPacket{
  uint8_t length;
  ProtocolOperation operation;
  uint8_t index;
  SatData data;
};

struct Operation{
  unsigned switch_active_thermal_control : 1;
  unsigned switch_attitude_control : 1;
  unsigned switch_imaging : 1;
  unsigned switch_imaging_mode : 1;
  unsigned switch_stand_by_mode : 1;
};

// extern Message message;
// extern TelemetryData telemetryData;
extern SatPacket satPacket;
extern GSPacket gsPacket;
extern Operation operation;

#endif
