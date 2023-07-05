//
// Created by chengfeng on 2020/10/5.
//

#ifndef HSLIDARP64_H
#define HSLIDARP64_H
#include <udp_protocol_header.h>
#include <cstdint>
#include <cstdio>
namespace hesai
{
namespace lidar
{

#ifdef _MSC_VER
#define PACKED
#pragma pack(push, 1)
#else
#define PACKED __attribute__((packed))
#endif

struct HS_LIDAR_L64_Header {
  // 0xFFEE 2bytes
  uint16_t m_u16Sob;
  uint8_t m_u8LaserNum;
  uint8_t m_u8BlockNum;
  // return mode 1 byte  when dual return 0-Single Return ,
  // 1-The first block is the 1 st return. 2-The first block
  // is the 2 nd return
  uint8_t m_u8ReturnType;
  uint8_t m_u8DistUnit;
  uint8_t m_u8Reserved0;
  uint8_t m_u8Reserved1;

  HS_LIDAR_L64_Header()
      : m_u16Sob(0),
        m_u8LaserNum(0),
        m_u8BlockNum(0),
        m_u8ReturnType(0),
        m_u8DistUnit(0),
        m_u8Reserved0(0),
        m_u8Reserved1(0) {}

  uint8_t GetLaserNum() const { return m_u8LaserNum; }
  uint8_t GetBlockNum() const { return m_u8BlockNum; }
  double GetDistUnit() const { return m_u8DistUnit / 1000.f; }
  uint8_t GetReturnType() const { return m_u8ReturnType; }
} PACKED;

struct HS_LIDAR_BODY_CHN_UNIT_L64 {
  uint16_t m_u16Distance;
  uint8_t m_u8Reflectivity;
  uint16_t GetDistance() const { return little_to_native(m_u16Distance); }
  uint8_t GetReflectivity() const { return m_u8Reflectivity; }
} PACKED;

struct HS_LIDAR_BODY_AZIMUTH_L64 {
  uint16_t m_u16Azimuth;

  uint16_t GetAzimuth() const { return little_to_native(m_u16Azimuth); }

  void Print() const {
    printf("HS_LIDAR_BODY_AZIMUTH_L64: azimuth:%u\n", GetAzimuth());
  }
} PACKED;

struct HS_LIDAR_TAIL_SEQ_NUM_L64 {
  uint32_t m_u32SeqNum;

  uint32_t GetSeqNum() const { return little_to_native(m_u32SeqNum); }
  static uint32_t GetSeqNumSize() { return sizeof(m_u32SeqNum); }

  void Print() const {
    printf("HS_LIDAR_TAIL_SEQ_NUM_L64:\n");
    printf("seqNum: %u\n", GetSeqNum());
  }
} PACKED;

struct HS_LIDAR_TAIL_L64 {
  ReservedInfo1 m_reservedInfo1;
  uint8_t m_u8Reserved0[2];
  uint8_t m_u8Shutdown;
  uint16_t m_u16ErrorCode;
  uint16_t m_u16MotorSpeed;
  uint32_t m_u32Timestamp;
  uint8_t m_u8ReturnMode;
  uint8_t m_u8FactoryInfo;
  uint8_t m_u8UTC[6];

  uint16_t GetMotorSpeed() const { return little_to_native(m_u16MotorSpeed); }
  uint8_t GetStatusID() const { return m_reservedInfo1.GetID(); }
  uint16_t GetStatusData() const { return m_reservedInfo1.GetData(); }
  uint16_t GetErrorCode() const { return little_to_native(m_u16ErrorCode); }

  uint32_t GetTimestamp() const { return little_to_native(m_u32Timestamp); }

  uint8_t GetUTCData(uint8_t index) const {
    return m_u8UTC[index < sizeof(m_u8UTC) ? index : 0];
  }

  int64_t GetMicroLidarTimeU64() const {
      if (m_u8UTC[0] != 0) {
          struct tm t = {0};
          t.tm_year = m_u8UTC[0] + 100;
          if (t.tm_year >= 200) {
              t.tm_year -= 100;
          }
          t.tm_mon = m_u8UTC[1] - 1;
          t.tm_mday = m_u8UTC[2];
          t.tm_hour = m_u8UTC[3];
          t.tm_min = m_u8UTC[4];
          t.tm_sec = m_u8UTC[5];
          t.tm_isdst = 0;
          return (mktime(&t) - timezone) * 1000000 + GetTimestamp();
      }
      else {
          uint32_t utc_time_big = *(uint32_t*)(&m_u8UTC[0] + 2);
          int64_t unix_second = ((utc_time_big >> 24) & 0xff) |
                  ((utc_time_big >> 8) & 0xff00) |
                  ((utc_time_big << 8) & 0xff0000) |
                  ((utc_time_big << 24));
          return unix_second * 1000000 + GetTimestamp();
      }
  }

} PACKED;
}  // namespace lidar
}  // namespace hesai
#endif  // INTENSITYRETEST_HSLIDARP64_H