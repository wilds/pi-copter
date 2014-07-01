//
//  Copyright (C) 2014 Danilo Selvaggi & Ivano Selvaggi
//  Copyright (c) 2014 richards-tech
//
//  This file is part of RTIMULib
//
//  RTIMULib is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  RTIMULib is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with RTIMULib.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _RTIMUMPU6050_H
#define	_RTIMUMPU6050_H

#include "RTIMU.h"

//  Define this symbol to use cache mode

#define MPU6050_CACHE_MODE

//  MPU9150 I2C Slave Addresses

#define MPU6050_ADDRESS0            0x68
#define MPU6050_ADDRESS1            0x69
#define MPU6050_ID                  0x68

//#define AK8975_ADDRESS              0x0c

//  Register map

#define MPU6050_YG_OFFS_TC          0x01
#define MPU6050_SMPRT_DIV           0x19
#define MPU6050_LPF_CONFIG          0x1a
#define MPU6050_GYRO_CONFIG         0x1b
#define MPU6050_ACCEL_CONFIG        0x1c
#define MPU6050_FIFO_EN             0x23
#define MPU6050_I2C_MST_CTRL        0x24
#define MPU6050_I2C_SLV0_ADDR       0x25
#define MPU6050_I2C_SLV0_REG        0x26
#define MPU6050_I2C_SLV0_CTRL       0x27
#define MPU6050_I2C_SLV1_ADDR       0x28
#define MPU6050_I2C_SLV1_REG        0x29
#define MPU6050_I2C_SLV1_CTRL       0x2a
#define MPU6050_I2C_SLV2_ADDR       0x2b
#define MPU6050_I2C_SLV2_REG        0x2c
#define MPU6050_I2C_SLV2_CTRL       0x2d
#define MPU6050_I2C_SLV3_ADDR       0x2e
#define MPU6050_I2C_SLV3_REG        0x2f
#define MPU6050_I2C_SLV3_CTRL       0x30
#define MPU6050_I2C_SLV4_CTRL       0x34
#define MPU6050_INT_PIN_CFG         0x37
#define MPU6050_INT_ENABLE          0x38
#define MPU6050_INT_STATUS          0x3a
#define MPU6050_ACCEL_XOUT_H        0x3b
#define MPU6050_RA_TEMP_OUT_H       0x41
#define MPU6050_GYRO_XOUT_H         0x43
#define MPU6050_EXT_SENS_DATA_00    0x49
#define MPU6050_I2C_SLV0_DO         0x63
#define MPU6050_I2C_SLV1_DO         0x64
#define MPU6050_I2C_SLV2_DO         0x65
#define MPU6050_I2C_SLV3_DO         0x66
#define MPU6050_I2C_MST_DELAY_CTRL  0x67
#define MPU6050_USER_CTRL           0x6a
#define MPU6050_PWR_MGMT_1          0x6b
#define MPU6050_PWR_MGMT_2          0x6c
#define MPU6050_FIFO_COUNT_H        0x72
#define MPU6050_FIFO_R_W            0x74
#define MPU6050_WHO_AM_I            0x75

//  sample rate defines (applies to gyros and accels, not mags)

#define MPU6050_SAMPLERATE_MIN      5                      // 5 samples per second is the lowest
#define MPU6050_SAMPLERATE_MAX      1000                   // 1000 samples per second is the absolute maximum

//  compass rate defines

#define MPU6050_COMPASSRATE_MIN     1                      // 1 samples per second is the lowest
#define MPU6050_COMPASSRATE_MAX     100                    // 100 samples per second is maximum

//  LPF options (gyros and accels)

#define MPU6050_LPF_256             0                       // gyro: 256Hz, accel: 260Hz
#define MPU6050_LPF_188             1                       // gyro: 188Hz, accel: 184Hz
#define MPU6050_LPF_98              2                       // gyro: 98Hz, accel: 98Hz
#define MPU6050_LPF_42              3                       // gyro: 42Hz, accel: 44Hz
#define MPU6050_LPF_20              4                       // gyro: 20Hz, accel: 21Hz
#define MPU6050_LPF_10              5                       // gyro: 10Hz, accel: 10Hz
#define MPU6050_LPF_5               6                       // gyro: 5Hz, accel: 5Hz

//  Gyro FSR options

#define MPU6050_GYROFSR_250         0                       // +/- 250 degrees per second
#define MPU6050_GYROFSR_500         8                       // +/- 500 degrees per second
#define MPU6050_GYROFSR_1000        0x10                    // +/- 1000 degrees per second
#define MPU6050_GYROFSR_2000        0x18                    // +/- 2000 degrees per second

//  Accel FSR options

#define MPU6050_ACCELFSR_2          0                       // +/- 2g
#define MPU6050_ACCELFSR_4          8                       // +/- 4g
#define MPU6050_ACCELFSR_8          0x10                    // +/- 8g
#define MPU6050_ACCELFSR_16         0x18                    // +/- 16g


//  AK8975 compass registers

#define HMC5883L_ADDRESS            0x1E // this device only has one address

#define HMC5883L_RA_CONFIG_A        0x00
#define HMC5883L_RA_CONFIG_B        0x01
#define HMC5883L_RA_MODE            0x02
#define HMC5883L_RA_DATAX_H         0x03
#define HMC5883L_RA_DATAX_L         0x04
#define HMC5883L_RA_DATAZ_H         0x05
#define HMC5883L_RA_DATAZ_L         0x06
#define HMC5883L_RA_DATAY_H         0x07
#define HMC5883L_RA_DATAY_L         0x08
#define HMC5883L_RA_STATUS          0x09
#define HMC5883L_RA_ID_A            0x0A
#define HMC5883L_RA_ID_B            0x0B
#define HMC5883L_RA_ID_C            0x0C

