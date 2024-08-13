#ifndef __HPP__
#define __HPP__

#include "mpu6050.def"
#include "soft_i2c.hpp"
#include "math.h"
#include "string.h"

static constexpr uint8_t mpu6050_gnd_address = 0x68 << 1;
static constexpr uint8_t mpu6050_vcc_address = 0x69 << 1;

struct EulerAngle
{
    double yaw;
    double pitch;
    double roll;
};

struct Pose
{
    int16_t acc[3];
    int16_t gyro[3];
};

template <class i2c, uint8_t address>
class MPU6050
{
private:
    static constexpr int16_t gyro_scales[] = {250, 500, 1000, 2000};
    static constexpr int16_t acc_scales[] = {2, 4, 8, 16};
    static constexpr int16_t error[] = {17, 7, 15};
    static constexpr double g = 9.8;
    static int16_t gyro_scale;
    static int16_t acc_scale;

public:
    static Pose pose;
    static EulerAngle angle;
    static bool init(void);
    static bool set_bit(uint8_t reg, uint8_t bit);
    static bool clear_bit(uint8_t reg, uint8_t bit);
    static uint8_t read_bit(uint8_t reg, uint8_t bit);
    static bool clear_reg(uint8_t reg);
    static bool write_reg(uint8_t reg, uint8_t val);
    static uint8_t read_reg(uint8_t reg);
    static bool read_regs(uint8_t reg, uint8_t *buffer, size_t size);
    static bool modify_reg(uint8_t reg, uint8_t clear_mask, uint8_t set_mask);
    static uint8_t get_id(void);
    static Pose &update_pose(void);
    static EulerAngle &update_angle(double dt);
};

template <class i2c, uint8_t address> int16_t MPU6050<i2c, address>::gyro_scale;
template <class i2c, uint8_t address> int16_t MPU6050<i2c, address>::acc_scale;
template <class i2c, uint8_t address> Pose MPU6050<i2c, address>::pose;
template <class i2c, uint8_t address> EulerAngle MPU6050<i2c, address>::angle;

template <class i2c, uint8_t address>
bool MPU6050<i2c, address>::init(void)
{
    static_assert(address == MPU6050_AD0_TO_GND_ADDRESS || address == MPU6050_AD0_TO_VCC_ADDRESS);
    static_assert(is_specialization_of_soft_i2c<i2c>::value);

    memset((void *)&angle, 0, sizeof(angle));

    i2c::init();
    
    /* 不复位，关闭睡眠模式，不循环，使能温度传感器，选择X轴陀螺仪的内部震荡电路作为系统时钟 */
    if (!write_reg(PWR_MGMT_1, 1 << PWR_MGMT_1_CLKSEL_Pos))
        return false;

    /* 不需要设置循环模式的唤醒频率, 六个轴都不需要待机 */
    if (!write_reg(PWR_MGMT_2, 0))
        return false;
    
    /* 设置采样分频, 这里选择10分频 */
    if (!write_reg(SMPLRT_DIV, 9 << SMPLRT_DIV_SMPLRT_DIV_Pos))
        return false;

    /* 不需要外部同步, 数字低通滤波设置为最高(最平滑) */
    if (!write_reg(CONFIG, 6 << CONFIG_DLPF_CFG_Pos))
        return false;

    /**
     * @brief GYRO_CONFIG: 角速度计配置寄存器
     * bit[7](XG_ST): X 轴是否自检
     * bit[6](YG_ST): Y 轴是否自检
     * bit[5](ZG_ST): Z 轴是否自检
     * bit[4:3](FS_SEL[1:0]): 陀螺仪的满量程
     *     - 0: 250dps
     *     - 1: 500dps
     *     - 2: 1000dps
     *     - 3: 2000dps
     * 
     * @note 角速度计配置: 不自测(高三位为自测使能, 手册有遗漏), 设计为最大量程
     */
    if (!write_reg(GYRO_CONFIG, GYRO_CONFIG_FS_SEL_2000DPS))
        return false;

    /**
     * @brief ACCEL_CONFIG: 加速度计配置寄存器
     * bit[7](XA_ST): X 轴是否自检
     * bit[6](YA_ST): Y 轴是否自检
     * bit[5](ZA_ST): Z 轴是否自检
     * bit[4:3](AFS_SEL[1:0]): 加速度计的满量程
     *     - 0: 2g
     *     - 1: 4g
     *     - 2: 8g
     *     - 3: 16g
     * 
     * @note 加速度计配置: 不自测,选择为最大量程,不使用高通滤波器
     */
    if (!write_reg(ACCEL_CONFIG, ACCEL_CONFIG_AFS_SEL_16G))
        return false;

    return true;
}

template <class i2c, uint8_t address>
bool MPU6050<i2c, address>::set_bit(uint8_t reg, uint8_t bit)
{
    static_assert(is_specialization_of_soft_i2c<i2c>::value);
    return write_reg(reg, read_reg(reg) | bit);
}

template <class i2c, uint8_t address>
bool MPU6050<i2c, address>::clear_bit(uint8_t reg, uint8_t bit)
{
    static_assert(is_specialization_of_soft_i2c<i2c>::value);
    return write_reg(reg, read_reg(reg) & ~bit);
}

template <class i2c, uint8_t address>
uint8_t MPU6050<i2c, address>::read_bit(uint8_t reg, uint8_t bit)
{
    static_assert(is_specialization_of_soft_i2c<i2c>::value);
    return read_reg(reg) | bit;
}

