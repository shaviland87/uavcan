/*
 * Copyright (C) 2014 Pavel Kirienko <pavel.kirienko@gmail.com>
 */

#include <gtest/gtest.h>
#include <uavcan/time.hpp>


TEST(Time, Monotonic)
{
    using uavcan::MonotonicDuration;
    using uavcan::MonotonicTime;

    MonotonicTime m1;
    MonotonicTime m2 = MonotonicTime::fromMSec(1);
    MonotonicDuration md1 = m2 - m1;  // 1000
    MonotonicDuration md2 = m1 - m2;  // -1000

    ASSERT_EQ(0, m1.toUSec());
    ASSERT_EQ(1000, m2.toUSec());
    ASSERT_EQ(1000, md1.toUSec());
    ASSERT_EQ(-1000, md2.toUSec());

    ASSERT_LT(m1, m2);
    ASSERT_LE(m1, m2);
    ASSERT_NE(m1, m2);
    ASSERT_TRUE(m1.isZero());
    ASSERT_FALSE(m2.isZero());

    ASSERT_GT(md1, md2);
    ASSERT_GE(md1, md2);
    ASSERT_NE(md1, md2);
    ASSERT_FALSE(md1.isZero());
    ASSERT_TRUE(md1.isPositive());
    ASSERT_TRUE(md2.isNegative());

    ASSERT_EQ(0, (md1 + md2).toUSec());
    ASSERT_EQ(2000, (md1 - md2).toUSec());

    md1 *= 2;                      // 2000
    ASSERT_EQ(2000, md1.toUSec());

    md2 += md1;                    // md2 = -1000 + 2000
    ASSERT_EQ(1000, md2.toUSec());

    ASSERT_EQ(-1000, (-md2).toUSec());

    /*
     * To string
     */
    ASSERT_EQ("0.000000", m1.toString());
    ASSERT_EQ("0.001000", m2.toString());

    ASSERT_EQ("0.002000", md1.toString());
    ASSERT_EQ("-0.001000", (-md2).toString());

    ASSERT_EQ("1001.000001", MonotonicTime::fromUSec(1001000001).toString());
    ASSERT_EQ("-1001.000001", MonotonicDuration::fromUSec(-1001000001).toString());
}


TEST(Time, Utc)
{
    using uavcan::UtcDuration;
    using uavcan::UtcTime;
    using uavcan::Timestamp;

    Timestamp ts;
    ts.husec = 90;

    UtcTime u1(ts);
    ASSERT_EQ(9000, u1.toUSec());

    ts.husec *= 2;
    u1 = ts;
    ASSERT_EQ(18000, u1.toUSec());

    ts = UtcTime::fromUSec(12345678900);
    ASSERT_EQ(123456789, ts.husec);

    /*
     * To string
     */
    ASSERT_EQ("0.018000", u1.toString());
    ASSERT_EQ("12345.678900", UtcTime(ts).toString());
}