#define HMC5883L_CRA_AVERAGE_BIT    6
#define HMC5883L_CRA_AVERAGE_LENGTH 2
#define HMC5883L_CRA_RATE_BIT       4
#define HMC5883L_CRA_RATE_LENGTH    3
#define HMC5883L_CRA_BIAS_BIT       1
#define HMC5883L_CRA_BIAS_LENGTH    2

#define HMC5883L_AVERAGING_1        0x00
#define HMC5883L_AVERAGING_2        0x01
#define HMC5883L_AVERAGING_4        0x02
#define HMC5883L_AVERAGING_8        0x03

#define HMC5883L_RATE_0P75          0x00
#define HMC5883L_RATE_1P5           0x01
#define HMC5883L_RATE_3             0x02
#define HMC5883L_RATE_7P5           0x03
#define HMC5883L_RATE_15            0x04
#define HMC5883L_RATE_30            0x05
#define HMC5883L_RATE_75            0x06

#define HMC5883L_BIAS_NORMAL        0x00
#define HMC5883L_BIAS_POSITIVE      0x01
#define HMC5883L_BIAS_NEGATIVE      0x02

#define HMC5883L_CRB_GAIN_BIT       7
#define HMC5883L_CRB_GAIN_LENGTH    3

#define HMC5883L_GAIN_1370          0x00
#define HMC5883L_GAIN_1090          0x01
#define HMC5883L_GAIN_820           0x02
#define HMC5883L_GAIN_660           0x03
#define HMC5883L_GAIN_440           0x04
#define HMC5883L_GAIN_390           0x05
#define HMC5883L_GAIN_330           0x06
#define HMC5883L_GAIN_220           0x07

//float HMC5883L_scale[] = {0.73f, 0.92f, 1.22f, 1.52f, 2.27f, 2.56f, 3.03f, 4.35f};

#define HMC5883L_MODEREG_BIT        1
#define HMC5883L_MODEREG_LENGTH     2

#define HMC5883L_MODE_CONTINUOUS    0x00
#define HMC5883L_MODE_SINGLE        0x01
#define HMC5883L_MODE_IDLE          0x02

#define HMC5883L_STATUS_LOCK_BIT    1
#define HMC5883L_STATUS_READY_BIT   0

/*
#define AK8975_DEVICEID             0x0                     // the device ID
#define AK8975_ST1                  0x02                    // status 1
#define AK8975_CNTL                 0x0a                    // control reg
#define AK8975_ASAX                 0x10                    // start of the fuse ROM data
*/

//  FIFO transfer size

#define MPU6050_FIFO_CHUNK_SIZE     12                      // gyro and accels take 12 bytes

#ifdef MPU6050_CACHE_MODE

//  Cache mode defines

#define MPU6050_CACHE_SIZE          16                      // number of chunks in a block
#define MPU6050_CACHE_BLOCK_COUNT   16                      // number of cache blocks

typedef struct
{
    unsigned char data[MPU6050_FIFO_CHUNK_SIZE * MPU6050_CACHE_SIZE];
    int count;                                              // number of chunks in the cache block
    int index;                                              // current index into the cache
    unsigned char compass[8];                               // the raw compass readings for the block
    unsigned char temperature[2];

} MPU6050_CACHE_BLOCK;

#endif


class RTIMUMPU6050HMC5883L : public RTIMU
{
public:
    RTIMUMPU6050HMC5883L(RTIMUSettings *settings);
    ~RTIMUMPU6050HMC5883L();

    bool setLpf(unsigned char lpf);
    bool setSampleRate(int rate);
    bool setCompassRate(int rate);
    bool setGyroFsr(unsigned char fsr);
    bool setAccelFsr(unsigned char fsr);

    virtual const char *IMUName() { return "MPU-6050"; }
    virtual int IMUType() { return RTIMU_TYPE_MPU6050; }
    virtual bool IMUInit();
    virtual bool IMURead();
    virtual int IMUGetPollInterval();
    virtual bool IMUGyroBiasValid() { return !m_gyroLearning; }

private:
    bool bypassOn();                                        // talk to compass
    bool bypassOff();                                       // talk to MPU6050
    bool setSampleRate();
    bool setCompassRate();
    bool resetFifo();

    bool m_firstTime;                                       // if first sample

    unsigned char m_slaveAddr;                              // I2C address of MPU6050
    unsigned char m_bus;                                    // I2C bus (usually 1 for Raspberry Pi for example)

    unsigned char m_lpf;                                    // low pass filter setting
    int m_sampleRate;                                       // sample rate in Hz for gyro and accel
    uint64_t m_sampleInterval;                              // interval between samples in microseonds
    int m_compassRate;                                      // compass sample rate in Hz
    unsigned char m_gyroFsr;
    unsigned char m_accelFsr;

    RTFLOAT m_gyroScale;
    RTFLOAT m_accelScale;

    RTFLOAT m_compassAdjust[3];                             // the compass fuse ROM values converted for use

    RTVector3 m_gyroBias;                                   // an accumulated bias for the gyros
    RTFLOAT m_gyroAlpha;                                    // gyro bias learning rate
    uint64_t m_gyroStartTime;                               // time at which the learning started
    bool m_gyroLearning;                                    // if in learning mode

    RTVector3 m_compassAverage;                             // a running average to smooth the mag outputs

#ifdef MPU6050_CACHE_MODE

    MPU6050_CACHE_BLOCK m_cache[MPU6050_CACHE_BLOCK_COUNT]; // the cache itself
    int m_cacheIn;                                          // the in index
    int m_cacheOut;                                         // the out index
    int m_cacheCount;                                       // number of used cache blocks

#endif

};

#endif // _RTIMUMPU6050_H