template <class i2c, uint8_t address>
bool MPU6050<i2c, address>::clear_reg(uint8_t reg)
{
    static_assert(is_specialization_of_soft_i2c<i2c>::value);
    return write_reg(reg, 0);
}

template <class i2c, uint8_t address>
bool MPU6050<i2c, address>::write_reg(uint8_t reg, uint8_t val)
{
    static_assert(is_specialization_of_soft_i2c<i2c>::value);
    uint8_t bytes[] = {address, reg, val};

    i2c::start();
    for (size_t i = 0; i < sizeof(bytes); i++)
    {
        i2c::writebyte(bytes[i]);
        if (!i2c::wait_ack())
        {
            i2c::stop();
            return false;
        }
    }
    i2c::stop();
    return true;
}

template <class i2c, uint8_t address>
uint8_t MPU6050<i2c, address>::read_reg(uint8_t reg)
{
    static_assert(is_specialization_of_soft_i2c<i2c>::value);
    uint8_t byte;
    uint8_t bytes[] = {address, reg};

    i2c::start();
    for (size_t i = 0; i < sizeof(bytes); i++)
    {
        i2c::writebyte(bytes[i]);
        if (!i2c::wait_ack())
        {
            i2c::stop();
            return (uint8_t)-1;
        }
    }

    i2c::start();
    i2c::writebyte(address | 1);
    if (!i2c::wait_ack())
    {
        i2c::stop();
        return (uint8_t)-1;
    }
    byte = i2c::readbyte();
    i2c::no_ack();
    i2c::stop();

    return byte;
}

template <class i2c, uint8_t address>
bool MPU6050<i2c, address>::read_regs(uint8_t reg, uint8_t *buffer, size_t size)
{
    static_assert(is_specialization_of_soft_i2c<i2c>::value);
    uint8_t bytes[] = {address, reg};

    i2c::start();
    for (size_t i = 0; i < sizeof(bytes); i++)
    {
        i2c::writebyte(bytes[i]);
        if (!i2c::wait_ack())
        {
            i2c::stop();
            return false;
        }
    }

    i2c::start();
    i2c::writebyte(address | 1);
    if (!i2c::wait_ack())
    {
        i2c::stop();
        return false;
    }

    for (size_t i = 0; i < size; i++)
    {
        buffer[i] = i2c::readbyte();
        if (i != size - 1)
            i2c::ack();
        else
            i2c::nack();
    }
    i2c::stop();

    return true;
}

template <class i2c, uint8_t address>
bool MPU6050<i2c, address>::modify_reg(uint8_t reg, uint8_t clear_mask, uint8_t set_mask)
{
    static_assert(is_specialization_of_soft_i2c<i2c>::value);
    return write_reg(reg, (read_reg(reg) & ~clear_mask) | set_mask);
}

template <class i2c, uint8_t address>
uint8_t MPU6050<i2c, address>::get_id(void)
{
    static_assert(is_specialization_of_soft_i2c<i2c>::value);
    return read_reg(WHO_AM_I);
}

template <class i2c, uint8_t address>
Pose &MPU6050<i2c, address>::update_pose(void)
{
    static_assert(is_specialization_of_soft_i2c<i2c>::value);
    // static uint8_t acc_buffer[6];
    // static uint8_t gyro_buffer[6];

    // readbytes(ACCEL_XOUT_H, acc_buffer, 6);
    // readbytes(GYRO_XOUT_H, gyro_buffer, 6);
    // pose.acc.x = (acc_buffer[0] << 8) | acc_buffer[1];
    // pose.acc.y = (acc_buffer[2] << 8) | acc_buffer[3];
    // pose.acc.z = (acc_buffer[4] << 8) | acc_buffer[5];
    // pose.gyro.x = (gyro_buffer[0] << 8) | gyro_buffer[1];
    // pose.gyro.y = (gyro_buffer[2] << 8) | gyro_buffer[3];
    // pose.gyro.z = (gyro_buffer[4] << 8) | gyro_buffer[5];
    pose.acc[0] = (read_reg(ACCEL_XOUT_H) << 8) | read_reg(ACCEL_XOUT_L);
    pose.acc[1] = (read_reg(ACCEL_YOUT_H) << 8) | read_reg(ACCEL_YOUT_L);
    pose.acc[2] = (read_reg(ACCEL_ZOUT_H) << 8) | read_reg(ACCEL_ZOUT_L);
    pose.gyro[0] = (read_reg(GYRO_XOUT_H) << 8) | read_reg(GYRO_XOUT_L);
    pose.gyro[1] = (read_reg(GYRO_YOUT_H) << 8) | read_reg(GYRO_YOUT_L);
    pose.gyro[2] = (read_reg(GYRO_ZOUT_H) << 8) | read_reg(GYRO_ZOUT_L);

    return pose;
}

template <class i2c, uint8_t address>
EulerAngle &MPU6050<i2c, address>::update_angle(double dt)
{
    static_assert(is_specialization_of_soft_i2c<i2c>::value);
    angle.pitch = atan2(-pose.acc[0], sqrt(pose.acc[1] * pose.acc[1] + pose.acc[2] * pose.acc[2]));
    angle.roll = atan2(pose.acc[1], pose.acc[2]);
    angle.yaw += (pose.gyro[2]) * 250 / 32767 * dt;
    return angle;
}

#endif // !__HPP__
