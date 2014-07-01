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

#include "RTIMUMPU6050HMC5883L.h"
#include "RTIMUSettings.h"
#include <iostream>
//  this sets the learning rate for compass running average calculation

#define COMPASS_ALPHA 0.2f

RTIMUMPU6050HMC5883L::RTIMUMPU6050HMC5883L(RTIMUSettings *settings) : RTIMU(settings)
{

}

RTIMUMPU6050HMC5883L::~RTIMUMPU6050HMC5883L()
{
}

bool RTIMUMPU6050HMC5883L::setLpf(unsigned char lpf)
{
    switch (lpf) {
    case MPU6050_LPF_256:
    case MPU6050_LPF_188:
    case MPU6050_LPF_98:
    case MPU6050_LPF_42:
    case MPU6050_LPF_20:
    case MPU6050_LPF_10:
    case MPU6050_LPF_5:
        m_lpf = lpf;
        return true;

    default:
        HAL_ERROR1("Illegal MPU6050 lpf %d\n", lpf);
        return false;
    }
}


bool RTIMUMPU6050HMC5883L::setSampleRate(int rate)
{
    if ((rate < MPU6050_SAMPLERATE_MIN) || (rate > MPU6050_SAMPLERATE_MAX)) {
        HAL_ERROR1("Illegal sample rate %d\n", rate);
        return false;
    }
    m_sampleRate = rate;
    m_sampleInterval = (uint64_t)1000000 / m_sampleRate;
    return true;
}

bool RTIMUMPU6050HMC5883L::setCompassRate(int rate)
{
    if ((rate < MPU6050_COMPASSRATE_MIN) || (rate > MPU6050_COMPASSRATE_MAX)) {
        HAL_ERROR1("Illegal compass rate %d\n", rate);
        return false;
    }
    m_compassRate = rate;
    return true;
}

bool RTIMUMPU6050HMC5883L::setGyroFsr(unsigned char fsr)
{
    switch (fsr) {
    case MPU6050_GYROFSR_250:
        m_gyroFsr = fsr;
        m_gyroScale = RTMATH_PI / (131.0 * 180.0);
        return true;

    case MPU6050_GYROFSR_500:
        m_gyroFsr = fsr;
        m_gyroScale = RTMATH_PI / (62.5 * 180.0);
        return true;

    case MPU6050_GYROFSR_1000:
        m_gyroFsr = fsr;
        m_gyroScale = RTMATH_PI / (32.8 * 180.0);
        return true;

    case MPU6050_GYROFSR_2000:
        m_gyroFsr = fsr;
        m_gyroScale = RTMATH_PI / (16.4 * 180.0);
        return true;

    default:
        HAL_ERROR1("Illegal MPU6050 gyro fsr %d\n", fsr);
        return false;
    }
}

bool RTIMUMPU6050HMC5883L::setAccelFsr(unsigned char fsr)
{
    switch (fsr) {
    case MPU6050_ACCELFSR_2:
        m_accelFsr = fsr;
        m_accelScale = 1.0/16384.0;
        return true;

    case MPU6050_ACCELFSR_4:
        m_accelFsr = fsr;
        m_accelScale = 1.0/8192.0;
        return true;

    case MPU6050_ACCELFSR_8:
        m_accelFsr = fsr;
        m_accelScale = 1.0/4096.0;
        return true;

    case MPU6050_ACCELFSR_16:
        m_accelFsr = fsr;
        m_accelScale = 1.0/2048.0;
        return true;

    default:
        HAL_ERROR1("Illegal MPU6050 accel fsr %d\n", fsr);
        return false;
    }
}


