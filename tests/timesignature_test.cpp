#include <drumcore/timesignature.h>
#include <gtest/gtest.h>

using namespace JKDigital;

TEST(TimeSignature, ActiveSteps_4_4) {
    EXPECT_EQ(TimeSignatureUtils::getActiveSteps(TimeSignature::k4_4), 32);
}

TEST(TimeSignature, ActiveSteps_3_4) {
    EXPECT_EQ(TimeSignatureUtils::getActiveSteps(TimeSignature::k3_4), 24);
}

TEST(TimeSignature, ActiveSteps_6_8) {
    EXPECT_EQ(TimeSignatureUtils::getActiveSteps(TimeSignature::k6_8), 24);
}

TEST(TimeSignature, ActiveSteps_7_8) {
    EXPECT_EQ(TimeSignatureUtils::getActiveSteps(TimeSignature::k7_8), 28);
}

TEST(TimeSignature, BeatsPerStep_IsConstant) {
    EXPECT_DOUBLE_EQ(TimeSignatureUtils::getBeatsPerStep(), 0.125);
}

TEST(TimeSignature, BeatsPerBar_4_4) {
    EXPECT_DOUBLE_EQ(TimeSignatureUtils::getBeatsPerBar(TimeSignature::k4_4), 4.0);
}

TEST(TimeSignature, BeatsPerBar_3_4) {
    EXPECT_DOUBLE_EQ(TimeSignatureUtils::getBeatsPerBar(TimeSignature::k3_4), 3.0);
}

TEST(TimeSignature, BeatsPerBar_6_8) {
    EXPECT_DOUBLE_EQ(TimeSignatureUtils::getBeatsPerBar(TimeSignature::k6_8), 3.0);
}

TEST(TimeSignature, BeatsPerBar_7_8) {
    EXPECT_DOUBLE_EQ(TimeSignatureUtils::getBeatsPerBar(TimeSignature::k7_8), 3.5);
}

TEST(TimeSignature, Numerator_Values) {
    EXPECT_DOUBLE_EQ(TimeSignatureUtils::getNumerator(TimeSignature::k4_4), 4.0);
    EXPECT_DOUBLE_EQ(TimeSignatureUtils::getNumerator(TimeSignature::k3_4), 3.0);
    EXPECT_DOUBLE_EQ(TimeSignatureUtils::getNumerator(TimeSignature::k6_8), 6.0);
    EXPECT_DOUBLE_EQ(TimeSignatureUtils::getNumerator(TimeSignature::k7_8), 7.0);
    EXPECT_DOUBLE_EQ(TimeSignatureUtils::getNumerator(TimeSignature::k12_8), 12.0);
}

TEST(TimeSignature, Denominator_Values) {
    EXPECT_DOUBLE_EQ(TimeSignatureUtils::getDenominator(TimeSignature::k4_4), 4.0);
    EXPECT_DOUBLE_EQ(TimeSignatureUtils::getDenominator(TimeSignature::k3_4), 4.0);
    EXPECT_DOUBLE_EQ(TimeSignatureUtils::getDenominator(TimeSignature::k6_8), 8.0);
    EXPECT_DOUBLE_EQ(TimeSignatureUtils::getDenominator(TimeSignature::k7_8), 8.0);
}