bool RTIMUMPU6050HMC5883L::IMUInit()
{
    bool compassAvailable = false;
    
    unsigned char result;
    unsigned char resulthmc[4];

    m_firstTime = true;

#ifdef MPU6050_CACHE_MODE
    m_cacheIn = m_cacheOut = m_cacheCount = 0;
#endif

    // set validity flags

    m_imuData.fusionPoseValid = false;
    m_imuData.fusionQPoseValid = false;
    m_imuData.gyroValid = true;
    m_imuData.accelValid = true;
    m_imuData.compassValid = true;
    m_imuData.pressureValid = false;
    m_imuData.temperatureValid = true;
    m_imuData.humidityValid = false;

    //  configure IMU

    m_slaveAddr = m_settings->m_I2CSlaveAddress;
    m_bus = m_settings->m_I2CBus;

    setSampleRate(m_settings->m_MPU9150GyroAccelSampleRate);
    setCompassRate(m_settings->m_MPU9150CompassSampleRate);
    setLpf(m_settings->m_MPU9150GyroAccelLpf);
    setGyroFsr(m_settings->m_MPU9150GyroFsr);
    setAccelFsr(m_settings->m_MPU9150AccelFsr);

    setCalibrationData(m_settings->m_compassCalValid, m_settings->m_compassCalMin, m_settings->m_compassCalMax);

    //  enable the I2C bus
    setI2CBus(m_bus);
    if (!I2COpen())
        return false;

    //  reset the MPU6050

    if (!I2CWrite(m_slaveAddr, MPU6050_PWR_MGMT_1, 0x80, "Failed to initiate MPU6050 reset"))
        return false;

    delayMs(100);

    if (!I2CWrite(m_slaveAddr, MPU6050_PWR_MGMT_1, 0x00, "Failed to stop MPU6050 reset"))
        return false;

    if (!I2CRead(m_slaveAddr, MPU6050_WHO_AM_I, 1, &result, "Failed to read MPU6050 id"))
        return false;

    if (result != 0x68) {
        HAL_ERROR1("Incorrect MPU6050 id %d\n", result);
        return false;
    }

    //  now configure the various components

    if (!I2CWrite(m_slaveAddr, MPU6050_LPF_CONFIG, m_lpf, "Failed to set lpf"))
        return false;

    if (!setSampleRate())
        return false;

    if (!I2CWrite(m_slaveAddr, MPU6050_GYRO_CONFIG, m_gyroFsr, "Failed to set gyro fsr"))
        return false;

    if (!I2CWrite(m_slaveAddr, MPU6050_ACCEL_CONFIG, m_accelFsr, "Failed to set accel fsr"))
         return false;

    //  now configure compass

    bypassOn();

    // get fuse ROM data

    if (!I2CRead(HMC5883L_ADDRESS, HMC5883L_RA_ID_A, 3, resulthmc, "\rHMC5883L not found!              ")) {
        bypassOff();
        //return false;
        compassAvailable = false;
    } else if (resulthmc[0] != 'H' && resulthmc[1] != '4' && resulthmc[2] != '3') {
        resulthmc[3] = 0;
        HAL_ERROR1("Incorrect HMC5883L id %s\n", resulthmc);
        bypassOff();
        //return false;
        compassAvailable = false;
    } else
        compassAvailable = true;
    
    if (compassAvailable) {
        
    if (!I2CWrite(HMC5883L_ADDRESS, HMC5883L_RA_CONFIG_A,
            (HMC5883L_AVERAGING_8 << (HMC5883L_CRA_AVERAGE_BIT - HMC5883L_CRA_AVERAGE_LENGTH + 1)) |
            (HMC5883L_RATE_30 << (HMC5883L_CRA_RATE_BIT - HMC5883L_CRA_RATE_LENGTH + 1)) |
            (HMC5883L_BIAS_NORMAL << (HMC5883L_CRA_BIAS_BIT - HMC5883L_CRA_BIAS_LENGTH + 1)), "Failed to set compass in power down mode 1")) {
        bypassOff();
        return false;
    }

    //I2CWrite(HMC5883L_ADDRESS, 0x3C, 0, NULL, "Failed to read HMC5883L data");
    if (!I2CWrite(HMC5883L_ADDRESS, HMC5883L_RA_CONFIG_B, HMC5883L_GAIN_1090 << (HMC5883L_CRB_GAIN_BIT - HMC5883L_CRB_GAIN_LENGTH + 1), "Failed to set gain")) {
        bypassOff();
        return false;
    }

    //I2CWrite(HMC5883L_ADDRESS, 0x3C, 0, NULL, "Failed to read HMC5883L data");
    if (!I2CWrite(HMC5883L_ADDRESS, HMC5883L_RA_MODE, HMC5883L_MODE_CONTINUOUS << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1), "Failed to set mode")) {
        bypassOff();
        return false;
    }

    /*
    while (true) {
        unsigned char buf[6];

        //I2CWrite(HMC5883L_ADDRESS, 0x3D, 0, NULL, "Failed to read HMC5883L data");
        if (!I2CRead(HMC5883L_ADDRESS, HMC5883L_RA_STATUS, 1, buf, "Failed to read HMC5883L status")) {
            bypassOff();
            return false;
        }
        if ((buf[0] & 0x1) == 0) {
            HAL_ERROR1("data not ready %d\n", buf[0]);
            continue;
        }

        //I2CWrite(HMC5883L_ADDRESS, 0x3D, 0, NULL, "Failed to read HMC5883L data");
        if (!I2CRead(HMC5883L_ADDRESS, HMC5883L_RA_DATAX_H, 6, buf, "Failed to read HMC5883L data")) {
            bypassOff();
            return false;
        }


        int16_t x = static_cast<int16_t> ((buf[0] << 8) | buf[1]);
        int16_t z = static_cast<int16_t> ((buf[2] << 8) | buf[3]);
        int16_t y = static_cast<int16_t> ((buf[4] << 8) | buf[5]);

        //float xx = (float) (x * HMC5883L_scale[HMC5883L_GAIN_1090]);
        //float yy = (float) (y * HMC5883L_scale[HMC5883L_GAIN_1090]);
        //float zz = (float) (z * HMC5883L_scale[HMC5883L_GAIN_1090]);

        float angle = atan2(y, x) * 180 / M_PI;


        //RTVector3 vvv;
        //RTMath::convertToVector(aaa, vvv, 1.0f, true);
        //HAL_ERROR1(RTMath::displayRadians("AAAAAAAAAAA ", vvv), 1);

        //HAL_ERROR1("compassData X %X", buf[0]);
        //HAL_ERROR1(" %X\t", buf[1]);
        //HAL_ERROR1("Z %X", buf[2]);
        //HAL_ERROR1(" %X\t", buf[3]);
        //HAL_ERROR1("Y %X", buf[4]);
        //HAL_ERROR1(" %X\n", buf[5]);

        HAL_ERROR1("X %d\t", x);
        HAL_ERROR1("Y %d\t", y);
        HAL_ERROR1("Z %d\t", z);
        HAL_ERROR1("angle %f\n", angle);

        delayMs(100);
    }
    */

    //  convert asa to usable scale factor

    m_compassAdjust[0] = 1.0f; //((float)asa[0] - 128.0) / 256.0 + 1.0f;
    m_compassAdjust[1] = 1.0f; //((float)asa[1] - 128.0) / 256.0 + 1.0f;
    m_compassAdjust[2] = 1.0f; //((float)asa[2] - 128.0) / 256.0 + 1.0f;

    bypassOff();

    //  now set up MPU6050 to talk to the compass chip

    if (!I2CWrite(m_slaveAddr, MPU6050_I2C_MST_CTRL, 0x40, "Failed to set I2C master mode"))
        return false;


    if (!I2CWrite(m_slaveAddr, MPU6050_I2C_SLV0_ADDR, 0x80 | HMC5883L_ADDRESS, "Failed to set slave 0 address"))
        return false;

    if (!I2CWrite(m_slaveAddr, MPU6050_I2C_SLV0_REG, HMC5883L_RA_DATAX_H, "Failed to set slave 0 reg"))
        return false;

    if (!I2CWrite(m_slaveAddr, MPU6050_I2C_SLV0_CTRL, 0b10100110, "Failed to set slave 0 ctrl"))
        return false;

    if (!I2CWrite(m_slaveAddr, MPU6050_I2C_MST_DELAY_CTRL, 0x1, "Failed to set mst delay"))
        return false;

    /*
    if (!I2CWrite(m_slaveAddr, MPU6050_YG_OFFS_TC, 0x80, "Failed to set yg offs tc"))
        return false;
    */

    if (!setCompassRate())
        return false;
    }
    //  enable the sensors

    if (!I2CWrite(m_slaveAddr, MPU6050_PWR_MGMT_1, 1, "Failed to set pwr_mgmt_1"))
        return false;

    if (!I2CWrite(m_slaveAddr, MPU6050_PWR_MGMT_2, 0, "Failed to set pwr_mgmt_2"))
         return false;

    //  select the data to go into the FIFO and enable

    if (!resetFifo())
        return false;

    m_gyroAlpha = 1.0f / m_sampleRate;
    m_gyroStartTime = RTMath::currentUSecsSinceEpoch();
    m_gyroLearning = true;

    HAL_INFO("MPU6050 init complete\n");
    return true;
}

bool RTIMUMPU6050HMC5883L::resetFifo()
{
    if (!I2CWrite(m_slaveAddr, MPU6050_INT_ENABLE, 0, "Writing int enable"))
        return false;
    if (!I2CWrite(m_slaveAddr, MPU6050_FIFO_EN, 0, "Writing fifo enable"))
        return false;
    if (!I2CWrite(m_slaveAddr, MPU6050_USER_CTRL, 0, "Writing user control"))
        return false;

    if (!I2CWrite(m_slaveAddr, MPU6050_USER_CTRL, 0x04, "Resetting fifo"))
        return false;

    if (!I2CWrite(m_slaveAddr, MPU6050_USER_CTRL, 0x60, "Enabling the fifo"))
        return false;

    delayMs(50);

    if (!I2CWrite(m_slaveAddr, MPU6050_INT_ENABLE, 1, "Writing int enable"))
        return false;

    if (!I2CWrite(m_slaveAddr, MPU6050_FIFO_EN, 0x78 /*|| 0x1*/, "Failed to set FIFO enables"))
        return false;

    return true;
}

bool RTIMUMPU6050HMC5883L::bypassOn()
{
    unsigned char userControl;

    if (!I2CRead(m_slaveAddr, MPU6050_USER_CTRL, 1, &userControl, "Failed to read user_ctrl reg"))
        return false;

    userControl &= ~0x20;

    if (!I2CWrite(m_slaveAddr, MPU6050_USER_CTRL, 1, &userControl, "Failed to write user_ctrl reg"))
        return false;

    delayMs(50);

    if (!I2CWrite(m_slaveAddr, MPU6050_INT_PIN_CFG, 0x82, "Failed to write int_pin_cfg reg"))
        return false;

    delayMs(50);
    return true;
}


bool RTIMUMPU6050HMC5883L::bypassOff()
{
    unsigned char userControl;

    if (!I2CRead(m_slaveAddr, MPU6050_USER_CTRL, 1, &userControl, "Failed to read user_ctrl reg"))
        return false;

    userControl |= 0x20;

    if (!I2CWrite(m_slaveAddr, MPU6050_USER_CTRL, 1, &userControl, "Failed to write user_ctrl reg"))
        return false;

    delayMs(50);

    if (!I2CWrite(m_slaveAddr, MPU6050_INT_PIN_CFG, 0x80, "Failed to write int_pin_cfg reg"))
         return false;

    delayMs(50);
    return true;
}

bool RTIMUMPU6050HMC5883L::setSampleRate()
{
    int clockRate = 1000;

    if (m_lpf == MPU6050_LPF_256)
        clockRate = 8000;

    if (!I2CWrite(m_slaveAddr, MPU6050_SMPRT_DIV, (unsigned char)(clockRate / m_sampleRate - 1),
                  "Failed to set sample rate"))
        return false;

    return true;
}

bool RTIMUMPU6050HMC5883L::setCompassRate()
{
    int rate;

    rate = m_sampleRate / m_compassRate - 1;

    if (rate > 31)
        rate = 31;

    // FIX
    if (!I2CWrite(m_slaveAddr, MPU6050_I2C_SLV4_CTRL, rate, "Failed to set slave ctrl 4"))
         return false;
    return true;
}

int RTIMUMPU6050HMC5883L::IMUGetPollInterval()
{
    return (400 / m_sampleRate);
}

bool RTIMUMPU6050HMC5883L::IMURead()
{
    unsigned char fifoCount[2];
    unsigned int count;
    unsigned char fifoData[12];
    unsigned char compassData[8];
    unsigned char temperatureData[2];

    if (!I2CRead(m_slaveAddr, MPU6050_FIFO_COUNT_H, 2, fifoCount, "Failed to read fifo count"))
         return false;

    count = ((unsigned int)fifoCount[0] << 8) + fifoCount[1];

    if (count == 1024) {
        HAL_INFO("MPU6050 fifo has overflowed");
        resetFifo();
        m_imuData.timestamp += m_sampleInterval * (1024 / MPU6050_FIFO_CHUNK_SIZE + 1); // try to fix timestamp
        return false;
    }


#ifdef MPU6050_CACHE_MODE
    if ((m_cacheCount == 0) && (count  >= MPU6050_FIFO_CHUNK_SIZE) && (count < (MPU6050_CACHE_SIZE * MPU6050_FIFO_CHUNK_SIZE))) {
        // special case of a small fifo and nothing cached - just handle as simple read

        if (!I2CRead(m_slaveAddr, MPU6050_FIFO_R_W, MPU6050_FIFO_CHUNK_SIZE, fifoData, "Failed to read fifo data"))
            return false;

        if (!I2CRead(m_slaveAddr, MPU6050_EXT_SENS_DATA_00, 6, compassData, "Failed to read compass data"))
            return false;

        if (!I2CRead(m_slaveAddr, MPU6050_RA_TEMP_OUT_H, 2, temperatureData, "Failed to read temperature data"))
            return false;

    } else {
        if (count >= (MPU6050_CACHE_SIZE * MPU6050_FIFO_CHUNK_SIZE)) {
            if (m_cacheCount == MPU6050_CACHE_BLOCK_COUNT) {
                // all cache blocks are full - discard oldest and update timestamp to account for lost samples
                m_imuData.timestamp += m_sampleInterval * m_cache[m_cacheOut].count;
                if (++m_cacheOut == MPU6050_CACHE_BLOCK_COUNT)
                    m_cacheOut = 0;
                m_cacheCount--;
            }

            int blockCount = count / MPU6050_FIFO_CHUNK_SIZE;   // number of chunks in fifo

            if (blockCount > MPU6050_CACHE_SIZE)
                blockCount = MPU6050_CACHE_SIZE;

            if (!I2CRead(m_slaveAddr, MPU6050_FIFO_R_W, MPU6050_FIFO_CHUNK_SIZE * blockCount,
                                m_cache[m_cacheIn].data, "Failed to read fifo data"))
                return false;

            if (!I2CRead(m_slaveAddr, MPU6050_EXT_SENS_DATA_00, 6, m_cache[m_cacheIn].compass, "Failed to read compass data"))
                return false;

            if (!I2CRead(m_slaveAddr, MPU6050_RA_TEMP_OUT_H, 2, m_cache[m_cacheIn].temperature, "Failed to read temperature data"))
                return false;

            m_cache[m_cacheIn].count = blockCount;
            m_cache[m_cacheIn].index = 0;

            m_cacheCount++;
            if (++m_cacheIn == MPU6050_CACHE_BLOCK_COUNT)
                m_cacheIn = 0;

        }

        //  now fifo has been read if necessary, get something to process

        if (m_cacheCount == 0)
            return false;

        memcpy(fifoData, m_cache[m_cacheOut].data + m_cache[m_cacheOut].index, MPU6050_FIFO_CHUNK_SIZE);
        memcpy(compassData, m_cache[m_cacheOut].compass, 6);

        m_cache[m_cacheOut].index += MPU6050_FIFO_CHUNK_SIZE;

        if (--m_cache[m_cacheOut].count == 0) {
            //  this cache block is now empty

            if (++m_cacheOut == MPU6050_CACHE_BLOCK_COUNT)
                m_cacheOut = 0;
            m_cacheCount--;
        }
    }

#else

    if (count > MPU6050_FIFO_CHUNK_SIZE * 40) {
        // more than 40 samples behind - going too slowly so discard some samples but maintain timestamp correctly
        while (count >= MPU6050_FIFO_CHUNK_SIZE * 10) {
            if (!I2CRead(m_slaveAddr, MPU6050_FIFO_R_W, MPU6050_FIFO_CHUNK_SIZE, fifoData, "Failed to read fifo data"))
                return false;
            count -= MPU6050_FIFO_CHUNK_SIZE;
            m_imuData.timestamp += m_sampleInterval;
        }
    }

    if (count < MPU6050_FIFO_CHUNK_SIZE)
        return false;

    if (!I2CRead(m_slaveAddr, MPU6050_FIFO_R_W, MPU6050_FIFO_CHUNK_SIZE, fifoData, "Failed to read fifo data"))
        return false;

    if (!I2CRead(m_slaveAddr, MPU6050_EXT_SENS_DATA_00, 6, compassData, "Failed to read compass data"))
        return false;

    if (!I2CRead(m_slaveAddr, MPU6050_RA_TEMP_OUT_H, 2, temperatureData, "Failed to read temperature data"))
    return false;

#endif

    RTMath::convertToVector(fifoData, m_imuData.accel, m_accelScale, true);
    RTMath::convertToVector(fifoData + 6, m_imuData.gyro, m_gyroScale, true);
    RTMath::convertToVector(compassData, m_imuData.compass, 1.0f /*0.3f*/, true);
    //bypassOn();
    //I2CRead(HMC5883L_ADDRESS, HMC5883L_RA_DATAX_H, 6, compassData, "Failed to read compass data");
    //bypassOff();
    /*
        int16_t x = static_cast<int16_t> ((compassData[0] << 8) | compassData[1]);
        int16_t z = static_cast<int16_t> ((compassData[2] << 8) | compassData[3]);
        int16_t y = static_cast<int16_t> ((compassData[4] << 8) | compassData[5]);
        float angle = atan2(y, x) * 180 / M_PI;
        HAL_ERROR1("X %d\t", x);
        HAL_ERROR1("Y %d\t", y);
        HAL_ERROR1("Z %d\t", z);
        HAL_ERROR1("angle %f\n", angle);
     */
//    HAL_ERROR1(RTMath::displayRadians("", m_imuData.compass), 1);

    m_imuData.temperature = ((RTFLOAT)((int16_t)(((uint16_t)temperatureData[0] << 8) | (uint16_t)temperatureData[1])) + 12412) / 340.0;

    if (m_gyroLearning) {
        // update gyro bias

        m_gyroBias.setX((1.0 - m_gyroAlpha) * m_gyroBias.x() + m_gyroAlpha * m_imuData.gyro.x());
        m_gyroBias.setY((1.0 - m_gyroAlpha) * m_gyroBias.y() + m_gyroAlpha * m_imuData.gyro.y());
        m_gyroBias.setZ((1.0 - m_gyroAlpha) * m_gyroBias.z() + m_gyroAlpha * m_imuData.gyro.z());

        if ((RTMath::currentUSecsSinceEpoch() - m_gyroStartTime) > 5000000) {
            m_gyroLearning = false;                     // only do this for 5 seconds
            //printf("Gyro Bias: %s\r", RTMath::displayRadians("", m_gyroBias));
        }
    }

    //  sort out gyro axes and correct for bias

    m_imuData.gyro.setX(m_imuData.gyro.x() - m_gyroBias.x());
    m_imuData.gyro.setY(-(m_imuData.gyro.y() - m_gyroBias.y()));
    m_imuData.gyro.setZ(-(m_imuData.gyro.z() - m_gyroBias.z()));

    //  sort out accel data;

    //m_imuData.accel.setX(-m_imuData.accel.x());

    //  sort out compass axes

    float temp;
    // wilds: remove - swap x y
    /*
        temp = m_imuData.compass.x();
    m_imuData.compass.setX(m_imuData.compass.y());
    m_imuData.compass.setY(-temp);
    */

    // HMC5853L has data xzy
    temp = m_imuData.compass.z();
    m_imuData.compass.setZ(m_imuData.compass.y());
    m_imuData.compass.setY(temp);


    //  use the fuse data adjustments

    m_imuData.compass.setX(m_imuData.compass.x() * m_compassAdjust[0]);
    m_imuData.compass.setY(m_imuData.compass.y() * m_compassAdjust[1]);
    m_imuData.compass.setZ(m_imuData.compass.z() * m_compassAdjust[2]);

    //  calibrate if required

    if (!m_calibrationMode && m_calibrationValid) {
        m_imuData.compass.setX((m_imuData.compass.x() - m_compassCalOffset[0]) * m_compassCalScale[0]);
        m_imuData.compass.setY((m_imuData.compass.y() - m_compassCalOffset[1]) * m_compassCalScale[1]);
        m_imuData.compass.setZ((m_imuData.compass.z() - m_compassCalOffset[2]) * m_compassCalScale[2]);
    }

    //  update running average

    m_compassAverage.setX(m_imuData.compass.x() * COMPASS_ALPHA + m_compassAverage.x() * (1.0 - COMPASS_ALPHA));
    m_compassAverage.setY(m_imuData.compass.y() * COMPASS_ALPHA + m_compassAverage.y() * (1.0 - COMPASS_ALPHA));
    m_compassAverage.setZ(m_imuData.compass.z() * COMPASS_ALPHA + m_compassAverage.z() * (1.0 - COMPASS_ALPHA));
    m_imuData.compass = m_compassAverage;

    if (m_firstTime)
        m_imuData.timestamp = RTMath::currentUSecsSinceEpoch();
    else
        m_imuData.timestamp += m_sampleInterval;

    m_firstTime = false;

    //  now update the filter

    updateFusion();

    return true;
}
